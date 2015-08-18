TARGETS = lucidi_bioinformatica_stampa.pdf lucidi_bioinformatica_video.pdf
LATEXMK = latexmk -recorder -use-make

all : $(TARGETS)
pdf: all

%.pdf : %.tex lucidi_bioinformatica_testo.tex vc.tex code
	$(LATEXMK) -pdf $<

vc.tex:	../.git/logs/HEAD Makefile figs
	./vc

# Below are the code snippets
code/KS/KS-leq.c:	../KS/drittel.cpp
	./cut_lines.sh $< $(@D)


code/samtools/calDepth.c:	../samtools/examples/calDepth.c
	./cut_lines.sh $< $(@D)

code/samtools/samclose.c:	../samtools/sam.c
	./cut_lines.sh $< $(@D)

code/samtools/bam_import_sam_header_parse.c:	../samtools/bam_import.c
	./cut_lines.sh $< $(@D)

code/samtools/sam_import_sam_header_parse2.c:	../samtools/sam_header.c
	./cut_lines.sh $< $(@D)

code/samtools/bam_parse_region.c:	../samtools/bam_aux.c
	./cut_lines.sh $< $(@D)

code/samtools/bgzf-1.c:	../samtools/bgzf.c
	./cut_lines.sh $< $(@D)




code: code/KS/KS-leq.c code/samtools/calDepth.c \
      code/samtools/samclose.c \
code/samtools/bam_import_sam_header_parse.c \
code/samtools/sam_import_sam_header_parse2.c \
code/samtools/bam_parse_region.c \
code/samtools/bgzf-1.c


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

clean :
	rm -f ./*.pdf ./figures/*.pdf && latexmk -c

release: pdf vc.tex
	$(foreach f,$(TARGETS), ~/bin/dropbox_uploader.sh upload $(f) elementi-bioinformatica;)
