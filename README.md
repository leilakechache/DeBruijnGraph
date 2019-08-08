This is a stripped-down version of Gossamer.

It includes only one functionality:
* build the de Bruijn graph from reads
* print contigs

The purpose of this repository is to provide a smaller codebase of Gossamer for researchers who are only interested in building de Bruijn graphs, and not a complete assembler codebase.

# Gossamer bioinformatics suite

## Prerequisites

This has been built on a fresh install of Ubuntu 16 using only the following
dependencies:

```bash
sudo apt-get install \
        g++ cmake libboost-all-dev pandoc \
	zlib1g-dev libbz2-dev libsqlite3-dev
```
It should run on any recent standard 64 bit Linux environment, with as little as 
2 GB of free RAM.

For best performance, we recommend using a machine with 16-32 GB of RAM.

Gossamer does not build on Ubuntu 14 because the default version of Boost was built
with the incorrect ABI.


## Building

Need to rewrite these, but here's the basics:

```bash
mkdir build
cd build
cmake ..
make
make test
make install
```


## Goss Documentation

 * [goss](docs/goss.md)
 
% goss(1) Gossamer User Manual
% Bryan Beresford-Smith, Andrew Bromage, Thomas Conway, Jeremy Wazny, Justin Zobel
% March 28, 2012

NAME
====
goss - a tool for *de novo* assembly of high throughput sequencing data.

Version 1.3.0

SYNOPSIS
========

goss build-graph -k 27 -i in.fastq -O graph

goss print-contigs -G graph-pruned > paths.fa

DESCRIPTION
===========

Input files are base-space reads in FASTA or FASTQ format or in a format
with one read per line and in either
plain text or compressed format (i.e. gzip).


- Build the complete *de Bruijn* graph for some k-mer size using all of
  the read data

    - If the data consists of several files e.g. from different lanes
      of sequencing then it is recommended that *goss build-graph* be run
      on each file separately and the resulting graphs merged using
      *goss merge-graphs*.


    goss build-graph -k 27 -i in1.fastq -i in2.fastq.gz -O graph1

    goss build-graph -k 27 -i in3.fastq -I in4.fasta    -O graph2


- Output the contigs to a FASTA file.
  
    - At this stage of the assembly, all of the read data has been incorporated into
      a *de Bruijn* graph and that graph has been substantially cleaned of features
      arising from read errors.
      It is now possible to print all of the unbranched paths in the graph 
      even though further steps in the assembly process will most likely substantially
      improve the overall lengths of the contigs.
      In fact, at any point in the graph building, e.g. after pruning, these contigs
      can be printed.


    goss print-contigs -G graph2 -o paths.fa



# OPTIONS COMMON TO ALL COMMANDS

The following options can be used with all of the *goss* commands and are therefore
not listed separately for each command.

-h,  \--help
:   Show a help message.

-l *FILE*, \--log-file *FILE*
:   Place to write progress messages. Messages are only written if the -v flag is used. 
    If omitted, messages are written to stderr.

-T *INT*, \--num-threads *INT*
:   The maximum number of *worker* threads to use. The actual number of threads
    used during the algorithms depends on each implementation. *goss* may use a small number
    of additional threads for performing non cpu-bound operations, such as file I/O.

\--tmp-dir *DIRECTORY*
:    A directory to use for temporary files.
     This flag may be repeated in order to nominate multiple temporary directories.

-v, \--verbose
:    Show progress messages.

-V, \--version
:    Show the software version.

-D  *arg*, \--debug *arg*
:   Enable particular debugging output. 


# COMMANDS AND OPTIONS

## goss build-graph 

goss build-graph [-B *INT*] [-S *INT*] -k *INT* {-I *FASTA-filename* |  -i *FASTQ-filename* | --line-in *filename*}+ -O *PREFIX* 

Build the *de Bruijn* graph from the reads contained in the given FASTA
and FASTQ files and output the resulting graph object as a set of files
with the given *PREFIX*.  Both FASTA and FASTQ input files are
supported, with options *-I* and *-i* respectively.  
In addition, files with one read per line are also supported.
The input files
can be compressed with the compression method implied by the file suffix
as follows:

- *.gz* Compressed using gzip.

For large projects (such as sequencing the human genome with high 
coverage) several separate *goss build-graph* commands may be done on
different files in parallel (e.g. on files from different lanes of an
Illumina GAII), followed by a sequence of merging steps using *goss
merge-graphs*. Even in the absence of a cluster, building parts and
merging is usually faster than building a large graph with all the input
files in one go. Future releases may do this automatically.


*OPTIONS*

-B *INT*, \--buffer-size *INT* 
:    Maximum buffer-size for in-memory buffers is *INT* Gigabytes
     (defaults to 2). 
     This is a convenient way of setting the maximum amount of 
     RAM which will be used by build-graph and  
     allows build-graph to be run on machines with a
     small amount of memory.  
     For large data sets, however, larger values
     for *B* will improve performance.  For example, for a data set with
     a large number (e.g. 70 million) of reads of length 100, a typical
     value of B would be 24 (i.e. use 24 Gb buffers).  The actual optimal
     value for the buffer-size is related to the final graph size.
     A typical value for B would be the amount of machine RAM, 
     in Gigabytes.
     Note: In the current release this must be less than 88.

