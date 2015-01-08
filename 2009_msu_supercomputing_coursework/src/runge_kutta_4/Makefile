###
# Variables
###

PNAME = wdk-runge-kutta-4.e
CXX = mpicxx
#CFLAGS = -openmp
#CFLAGS += -Wall
#CFLAGS += -Werror


###
# Targets
###

all: full_compile

full_compile: main.o runge_kutta_4.o sns_mpi.o sns_utility.o
	$(CXX) ./*.o -o ./$(PNAME) -limf $(LIBS) $(CFLAGS)

main.o: main.cpp
	$(CXX) -c ./main.cpp $(INCLUDES) $(CFLAGS)

runge_kutta_4.o: runge_kutta_4.cpp runge_kutta_4.h
	$(CXX) -c ./runge_kutta_4.cpp $(INCLUDES) $(CFLAGS)

sns_mpi.o: sns_mpi.cpp sns_mpi.h
	$(CXX) -c ./sns_mpi.cpp $(INCLUDES) $(CFLAGS)

sns_utility.o: sns_utility.cpp sns_utility.h
	$(CXX) -c ./sns_utility.cpp $(INCLUDES) $(CFLAGS)

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
