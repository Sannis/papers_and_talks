
all: mission.pdf

mission.pdf: mission.tex mystyle.sty 0_common.tex 1_explicit_runge_kutta_4.tex 2_fourier_fftw2.tex 3_sweep.tex 4_detailed_description.tex
	pdflatex ./mission.tex
	pdflatex ./mission.tex # for better references ;)

clean:
	rm -f ./*.aux
	rm -f ./*.log
	rm -f ./*.out
	if [ -e "./mission.pdf" ]; then mv ./mission.pdf ./mission.prev.`date +%Y%m%d.%H%M%S`.pdf; fi;

