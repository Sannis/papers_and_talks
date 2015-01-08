
BIBTEX ?= bibtex
PDFLATEX ?= pdflatex

#BIBTEXFLAGS ?= -H -c utf8
PDFLATEXFLAGS ?= --shell-escape

CLEAN_LOGS ?= *.aux *.toc *.idx *.ind *.ilg *.log *.out *.lof *.lot *.lol \
  *.blg *.bak *.dvi *.ps
CLEAN_ALL ?= $(CLEAN_LOGS) *.bbl *.pdf

all: abstracts diploma

diploma: diploma.pdf
	rm -f $(CLEAN_LOGS)
	(cd tex/; rm -f $(CLEAN_LOGS))

diploma.bbl: diploma.bib
	$(PDFLATEX) $(PDFLATEXFLAGS) diploma
	$(BIBTEX) $(BIBTEXFLAGS) diploma

diploma.pdf: diploma.tex tex/*.tex diploma.bbl diploma.sty
	$(PDFLATEX) $(PDFLATEXFLAGS) diploma
	$(PDFLATEX) $(PDFLATEXFLAGS) diploma

abstracts: abstract_ru.pdf abstract_en.pdf
	rm -f $(CLEAN_LOGS)
	(cd tex/; rm -f $(CLEAN_LOGS))

abstract_ru.pdf: abstract_ru.tex diploma.sty
	$(PDFLATEX) $(PDFLATEXFLAGS) abstract_ru

abstract_en.pdf: abstract_en.tex diploma.sty
	$(PDFLATEX) $(PDFLATEXFLAGS) abstract_en

clean:
	rm -f $(CLEAN_ALL)
	(cd tex/; rm -f $(CLEAN_LOGS))

