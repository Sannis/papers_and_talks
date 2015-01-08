all: pdf

eps: log.txt graph.gp gnuplot_styles.gp
	cat ./graph.gp | gnuplot
	../../clean_eps_comments.sh .

pdf: eps $(patsubst %.eps,%.pdf,$(wildcard *.eps))

%.pdf: %.eps
	epstopdf $<

clean:
	rm -f ./*.eps

