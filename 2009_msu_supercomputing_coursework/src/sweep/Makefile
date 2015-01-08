###
# Variables
###

PNAME = wdk-sweep.e
MPICXX = mpicxx

LIBS = -limf

###
# Targets
###

all: compile link

link: focus.o
	$(MPICXX) ./focus.o -o ./$(PNAME) $(LIB_DIRS) $(LIBS) $(CFLAGS)

compile: focus.cpp output.cpp initialization.cpp finalization.cpp calculation.cpp propagation.cpp
	$(MPICXX) -c focus.cpp -o ./focus.o $(INCLUDE_DIRS) $(CFLAGS)

clean_o:
	rm -f ./*.o

clean_pbs:
	rm -f ./*.rep-*
	rm -f ./*.out-*
	rm -f ./machinefile-*
	rm -f ./.cleo-*
	rm -f ./.panfs.*

clean_cpl:
	find ./ | grep .cpl | xargs -- rm

clean: clean_o clean_pbs
	rm -f ./$(PNAME)
	rm -f ./*~

