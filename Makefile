#
# Copyright (c) Mateusz Maciejewski ^ TOXIC_DEATH.117, 2021 - 2023
#

# unified artifact FW .hex file for burn_ targets
SB_FW_BURN_FILE = sync_blaster_fw.hex

# GCC toolchain & platform utilities
CC = /c/msys64/home/Hareton/avr8-gnu-toolchain-win32_x86_64/bin/avr-gcc.exe
SIZE = /c/msys64/home/Hareton/avr8-gnu-toolchain-win32_x86_64/bin/avr-size.exe
OBJCOPY = /c/msys64/home/Hareton/avr8-gnu-toolchain-win32_x86_64/bin/avr-objcopy.exe
DUMP = /c/msys64/home/Hareton/avr8-gnu-toolchain-win32_x86_64/bin/avr-objdump.exe
STRIP = /c/msys64/home/Hareton/avr8-gnu-toolchain-win32_x86_64/bin/avr-strip.exe
DUDE = /c/Users/Hareton/Desktop/avrdude-old/avrdude.exe

# GCC parameters
CFLAGS = -Wall -Os -mmcu=$(GCC_MMCU)
LDFLAGS = -Wall -Os -mmcu=$(GCC_MMCU)

all:
	@echo "`tput rev; tput setaf 3` *** PLEASE SPECIFY BUILD TARGET FROM THE LIST BELOW        *** `tput sgr0`"
	@echo
	@echo " - Any:"
	@echo "                    clean"
	@echo
	@echo " - ATTINY25:"
	@echo "                    build_tiny25"
	@echo "                    build_singlenote_tiny25"
	@echo "                    build_testing_tiny25"
	@echo "                    build_testing_singlenote_tiny25"
	@echo "                    fuse_read_tiny25"
	@echo "                    fuse_write_tiny25"
	@echo "                    burn_tiny25"
	@echo "                    burn_singlenote_tiny25"
	@echo "                    burn_testing_tiny25"
	@echo "                    burn_testing_singlenote_tiny25"
	@echo
	@echo " - ATTINY45:"
	@echo "                    build_tiny45"
	@echo "                    build_singlenote_tiny45"
	@echo "                    build_testing_tiny45"
	@echo "                    build_testing_singlenote_tiny45"
	@echo "                    fuse_read_tiny45"
	@echo "                    fuse_write_tiny45"
	@echo "                    burn_tiny45"
	@echo "                    burn_singlenote_tiny45"
	@echo "                    burn_testing_tiny45"
	@echo "                    burn_testing_singlenote_tiny45"
	@echo	
	@echo " - ATTINY85:"
	@echo "                    build_tiny85"
	@echo "                    build_singlenote_tiny85"
	@echo "                    build_testing_tiny85"
	@echo "                    build_testing_singlenote_tiny85"
	@echo "                    fuse_read_tiny85"
	@echo "                    fuse_write_tiny85"
	@echo "                    burn_tiny85"
	@echo "                    burn_singlenote_tiny85"
	@echo "                    burn_testing_tiny85"
	@echo "                    burn_testing_singlenote_tiny85"
	@echo

OBJECTS = sb_main.o sb_delay.o sb_tascam.o sb_midiusi.o
OBJECTS_TESTING = sb_debug.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

sync_blaster.elf: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^
	$(SIZE) $@
	$(DUMP) -h -S $@ > blaster.lst

sync_blaster_testing.elf: $(OBJECTS) $(OBJECTS_TESTING)
	$(CC) $(LDFLAGS) -o $@ $^
	$(SIZE) $@
	$(DUMP) -h -S $@ > blaster.lst

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $^ $@
	mv $@ $(SB_FW_BURN_FILE)

clean:
	rm -f *.o
	rm -f sync_blaster*.elf
	rm -f sync_blaster*.hex
	rm -f $(SB_FW_BURN_FILE)
	rm -f blaster.lst

_fuse_read:
	@echo "`tput rev; tput setaf 2` *** Reading fuses ...                                      *** `tput sgr0`"
	$(DUDE) -c usbasp -p $(DUDE_TYPE) -B 1024 -U hfuse:r:-:h -U lfuse:r:-:h -U efuse:r:-:h
	@echo "`tput rev; tput setaf 2` *** ... done.                                              *** `tput sgr0`"
	@echo

_fuse_write:
	@echo "`tput rev; tput setaf 1` *** Writing fuses ...                                      *** `tput sgr0`"
	$(DUDE) -c usbasp -p $(DUDE_TYPE) -B 1024 -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m
	@echo "`tput rev; tput setaf 1` *** ... done.                                              *** `tput sgr0`"
	@echo

_burn:
	@echo "`tput rev; tput setaf 1` *** Writing flash mem ...                                  *** `tput sgr0`"
	$(DUDE) -c usbasp -p $(DUDE_TYPE) -B 16 -U flash:w:$(SB_FW_BURN_FILE):i -F
	@echo "`tput rev; tput setaf 1` *** ... done.                                              *** `tput sgr0`"
	@echo

