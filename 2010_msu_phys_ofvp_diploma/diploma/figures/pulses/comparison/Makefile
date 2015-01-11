all: graph pdf
	../../clean_eps_comments.sh .

graph: data.txt graph.gp gnuplot_styles.gp
	cat ./graph.gp | gnuplot
	cat ./graph_slides.gp | gnuplot

pdf: $(patsubst %.eps,%.pdf,$(wildcard *.eps))

%.pdf: %.eps
	epstopdf $<

clean:
	rm -f ./*.eps

