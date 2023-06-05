#VARS
APPS=mp4trace etmp4 psnr hist mos miv eg vsgen
CC=gcc
C_SOURCE=$(wildcard ./src/*.c)
H_SOURCE=$(wildcard ./src/*.h)
OBJ=$(subst .c,.o,$(subst src,objects,$(C_SOURCE)))
PRF=-pg
CC_FLAGS=-W                              \
         -Wall                           \
		 -Wno-sign-compare               \
		 -Wno-multichar                  \
		 -Wno-pointer-sign               \
		 -Wno-parentheses                \
		 -Wno-missing-field-initializers \
		 -Wno-missing-braces             \
		 -O3                             \
		 -c

INSTDIR=/usr/local/bin

all: objFolder $(APPS)

mp4trace: objects/bits.o objects/error.o objects/lock.o objects/misc.o objects/queue.o objects/socket.o objects/thread.o objects/timing.o objects/mp4trace.o
	@echo 'Building binary using GCC linker: $@'
	$(CC) $^ -o $@ -lpthread -lgpac_static -static -lncurses -ltinfo
	@echo 'Finished building binary: $@'
	@echo ' '

etmp4: objects/bits.o objects/misc.o objects/read.o objects/stat.o objects/writemp4.o objects/etmp4.o
	@echo 'Building binary using GCC linker: $@'
	$(CC) $^ -o $@ -lgpac_static -lm -static -lncurses -ltinfo
	@echo 'Finished building binary: $@'
	@echo ' '
 
psnr: objects/psnr.o
	@echo 'Building binary using GCC linker: $@'
	$(CC) $^ -o $@ -lm
	@echo 'Finished building binary: $@'
	@echo ' '

hist: objects/stat.o objects/hist.o
	@echo 'Building binary using GCC linker: $@'
	$(CC) $^ -o $@
	@echo 'Finished building binary: $@'
	@echo ' '

mos: objects/dir.o objects/mos.o
	@echo 'Building binary using GCC linker: $@'
	$(CC) $^ -o $@
	@echo 'Finished building binary: $@'
	@echo ' '

miv: objects/dir.o objects/miv.o
	@echo 'Building binary using GCC linker: $@'
	$(CC) $^ -o $@
	@echo 'Finished building binary: $@'
	@echo ' '

eg: objects/misc.o objects/random.o objects/read.o objects/eg.o
	@echo 'Building binary using GCC linker: $@'
	$(CC) $^ -o $@
	@echo 'Finished building binary: $@'
	@echo ' '

vsgen: objects/vsgen.o
	@echo 'Building binary using GCC linker: $@'
	$(CC) $^ -o $@ -lm
	@echo 'Finished building binary: $@'
	@echo ' '

./objects/%.o: ./src/%.c
	@echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS) -o $@
	@echo ' '

install: objFolder $(APPS)
	@echo I $(APPS) in $(INSTDIR) ...
	@install -s -m 755 $(APPS) $(INSTDIR)

objFolder:
	@ mkdir -p objects

clean:
	@rm -f $(APPS) gmon.out *.s *.i *~ *.*~ evalvid-2.7.tar.bz2
	@rm -rf objects

docker:
	docker container run -it --rm --name evalvid -v $(shell pwd):/home/evalvid evalvid