# Esercizio

Creare una pipeline usando snakemake che utilizzi il file `truth.csv` per valutare l'accuratezza dei classificatori `classifier_[123].py`. 

Il file `truth.csv` contiene al suo interno coppie `regione,classificazione` reppresentante una regione genomica e la sua classificazione `B (benigna) / P (patogenica)`, considerate questo file come la ground truth per confrontare i vari classificatori, prestando attenzione a:

- Input/Output di ogni classificatore. Per effettuare un confronto è necessario fare conversioni di formati sia per l'input che per l'output dei vari metodi.
- Ogni classificatore accetta un parametro iniziale (seed) per fare la classificazione. Impostare una pipeline che testi il comportamento dei tool per i seguenti valori iniziali `[0, 5, 10]`.
(nota: ogni metodo utilizza un nome diverso per tale argomento).
- Utilizzare la parola chiave `benchmark` di snakemake per valutare anche i tempi di esecuzione dei vari tool.
- Fornire come output finale della pipeline un file CSV come segue:

```csv
tool,init,precision,recall,time,memory
classifier_1,0,60%,72%,2s,1MB
classifier_2,5,50%,90%,7s,8MB
...
```

# Classifiers READMEs

### Classifier 1

Help:
```
usage: classifier_1.py [-h] --ref REF -i INPUT [--seed SEED] -o OUTPUT

Argument Parser Example

optional arguments:
  -h, --help            show this help message and exit
  --ref REF             Reference
  -i INPUT, --input INPUT
                        Input
  --seed SEED           An integer seed value
  -o OUTPUT, --output OUTPUT
                        Output file path
```

Input example:
```txt
chr10:1819916-1836627
chr16:2227361-2239388
chr22:6451132-6456071
```

Output example:
```txt
chr10:1819916-1836627,P
chr16:2227361-2239388,B
chr22:6451132-6456071,B
chr15:1711192-1733668,B
chr14:8037790-8046729,B

```

### Classifier 2

Help:
```
usage: classifier_2.py [-h] --fa FA --csvin CSVIN [--start START]

Argument Parser Example

optional arguments:
  -h, --help     show this help message and exit
  --fa FA        FASTA
  --csvin CSVIN  Input
  --start START  An integer start value
```

Input example:
```csv
chr10,1819916,1836627
chr16,2227361,2239388
chr22,6451132,6456071
chr15,1711192,1733668
chr14,8037790,8046729
```

Output example:
```csv
chr,start,end,classification
chr10,1819916,1836627,pathogenic
chr16,2227361,2239388,pathogenic
chr22,6451132,6456071,pathogenic
chr15,1711192,1733668,benign
chr14,8037790,8046729,pathogenic
```

### Classifier 3

Help:
```
usage: classifier_3.py [-h] -r REFFA --filein FILEIN [-s S] [--out OUT] [--ignore-cov]

Argument Parser Example

optional arguments:
  -h, --help            show this help message and exit
  -r REFFA, --reffa REFFA
                        FASTA
  --filein FILEIN       Input
  -s S                  An integer seed value
  --out OUT             Output file. Use - for stdout (default -).
  --ignore-cov          Ignore coverage value (default False).
```

Input example:
```csv
region,cov
chr10:1819916-1836627,0
chr16:2227361-2239388,0
chr22:6451132-6456071,0
chr15:1711192-1733668,0
```

Output example:
```csv
region,cov,prediction
chr10:1819916-1836627,0,p
chr16:2227361-2239388,0,p
chr22:6451132-6456071,0,p
chr15:1711192-1733668,0,b
```
