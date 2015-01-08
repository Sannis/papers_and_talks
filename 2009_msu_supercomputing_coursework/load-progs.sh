#!/bin/bash

# Make folder for 3d-party programs
mkdir -p ./progs
cd ./progs
rm -f ./*.gz

if [ !-e ./create_2d_func/create_2d_func ]
then
	# Load create_2d_func
	wget -c -t 5 -O ./create_2d_func-v1.0-tar.gz http://github.com/Sannis/create_2d_func/tarball/v1.0
	tar -xvf ./create_2d_func-*-tar.gz
	rm -rf ./create_2d_func
	mv ./Sannis-create_2d_func-* ./create_2d_func

	# Load libsns for create_2d_func
	wget -c -t 5 -O ./libsns-v0.9-tar.gz http://github.com/Sannis/libsns/tarball/v0.9
	tar -xvf ./libsns-*-tar.gz
	rm -rf ./create_2d_func/src/sns
	mv ./Sannis-libsns-* ./create_2d_func/src/sns

	# Make create_2d_func
	cd ./create_2d_func
	make
	cd ..
fi

if [ !-e ./bin2gif/bin2gif ]
then
	# Load bin2gif
	wget -c -t 5 -O ./bin2gif-v0.3-tar.gz http://github.com/Sannis/bin2gif/tarball/v0.3
	tar -xvf ./bin2gif-*-tar.gz
	rm -rf ./bin2gif
	mv ./Sannis-bin2gif-* ./bin2gif

	# Make bin2gif
	cd ./bin2gif
		make bin2gif
		make bin2gif-static
	cd ..
fi

if [ !-e ./gnuplot/src/gnuplot ]
	# Load and make gnuplot
	wget -c -t 5 -O gnuplot-4.4.0.tar.gz http://sourceforge.net/projects/gnuplot/files/gnuplot/4.4.0/gnuplot-4.4.0.tar.gz/download
	tar -xvf ./gnuplot-*.tar.gz
	rm -rf ./gnuplot
	mv ./gnuplot-* ./gnuplot
	cd ./gnuplot
	./configure
	make
	cd ..
fi

# Go back to project root
cd ..

