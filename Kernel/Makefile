include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(wildcard *.c)
SOURCES_INTERRUPTIONS=$(wildcard interruptions/*.c)
SOURCES_INTERRUPTIONS_ASM=$(wildcard interruptions/*.asm)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_FUNCTIONS=$(wildcard functions/*.c)
SOURCES_MEMORYMANAGER=$(wildcard MemoryManager/*.c)
SOURCES_SCHEDULER=$(wildcard scheduler/*.c)

OBJECTS=$(SOURCES:.c=.o)
OBJECTS_INTERRUPTIONS=$(SOURCES_INTERRUPTIONS:.c=.o)
OBJECTS_INTERRUPTIONS_ASM=$(SOURCES_INTERRUPTIONS_ASM:.asm=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_FUNCTIONS=$(SOURCES_FUNCTIONS:.c=.o)
OBJECTS_MEMORYMANAGER=$(SOURCES_MEMORYMANAGER:.c=.o)
OBJECTS_SCHEDULER=$(SOURCES_SCHEDULER:.c=.o)

LOADERSRC=loader.asm
buddy: BUDDYFLAG := -D BUDDY_MM


LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

ALL_OBJECTS= $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_INTERRUPTIONS) $(OBJECTS_INTERRUPTIONS_ASM) $(OBJECTS_FUNCTIONS) $(OBJECTS_MEMORYMANAGER) $(OBJECTS_SCHEDULER)

all: $(KERNEL)
buddy: all

$(KERNEL): $(STATICLIBS) $(ALL_OBJECTS)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(ALL_OBJECTS)
	$(LD) $(LDFLAGS) -T kernel.ld -o kernel.elf --oformat=elf64-x86-64 $(ALL_OBJECTS)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -I./interruptions -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf $(ALL_OBJECTS) *.bin

.PHONY: all clean
