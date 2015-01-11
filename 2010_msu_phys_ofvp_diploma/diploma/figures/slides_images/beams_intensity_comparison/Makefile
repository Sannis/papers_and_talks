all: eps pdf

eps: inphase.txt outphase.txt graph.gp gnuplot_styles.gp
	cat ./graph.gp | gnuplot
	../../clean_eps_comments.sh .

pdf: graph_all.pdf graph_start.pdf

%.pdf: %.eps
	epstopdf $<

clean:
	rm -f ./*.eps ./*.pdf

