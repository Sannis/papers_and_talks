
all: images tex

images: ./images/sweep_method_forward.svg ./images/sweep_method_backward.svg
	inkscape -z -D -E ./images/sweep_method_forward.eps ./images/sweep_method_forward.svg
	inkscape -z -D -E ./images/sweep_method_backward.eps ./images/sweep_method_backward.svg

	../graphs_grayscale/clean_eps_comments.sh images

tex:
	latex ./paper_num_meth.tex
	latex ./paper_num_meth.tex # for references
	dvips -P pdf ./paper_num_meth.dvi
	ps2pdf ./paper_num_meth.ps

clean:
	rm -f ./*.aux
	rm -f ./*.log
	rm -f ./*.out
	rm -f ./*.dvi
	rm -f ./*.ps
	if [ -e "./paper_num_meth.pdf" ]; then mv ./paper_num_meth.pdf ./paper_num_meth.prev.`date +%Y%m%d.%H%M%S`.pdf; fi;

