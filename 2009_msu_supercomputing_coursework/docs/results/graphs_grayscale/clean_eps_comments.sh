#!/bin/bash

dir=$1

echo "Strip comments in ./$1/*.eps"

# Create tmp dir
mkdir -p ./$1/tmp

find ./$1 -type f | sed "s/.\/$1\/\(.*\)/cat .\/$1\/\\1 | grep -v \"^% \" | grep -v \"^%	\" | grep -v \"^%\/\" | grep -v \"^%$\" | \
                                                            grep -v \"%Begin\" | grep -v \"%End\" | \
                                                            grep -v \"Creator\" | grep -v \"CreationDate\" | grep -v \"\/Author\" | \
                                                            grep -v \"^%%Title\" | grep -v \"^%%DocumentFonts\" | grep -v \"^%%Trailer\" \
                                                            > .\/$1\/tmp\/\\1/" | bash

rm ./$1/*.eps

mv ./$1/tmp/*.eps ./$1/

rm -rf ./$1/tmp

echo "[Finished]"

