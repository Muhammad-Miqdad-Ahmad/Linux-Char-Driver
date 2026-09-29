KDIR       := /lib/modules/$(shell uname -r)/build
ROOT_DIR   := $(CURDIR)
SRC_DIR    := $(ROOT_DIR)/src
INC_DIR    := $(ROOT_DIR)/include
BUILD_DIR  := $(ROOT_DIR)/build
MODULE_DIR := $(ROOT_DIR)/modules
CURRENT    := heavy_module

# Kbuild runs from $(KDIR); objects go to M=, sources are read from src=
KBUILD := $(MAKE) -C $(KDIR) M=$(BUILD_DIR) src=$(SRC_DIR) INC_DIR=$(INC_DIR)

.PHONY: all build bear add rm msg clean clean-build

all: build

build:
	@mkdir -p $(BUILD_DIR) $(MODULE_DIR)
	$(KBUILD) modules
	cp $(BUILD_DIR)/*.ko $(MODULE_DIR)/

# Build and generate compile_commands.json for clangd / VS Code
bear:
	$(MAKE) clean-build
	bear --output $(ROOT_DIR)/compile_commands.json -- $(MAKE) build

add: build
	sudo insmod $(MODULE_DIR)/$(CURRENT).ko

rm:
	sudo rmmod $(CURRENT)

msg:
	sudo dmesg | tail -n 30

clean-build:
	rm -rf $(BUILD_DIR) $(MODULE_DIR)

clean: clean-build
	rm -f $(ROOT_DIR)/compile_commands.json
