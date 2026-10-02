#---------------------------------------------------------------------------------
# Convenience wrapper around CMake (borealis by xfangfang is CMake based).
#   make        -> build SaltyNX-Tool.nro (deko3d) and copy it to the project root
#   make clean  -> remove build files
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>/devkitpro")
endif

TARGET	:=	SaltyNX-Tool
BUILD	:=	build
JOBS	?=	$(shell nproc 2>/dev/null || echo 4)

.PHONY: all configure clean

all: configure
	@cmake --build $(BUILD) --target $(TARGET).nro -j $(JOBS)
	@cp $(BUILD)/$(TARGET).nro $(TARGET).nro

configure:
	@[ -f $(BUILD)/CMakeCache.txt ] || cmake -S . -B $(BUILD) -DCMAKE_BUILD_TYPE=Release

clean:
	@echo clean ...
	@rm -rf $(BUILD) $(TARGET).nro