setenv_tiny25:
	@echo "`tput rev; tput setaf 7` *** Setting ATTINY25 environment ...                       *** `tput sgr0`"
	$(eval DUDE_TYPE := t25)
	$(eval GCC_MMCU := attiny25)
	@echo "`tput rev; tput setaf 7` *** ... done.                                              *** `tput sgr0`"
	@echo

setenv_tiny45:
	@echo "`tput rev; tput setaf 7` *** Setting ATTINY45 environment ...                       *** `tput sgr0`"
	$(eval DUDE_TYPE := t45)
	$(eval GCC_MMCU := attiny45)
	@echo "`tput rev; tput setaf 7` *** ... done.                                              *** `tput sgr0`"
	@echo

setenv_tiny85:
	@echo "`tput rev; tput setaf 7` *** Setting ATTINY85 environment ...                       *** `tput sgr0`"
	$(eval DUDE_TYPE := t85)
	$(eval GCC_MMCU := attiny85)
	@echo "`tput rev; tput setaf 7` *** ... done.                                              *** `tput sgr0`"
	@echo

_testing:
	@echo "`tput rev; tput setaf 3` *** ATTENTION: THIS IS IMAGE CONTAINING TEST CODE!         *** `tput sgr0`"
	@echo
	$(eval SB_TESTING := "testing")
	$(eval CFLAGS += -DSB_TESTING=$(SB_TESTING))

testing: _testing $(OBJECTS_TESTING)
	@echo
	@echo "`tput rev; tput setaf 3` *** TEST SUITE CODE READY.                                 *** `tput sgr0`"
	@echo

_single_note_gate_mode:
	@echo
	@echo "`tput rev; tput setaf 5` *** ATTENTION: THIS IS SINGLE NOTE GATE MODE BUILD!        *** `tput sgr0`"
	@echo
	$(eval SB_SINGLE_NOTE_GATE_MODE := "single_note_gate_mode")
	$(eval CFLAGS += -DSB_SINGLE_NOTE_GATE_MODE=$(SB_SINGLE_NOTE_GATE_MODE))

single_note_gate_mode: _single_note_gate_mode

build_tiny25: setenv_tiny25 sync_blaster.hex
build_tiny45: setenv_tiny45 sync_blaster.hex
build_tiny85: setenv_tiny85 sync_blaster.hex

build_singlenote_tiny25: setenv_tiny25 single_note_gate_mode sync_blaster.hex
build_singlenote_tiny45: setenv_tiny45 single_note_gate_mode sync_blaster.hex
build_singlenote_tiny85: setenv_tiny85 single_note_gate_mode sync_blaster.hex

build_testing_tiny25: setenv_tiny25 testing sync_blaster_testing.hex
build_testing_tiny45: setenv_tiny45 testing sync_blaster_testing.hex
build_testing_tiny85: setenv_tiny85 testing sync_blaster_testing.hex

build_testing_singlenote_tiny25: setenv_tiny25 testing single_note_gate_mode sync_blaster_testing.hex
build_testing_singlenote_tiny45: setenv_tiny45 testing single_note_gate_mode sync_blaster_testing.hex
build_testing_singlenote_tiny85: setenv_tiny85 testing single_note_gate_mode sync_blaster_testing.hex

fuse_read_tiny25: setenv_tiny25 _fuse_read
fuse_read_tiny45: setenv_tiny45 _fuse_read
fuse_read_tiny85: setenv_tiny85 _fuse_read

fuse_write_tiny25: setenv_tiny25 _fuse_write
fuse_write_tiny45: setenv_tiny45 _fuse_write
fuse_write_tiny85: setenv_tiny85 _fuse_write

burn_tiny25: setenv_tiny25 sync_blaster.hex _burn
burn_tiny45: setenv_tiny45 sync_blaster.hex _burn
burn_tiny85: setenv_tiny85 sync_blaster.hex _burn

burn_singlenote_tiny25: setenv_tiny25 single_note_gate_mode sync_blaster.hex _burn
burn_singlenote_tiny45: setenv_tiny45 single_note_gate_mode sync_blaster.hex _burn
burn_singlenote_tiny85: setenv_tiny85 single_note_gate_mode sync_blaster.hex _burn

burn_testing_tiny25: setenv_tiny25 testing sync_blaster_testing.hex _burn
burn_testing_tiny45: setenv_tiny45 testing sync_blaster_testing.hex _burn
burn_testing_tiny85: setenv_tiny85 testing sync_blaster_testing.hex _burn

burn_testing_singlenote_tiny25: setenv_tiny25 testing single_note_gate_mode sync_blaster_testing.hex _burn
burn_testing_singlenote_tiny45: setenv_tiny45 testing single_note_gate_mode sync_blaster_testing.hex _burn
burn_testing_singlenote_tiny85: setenv_tiny85 testing single_note_gate_mode sync_blaster_testing.hex _burn
