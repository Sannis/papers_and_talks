all: images_eps images_pdf

gle: $(patsubst %.gif,%.gle,$(wildcard *.gif))

%.gle: %.gif template.gle
	cat template.gle | sed "s/#FILENAME#/$</" > $(patsubst %.gif,%.gle,$<)

images_eps: $(patsubst %.gif,%.eps,$(wildcard *.gif))
	../../clean_eps_comments.sh .

%.eps: %.gif %.gle
	gle -d eps $(patsubst %.gif,%.gle,$<)

images_pdf: $(patsubst %.gif,%.pdf,$(wildcard *.gif))

%.pdf: %.gif %.gle
	gle -d pdf $(patsubst %.gif,%.gle,$<)

clean:
	rm -f ./*.eps ./*.pdf

