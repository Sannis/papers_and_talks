
all: paper_num_meth.pdf

paper_num_meth.pdf: paper_num_meth.tex mystyle.sty 0_common.tex 1_explicit_rk4.tex 2_fourier_fftw.tex 3_sweep.tex 4_results.tex 5_conclusions.tex
	#latex ./paper_num_meth.tex
	#latex ./paper_num_meth.tex # for references ;)
	#dvips -P pdf ./paper_num_meth.dvi
	#ps2pdf ./paper_num_meth.ps
	pdflatex --shell-escape ./paper_num_meth.tex
	pdflatex --shell-escape ./paper_num_meth.tex # for references ;)

clean:
	rm -f ./*.aux
	rm -f ./*.log
	rm -f ./*.out
	rm -f ./*.dvi
	rm -f ./*.ps
