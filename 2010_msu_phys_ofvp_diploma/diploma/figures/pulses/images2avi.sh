#!/bin/bash

# Options
convert_options_with_crop="-gravity center -crop 128x128+0+0 -rotate 90"

#============
# Make video
#============

mkdir -p $1/all_norm_e/crop

for f in $1/all_norm_e/*.gif
do
    convert $f $convert_options_with_crop `dirname $f`/crop/`basename $f gif`gif
done


