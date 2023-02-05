CC=gcc
SRC=src/$(wildcard *.c)
OBJ=$(SRC:.c=../.o)
PRF=-pg
COP=-W -Wall -Wno-sign-compare -Wno-multichar -Wno-pointer-sign -Wno-parentheses -Wno-missing-field-initializers -Wno-missing-braces -O3
DST=mp4trace etmp4 psnr hist mos miv eg vsgen
INSTDIR=/usr/local/bin

all: $(DST)

mp4trace: src/bits.o src/error.o src/lock.o src/misc.o src/queue.o src/socket.o src/thread.o src/timing.o src/mp4trace.o
	@echo L $@ ...
	@$(CC) $^ -o $@ -lpthread -lgpac_static -static

etmp4: src/bits.o src/misc.o src/read.o src/stat.o src/writemp4.o src/etmp4.o
	@echo L $@ ...
	@$(CC) $^ -o $@ -lgpac_static -lm -static

psnr: src/psnr.o
	@echo L $@ ...
	@$(CC) $^ -o $@ -lm

hist: src/stat.o src/hist.o
	@echo L $@ ...
	@$(CC) $^ -o $@

mos: src/dir.o src/mos.o
	@echo L $@ ...
	@$(CC) $^ -o $@

miv: src/dir.o src/miv.o
	@echo L $@ ...
	@$(CC) $^ -o $@

eg: src/misc.o src/random.o src/read.o src/eg.o
	@echo L $@ ...
	@$(CC) $^ -o $@

vsgen: src/vsgen.o
	@echo L $@ ...
	@$(CC) $^ -o $@ -lm

$(OBJ)/%.o: %.c
	@echo C $< ...
	@$(CC) $(COP) -c $<

install: $(DST)
	@echo I $(DST) in $(INSTDIR) ...
	@install -s -m 755 $(DST) $(INSTDIR)

clean:
	@rm -f $(DST) gmon.out *.s *.i *~ *.*~ evalvid-2.7.tar.bz2
	@rm -f src/*.o

tar:
	@tar cjf evalvid-2.7.tar.bz2 *.h *.c *.vcproj *.sln Makefile

docker:
	docker container run -it --rm --name evalvid --mount type=bind,source=$(shell pwd),dst=/home/evalvid evalvid
