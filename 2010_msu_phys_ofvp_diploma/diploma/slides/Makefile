BIBTEX ?= bibtex
PDFLATEX ?= pdflatex

#BIBTEXFLAGS ?= -H -c utf8
PDFLATEXFLAGS ?= --shell-escape

CLEAN_LOGS ?= *.aux *.toc *.idx *.ind *.ilg *.log *.out *.lof *.lot *.lol \
  *.bbl *.blg *.bak *.dvi *.ps
CLEAN_ALL ?= $(CLEAN_LOGS) *.pdf

all: slides

slides: slides.pdf
	rm -f $(CLEAN_LOGS)

slides.pdf: *.tex *.sty
	$(PDFLATEX) $(PDFLATEXFLAGS) slides
	$(PDFLATEX) $(PDFLATEXFLAGS) slides # for references ;)

clean:
	rm -f $(CLEAN_ALL)
