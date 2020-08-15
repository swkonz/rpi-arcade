APPLICATION = main.bin
MY_MODULES = debounce.o

CFLAGS = -I$(CS107E)/include -g -Wall -Og -std=c99 -ffreestanding
CFLAGS += -mapcs-frame -fno-omit-frame-pointer -mpoke-function-name -Wpointer-arith
LDFLAGS = -nostdlib -T memmap -L$(CS107E)/lib
LDLIBS = -lpi -lgcc -lpiextra

all : $(MY_MODULES) $(APPLICATION) 

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.elf: %.o $(MY_MODULES) start.o cstart.o
	arm-none-eabi-gcc $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	arm-none-eabi-as $(ASFLAGS) $< -o $@

%.list: %.o
	arm-none-eabi-objdump --no-show-raw-insn -d $< > $@

install: $(APPLICATION)
	rpi-install.py -p $<

clean:
	rm -f *.o *.bin *.elf *.list *~
	rm -f apps/*.o apps/*.bin apps/*.elf apps/*.list apps/*~

.PHONY: all clean install

.PRECIOUS: %.elf %.o

# empty recipe used to disable built-in rules for native build
%:%.c
%:%.o

define CS107E_ERROR_MESSAGE
ERROR - CS107E environment variable is not set.

Review instructions for properly configuring your shell.
https://cs107e.github.io/guides/install/userconfig#env

endef

ifndef CS107E
$(error $(CS107E_ERROR_MESSAGE))
endif
