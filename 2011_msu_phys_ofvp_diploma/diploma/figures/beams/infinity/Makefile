all: graph_pdf images_pdf

graph_pdf: graph.eps
	epstopdf $<

graph.eps: outphase.txt inphase.txt graph.gp gnuplot_styles.gp
	cat ./graph.gp | gnuplot
	../../clean_eps_comments.sh .

gle: $(patsubst %.gif,%.gle,$(wildcard *.gif))

%.gle: %.gif template.gle
	cat template.gle | sed "s/#FILENAME#/$</" > $(patsubst %.gif,%.gle,$<)

images_eps: $(patsubst %.gif,%.eps,$(wildcard *.gif))
	../../clean_eps_comments.sh .

%.eps: %.gif %.gle
	gle -d eps $(patsubst %.gif,%.gle,$<)

images_pdf: images_eps $(patsubst %.gif,%.pdf,$(wildcard *.gif))

%.pdf: %.gif %.gle
	gle -d pdf $(patsubst %.gif,%.gle,$<)

pdf: graph_all.pdf graph_start.pdf

clean:
	rm -f ./*.eps ./*.pdf

