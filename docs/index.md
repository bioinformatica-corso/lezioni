---
layout: page
title: Elementi di Bioinformatica (2019-20)
---

## Elenco delle lezioni


| #  | Who | Date  |Topic      | Material   | Assignments   | Solutions | 
| -- | --- | ----- | --------- | ---------- | ------------- | --------- |
| 1 | GDV | 1/10 | Introduction to the Jupyter Notebook, Array, lists, loops, dictionaries. Reading files, functions, and libraries.  | [Notebook 1](https://jakevdp.github.io/PythonDataScienceHandbook/01.00-ipython-beyond-normal-python.html) [Notebook 2](https://github.com/gdv/EngComp/blob/master/modules/1_offtheground/1_Interacting_with_Python.ipynb) [Notebook python basics](https://nbviewer.jupyter.org/github/gdv/foundationsCS/blob/master/py-01-basics.ipynb) [Notebook introduction](https://nbviewer.jupyter.org/github/gdv/foundationsCS/blob/master/py-02-intro-notebook.ipynb) | [Exercise](https://github.com/gdv/foundationsCS/blob/master/ex-01-DictFilesFunc.ipynb), [PromessiSposi.txt](https://github.com/gdv/foundationsCS/blob/master/ex-data/PromessiSposi.txt)  |         [Solution Part 1](https://github.com/gdv/foundationsCS/blob/master/ex-01-DictFilesFunc-solution-full.ipynb)                   |
| -- | --- | ----- | --------- | ---------- | ------------- | --------- |
| 2 | GDV | 2/10 | Separator-based files. Relational model DBMS. Schemas and instances. Exploring a DB. Constraints, Keys, Indices. Null values. | [slides](https://github.com/gdv/introduction_to_relational_databases/raw/master/01-intro.pdf) [planets.csv](https://raw.githubusercontent.com/mwaskom/seaborn-data/master/planets.csv) |  |
| -- | --- | ----- | --------- | ---------- | ------------- | --------- |
| 3 | RR | 4/10 |  Basic shell usage, Version control | See "Linux shell commands" at the end of this page and [version control](http://swcarpentry.github.io/git-novice/) for version control. | 
| -- | --- | ----- | --------- | ---------- | ------------- | --------- |


## Azure Notebooks

[Azure notebooks of the course](https://notebooks.azure.com/gianluca-dellavedova/projects/foundationsCS-2019)

## Install software

You will need to use the command line.


1.  Install [conda](https://conda.io/projects/conda/en/latest/user-guide/install/index.html). Install the Python 3 version.
1.  `git clone https://github.com/gdv/foundationsCS.git`
1.  `cd foundationsCS`
1.  `conda env create -f FoCS.yml`
1.  `conda activate FoCS`


## Linux shell commands

* [ls, cd, pwd](https://asciinema.org/a/197295)
* [mv cp nano touch rsync](https://asciinema.org/a/197355)
* [redirection pipe cut wc sort head tail](https://asciinema.org/a/197358)
* [for history echo](https://asciinema.org/a/197360)
* [bash programs, script, filenames with spaces](https://asciinema.org/a/197363)
* [grep, find](https://asciinema.org/a/197364)
* [fg, bg, ctrl+c, ctrl+z, ps, kill, chmod](https://asciinema.org/a/197590)

# License


<a rel="license" href="http://creativecommons.org/licenses/by/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by/4.0/">Creative Commons Attribution 4.0 International License</a>.
