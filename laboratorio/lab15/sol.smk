rule run:
    input:
        expand("{sample}.call.vcf",
        sample= ["sample_1", "sample_2"])

rule index_fa:
    input: "ref.fa"
    output: "ref.fa.fai"
    shell:
        '''
        samtools faidx {input}
        '''

rule bwa_index:
    input: 
        fa = "ref.fa",
        fai = "ref.fa.fai",
    output: "ref.fa.bwt"
    shell:
        '''
        bwa index {input.fa}
        '''

rule bwa_mem:
    input:
        ref = "ref.fa",
        bwt = "ref.fa.bwt",
        sample_R1 = "{sample}_R1.fastq",
        sample_R2 = "{sample}_R2.fastq",
    output: "{sample}.aln.sam"
    shell:
        '''
        bwa mem {input.ref} {input.sample_R1} {input.sample_R2} -o {output}
        '''

rule convert_to_bam:
    input: "{sample}.aln.sam"
    output: "{sample}.aln.bam"
    shell:
        '''
        samtools view -b {input} | samtools sort > {output}
        '''

rule index_bam:
    input: "{sample}.aln.bam",
    output: "{sample}.aln.bam.bai"
    shell:
        '''
        samtools index {input}
        '''

rule call:
    input: 
        ref = "ref.fa",
        reffai = "ref.fa.fai",
        bam = "{sample}.aln.bam",
        bai = "{sample}.aln.bam.bai"
    output: "{sample}.call.vcf"
    shell:
        '''
        bcftools mpileup -Ou -f {input.ref} {input.bam} | bcftools call -mv > {output}
        '''

rule clean:
    shell:
        '''
        rm -f ref.fa.*
        rm -f *.aln.sam
        rm -f *.aln.bam
        rm -f *.aln.bam.bai
        rm -f *.call.vcf
        '''