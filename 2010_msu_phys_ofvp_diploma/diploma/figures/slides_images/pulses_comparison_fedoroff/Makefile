all: pdf

pdf: $(patsubst %.gle,%.pdf,$(wildcard *.gle))

%.pdf: %.gle
	gle -d pdf $<

clean:
	rm -f ./*.pdf

