###
# Variables
###

PNAME = wdk-fftw.e
MPICXX = mpicxx

#CFLAGS += -Wall

INCLUDE_DIRS =

LIBS = -lfftw_mpi -lfftw -limf

# Host specific variables
HOSTNAME = $(shell hostname)

ifeq ($(HOSTNAME),ktg1.phys.msu.ru)
	HOSTTITLE = ILC MSU cluster
	LIB_DIRS += -L/opt/fftw2/icc/lib
else
	ifeq ($(HOSTNAME),oleg-pc)
		HOSTTITLE = Oleg home PC
		LIB_DIRS += -L/opt/fftw2/icc/lib
	else
		HOSTTITLE = SKIF MSU cluster
		INCLUDE_DIRS = -I/home/$(USER)/local/include
		LIB_DIRS += -L/home/$(USER)/local/lib
	endif
endif

# Information messages

MSG_COMPILE = Compile on $(HOSTTITLE), $(USER)@$(HOSTNAME)
MSG_INSTALL = Install on $(HOSTTITLE), $(USER)@$(HOSTNAME)

###
# Targets
###

all: compile link

link: beam.o
	$(MPICXX) ./beam.o -o ./$(PNAME) $(LIB_DIRS) $(LIBS) $(CFLAGS)

compile: beam.cpp
	$(MPICXX) -c ./beam.cpp -o ./beam.o $(INCLUDE_DIRS) $(LIBS) $(CFLAGS)

clean_o:
	rm -f ./*.o

clean_pbs:
	rm -f ./*.rep-*
	rm -f ./*.out-*
	rm -f ./machinefile-*
	rm -f ./.cleo-*
	rm -f ./.panfs.*

clean: clean_o clean_pbs
	rm -f ./$(PNAME)
	rm -f ./*~
