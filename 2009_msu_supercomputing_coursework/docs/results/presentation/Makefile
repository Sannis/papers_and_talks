
all: presentation.pdf

presentation.pdf: ./presentation.tex ./ofvp_beamer_theme.sty ./ofvp_beamer_packages.sty ./rk4_step_parallel.c ./rk4_step.c ../graphs/Skif/md5sum ../graphs/Bluegene/md5sum
	pdflatex ./presentation.tex
	pdflatex ./presentation.tex
	
clean:
	rm -f ./*.aux
	rm -f ./*.log
	rm -f ./*.out
	mv ./presentation.pdf ./presentation.prev.`date +%Y%m%d.%H%M%S`.pdf

