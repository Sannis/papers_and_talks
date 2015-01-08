
all: report.pdf

report.pdf: report.tex mystyle.sty 0_common.tex 1_explicit_rk4.tex 2_fourier_fftw.tex 3_sweep.tex 4_results.tex 5_conclusions.tex 6_thanks.tex
	pdflatex ./report.tex
	pdflatex ./report.tex # for references ;)

clean:
	rm -f ./*.aux
	rm -f ./*.log
	rm -f ./*.out
	if [ -e "./report.pdf" ]; then mv ./report.pdf ./report.prev.`date +%Y%m%d.%H%M%S`.pdf; fi;

