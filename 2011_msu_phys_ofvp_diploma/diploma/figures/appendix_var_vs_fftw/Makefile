
all:
	# Create directories for graphs
	mkdir -p ./skif/

	# Run graph scripts
	echo "Running graph_fftw.sh:"
	./graph_fftw.sh
	echo "Running graph_var.sh:"
	./graph_var.sh

	./clean_eps_comments.sh skif
