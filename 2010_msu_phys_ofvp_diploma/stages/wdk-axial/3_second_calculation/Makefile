
TEXFILENAME = filament_measurements

all: tex

tex:
	latex ./$(TEXFILENAME).tex
	latex ./$(TEXFILENAME).tex # for references, if needed
	dvips -P pdf ./$(TEXFILENAME).dvi
	ps2pdf ./$(TEXFILENAME).ps

clean:
	rm -f ./*.aux
	rm -f ./*.log
	rm -f ./*.out
	rm -f ./*.dvi
	rm -f ./*.ps
	if [ -e "./$(TEXFILENAME).pdf" ]; then mv ./$(TEXFILENAME).pdf ./$(TEXFILENAME).prev.`date +%Y%m%d.%H%M%S`.pdf; fi;

