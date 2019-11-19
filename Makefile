#The Makefile is released under a Creative Commons Attribution license.
#The full text of the license is available here.
#
#http://creativecommons.org/licenses/by/2.5/ca/
#
#Users of this code should attribute the work to the Open Data Structures
#project by displaying a notice stating their product contains code
#and/or text from the Open Data Structures Project and/or linking to
#opendatastructures.org.
texs=$(wildcard *-*.tex)
OBJECTS = $(texs:.tex=.pdf)
LATEXMK = latexmk -recorder -use-make -pdf -interaction=nonstopmode

pdf: $(OBJECTS)

%.pdf : %.tex
	$(LATEXMK) -pdf $<

clean:
	rm -f ./*.pdf ./figures/*.pdf *.snm *.nav *.vrb && latexmk -c


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
