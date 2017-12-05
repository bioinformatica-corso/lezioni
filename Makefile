stampatexs=$(wildcard *-stampa.tex)
videotexs=$(wildcard *-video.tex)
stampapdfs=$(stampatexs:.tex=.pdf)
videopdfs=$(videotexs:.tex=.pdf)
LATEXMK = latexmk -recorder -use-make -interaction=nonstopmode -f

pdf: $(videopdfs)

%-video.pdf : %-video.tex %-testo.tex
	$(LATEXMK) -pdf $<

%-stampa.pdf : %-stampa.tex %-testo.tex
	$(LATEXMK) -pdf $<

clean :
	rm -f ./*.pdf ./figures/*.pdf *.snm *.nav *.vrb && latexmk -c

vc.tex:	.git/logs/HEAD Makefile figs
	./vc


#The Makefile is released under a Creative Commons Attribution license.
#The full text of the license is available here.
#
#http://creativecommons.org/licenses/by/2.5/ca/
#
#Users of this code should attribute the work to the Open Data Structures
#project by displaying a notice stating their product contains code
#and/or text from the Open Data Structures Project and/or linking to
#opendatastructures.org.

sources=$(wildcard figures/*.ipe)
pdfs=$(sources:.ipe=.pdf)
burstpdfs=$(sources:.ipe=-1.pdf)
svgfigs=$(wildcard figures/*.svg)
svgfigspdf=$(svgfigs:.svg=.pdf)

figs: $(pdfs) $(burstpdfs) $(externalfigs) $(svgfigspdf)

%-1.pdf : %.pdf
		pdftk $< burst output $(<:.pdf=-%d.pdf)

%.pdf : %.ipe
		ipetoipe -pdf $<

%.pdf : %.svg
	inkscape $< --export-pdf=$@

release: $(videopdfs) $(stampapdfs)
	cp $(videopdfs) $(stampapdfs) ~/Corsi/elementi-bioinformatica/lezioni;
