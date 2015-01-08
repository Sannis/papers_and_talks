MPICXX = mpicxx
MPIRUN = mpirun


ifeq ($(HOSTNAME),t60-2.parallel.ru)
	QUEUE = -q test
	TASKS = tasks $(QUEUE)
else
	QUEUE = 
	TASKS = Echo no tasks.
endif


all: test_io.e

test_io.e: test_io.cpp sns_util_fs.cpp sns_util_fs.h
	$(MPICXX) -o ./test_io.e -limf ./test_io.cpp ./sns_util_fs.cpp

run1: test_io.e
	mpdboot --file=~/mpd.hosts
	$(MPIRUN) -np 12 $(QUEUE) ./test_io.e -n 192 --npx=4 --npy=3
	mpdallexit

test_192.bin: test_io.e
	# FIXME
	sudo dd if=/dev/sda of=./test_192.bin bs=294912 count=1 || sudo dd if=/dev/hda of=./test_192.bin bs=294912 count=1

run2: test_io.e test_192.bin
	mpdboot --file=~/mpd.hosts
	$(MPIRUN) -np 12 $(QUEUE) ./test_io.e -n 192 --npx=4 --npy=3 --input=./test_192.bin
	mpdallexit

diff:
	diff ./test_192.bin ./test_192_4x3.bin

tasks:
	$(TASKS)

t: tasks

gif:
	bin2gif ./*.bin

g: gif

clean:
	rm -f ./*.e
	rm -f ./*.o
	rm -f ./test_192_*.bin
	rm -f ./*~
