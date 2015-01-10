
all: presentation.pdf

presentation.pdf: ./presentation.tex ./ofvp_beamer_theme.sty ./ofvp_beamer_packages.sty ./rk4_step_parallel.c ./rk4_step.c
	pdflatex --shell-escape ./presentation.tex
	pdflatex --shell-escape ./presentation.tex # for references ;)

clean:
	rm -f ./*.aux
	rm -f ./*.log
	rm -f ./*.out
