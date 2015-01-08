
all:
	# Create directories for graphs
	mkdir -p ./Bluegene/
	mkdir -p ./Skif/

	# Run graph scripts
	echo "Running graph_fftw.sh:"
	./graph_fftw.sh
	echo "Running graph_sweep.sh:"
	./graph_sweep.sh

	./clean_eps_comments.sh Bluegene
	./clean_eps_comments.sh Skif
