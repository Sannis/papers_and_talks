all: la lb disp vs
	(cd ./comparison/; make)
	(cd ./l800/; make)
	(cd ./l800_images/; make)
	(cd ./l10000/; make)
	(cd ./no/; make)
	(cd ./yes/; make)
	(cd ./noyes/; make)
	(cd ./vs/; make)

%.eps: %.gif
	convert $< $(patsubst %.gif,%.ps,$<)
	ps2eps -f -q --fixps --removepreview $(patsubst %.gif,%.ps,$<)
	rm -f $(patsubst %.gif,%.ps,$<)

la_images:
	./images2avi.sh l800

la:
	./graph.sh ./l800
	../clean_eps_comments.sh l800

lb:
	./graph.sh ./l10000
	../clean_eps_comments.sh l10000

no:
	./graph.sh ./no
	../clean_eps_comments.sh no

yes:
	./graph.sh ./yes
	../clean_eps_comments.sh yes

disp: no yes
	./graph2.sh ./yes/log.txt ./no/log.txt
	mv ./*.eps ./noyes
	../clean_eps_comments.sh noyes

vs: la lb
	./graph3.sh ./l800/log.txt ./l10000/log.txt
	mv ./*.eps ./vs
	../clean_eps_comments.sh vs

clean:
	rm -f ./comparison/*.eps
	rm -f ./l800/*.eps
	rm -f ./l800_images/*.eps
	rm -f ./l10000/*.eps
	rm -f ./no/*.eps
	rm -f ./yes/*.eps
	rm -f ./noyes/*.eps

.PHONY: la lb no yes noyes

