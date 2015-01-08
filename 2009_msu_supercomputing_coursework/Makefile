


all: load-progs create-impulses make-src


load-progs:
	./load-progs.sh

create-impulses:
	./create-impulses.sh

make-src:
	for dir in ./src/*; do \
		(cd $$dir && make); \
	done;

