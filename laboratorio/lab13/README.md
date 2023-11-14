# Align + SNP call

Index the reference

```bash
bwa index ref.fa
```

Align reads

```bash
bwa mem ref.fa reads_R1.fastq reads_R2.fastq -o reads.aln.sam
```

Samtools

```bash
samtools view -b reads.aln.sam | samtools sort > reads.aln.bam
samtools view reads.aln.bam | less
```

Bcftools

```bash
bcftools mpileup -Ou -f chr21.100k.fa reads.aln.bam | bcftools call -mv > snp.call.vcf
```