-I *FILE*, \--fasta-in *FILE*
:    Input file in FASTA format.

-i *FILE*, \--fastq-in *FILE*
:    Input file in FASTQ format.

--line-in *FILE*
:    Input file with one read per line and no other annotation.

-O *PREFIX*, \--graph-out *PREFIX*
:    Use *PREFIX* as the prefix name of the output graph object. The
     *PREFIX* must be a valid file name prefix.

-k *INT*, \--kmer-size *INT*
:    The k-mer size to use for building the graph: in version 0.3.0 this
     *must be an integer strictly less than 63*.


## goss print-contigs

goss print-contigs -G *PREFIX* [--min-coverage *INT*] [--min-length *INT*] [-o *FILE*] [--no-sequence] [--verbose-headers] [--no-line-breaks] [--include-entailed-contigs] [--print-rcs]

Print all of the non-branching paths in a given graph. By default, contigs will be printed from 
the supergraph if it is present, otherwise the *de Bruijn* graph will be used.
The paths are printed in FASTA format with a uniquely identify integer in each contig's 
header/descriptor line.
If the --verbose-headers flag is supplied, additional information about contigs will be added
to each header. The make up of a 'verbose' header depends on whether the underlying graph is a *de Bruijn* 
graph or a supergraph.
In the case of a *de Bruijn* graph, the header line contains the following additional information
(interspersed with colons ':'), which is separated from the contig id number by a space:

- Path Length
- Minimum Path Coverage
- Maximum Path Coverage
- Average Path Coverage
- Standard Deviation of Path Coverage

The coverage of a rho-mer is defined to be the number of times it occurs in all reads.
The minimum path coverage is the minimum coverage of all of the rho-mers in the path.

For contigs generated from a supergraph, the descriptor line is made up of the following fields
(separated by commas ','):

- Path Length
- List of Segment Lengths (separated by colons ':')
- List of Segment Starts (separated by colons ':')
- The Integer Identifier of the Reverse Complement
- List of Integer Identifiers of Successor Segments in the Supergraph (separated by colons ':')
- Minimum Path Coverage
- Maximum Path Coverage
- Average Path Coverage
- Standard Deviation of Path Coverage

*OPTIONS*

-G *PREFIX*, \--graph-in *PREFIX*
:    The name of the graph object. This is the string used as the prefix
     for the names of the files making up a graph object.

--min-coverage *INT*
:    Only print those paths which have a minimum rho-mer coverage  >= C.
     This flag is only used when printing contigs from a *de Bruijn* graph.
     Defaults to 0.

\--min-length *INT*
:    Only print those paths which have a length >= the specified minimum
     length. Defaults to 0.

-o *FILE*, \--output-file *FILE*
:    The name of the FASTA output file for the printed paths. Use '-'
     to output to standard output. The *FILE* must be a valid file name.
     Defaults to standard output.

\--no-sequence
:    Suppresses the printing of the actual contig sequences. Instead, only
     the information present in the (verbose) descriptor lines will be printed in a
     tab separated format. The default is to print sequences.

\--print-linear-segments
:    Only print linear segments, ignoring the supergraph if it is present.
     The default is to use the supergraph if available.

\--verbose-headers
:    Print extra information about each contig in its header/descriptor line
     (in addition to its uniquely identifying integer.)

\--no-line-breaks
:    Print each contig on a single line, instead of breaking at 60 columns.

\--include-entailed-contigs
:    By default, only supergraph contigs which are not completely contained within
     other contigs are printed. Supplying this flag results in all supergraph contigs
     being printed, regardless of whether they have been used in the construction of
     longer contigs. Note: Most long contigs, when combined into longer contigs, 
     will be removed from the supergraph anyway, leaving only relatively short 
     entailed contigs (< ~50 bases).

\--print-rcs
:   Include each contig's reverse complement in the output.


--

# LIMITATIONS

Version 1.3.0 is a prototype assembler. Only the steps described above
are supported. It has been tested on very large data sets of Illumina reads
for human genome assembly.

In summary, this version can be used to:

- Build the *de Bruijn* graph from the specified input reads for k-mer
  size k < 63. 

The following limitations apply:

- SOLiD reads in colour space are not supported in this release.
- Bzip-compressed input is not supported in this release due to a bug in an external library.

# FUTURE RELEASES

Bzip support will be re-introduced. 

A future release will also support colour space reads.

# Publication

Please reference the original gossamer paper:

Thomas C Conway, Andrew J Bromage, "Succinct data structures for assembling large genomes", Bioinformatics, 2011 vol. 27 (4) pp. 479-86
[GossamerPaper]

[GossamerPaper]: <http://bioinformatics.oxfordjournals.org/content/27/4/479.abstract> 


