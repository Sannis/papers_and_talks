
all: graph.sh gnuplot_styles.gp clean_eps_comments.sh la lb

la: ./l800/*.dat
	./graph.sh ./l800
	./clean_eps_comments.sh l800

lb: ./l10000/*.dat
	./graph.sh ./l10000
	./clean_eps_comments.sh l10000

