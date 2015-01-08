#!/bin/bash

gle -d pdf ./scheme-1-sync.gle
convert ./scheme-1-sync.pdf ./scheme-1-sync.gif

gle -d pdf ./scheme-1-async.gle
convert ./scheme-1-async.pdf ./scheme-1-async.gif

montage scheme-1-sync.gif scheme-1-async.gif -tile 1x2 -geometry 100% scheme-1.gif

gle -d pdf ./scheme-2-sync.gle
convert ./scheme-2-sync.pdf ./scheme-2-sync.gif

gle -d pdf ./scheme-2-async.gle
convert ./scheme-2-async.pdf ./scheme-2-async.gif

montage scheme-2-sync.gif scheme-2-async.gif -tile 1x2 -geometry 100% scheme-2.gif

gle -d pdf ./scheme-3-bad.gle
convert ./scheme-3-bad.pdf ./scheme-3-bad.gif

cp scheme-3-bad.gif scheme-3.gif

gle -d pdf ./scheme-4-bad.gle
convert ./scheme-4-bad.pdf ./scheme-4-bad.gif

gle -d pdf ./scheme-4-good.gle
convert ./scheme-4-good.pdf ./scheme-4-good.gif

montage scheme-4-bad.gif scheme-4-good.gif -tile 1x2 -geometry 100% scheme-4.gif

montage scheme-1.gif scheme-2.gif scheme-3.gif scheme-4.gif -tile 1x4 -geometry 100% schemes.gif
