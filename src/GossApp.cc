// Copyright (c) 2008-1016, NICTA (National ICT Australia).
// Copyright (c) 2016, Commonwealth Scientific and Industrial Research
// Organisation (CSIRO) ABN 41 687 119 230.
//
// Licensed under the CSIRO Open Source Software License Agreement;
// you may not use this file except in compliance with the License.
// Please see the file LICENSE, included with this distribution.
//

#include "GossApp.hh"

#include "Utils.hh"
#include "GossamerException.hh"
#include "GossCmdReg.hh"
#include "GossOption.hh"

#include <iostream>
#include <stdexcept>
#include <stdint.h>
#include <stdlib.h>


#include "GossCmdBuildGraph.hh"
#include "GossCmdPrintContigs.hh"


using namespace boost;
using namespace boost::program_options;
using namespace std;
using namespace Gossamer;

typedef vector<string> strings;

namespace // anonymous
{
    GossOptions globalOpts;

    GossOptions commonOpts;

    vector<GossCmdReg> cmds;

} // namespace anonymous

const char*
GossApp::name() const
{
    return "goss";
}
#undef ONLY_RELEASED_COMMANDS

GossApp::GossApp()
    : App(globalOpts, commonOpts)
{
    cmds.push_back(GossCmdReg("build-graph", GossCmdFactoryPtr(new GossCmdFactoryBuildGraph)));
    cmds.push_back(GossCmdReg("print-contigs", GossCmdFactoryPtr(new GossCmdFactoryPrintContigs)));

    globalOpts.addOpt<strings>("debug", "D", "enable particular debugging output");
    globalOpts.addOpt<bool>("help", "h", "show a help message");
    globalOpts.addOpt<string>("log-file", "l", "place to write messages");
    globalOpts.addOpt<strings>("tmp-dir", "", "a directory to use for temporary files (default /tmp)");
    globalOpts.addOpt<uint64_t>("num-threads", "T", "maximum number of worker threads to use, where possible");
    globalOpts.addOpt<bool>("verbose", "v", "show progress messages");
    globalOpts.addOpt<bool>("version", "V", "show the software version");

    commonOpts.addOpt<uint64_t>("buffer-size", "B", "maximum size (in GB) for in-memory buffers (default: 2)");
    commonOpts.addOpt<strings>("fasta-in", "I", "input file in FASTA format");
    commonOpts.addOpt<strings>("fastas-in", "F", "input file containing filenames in FASTA format");
    commonOpts.addOpt<strings>("fastq-in", "i", "input file in FASTQ format");
    commonOpts.addOpt<strings>("fastqs-in", "f", "input file containing filenames in FASTQ format");
    commonOpts.addOpt<strings>("line-in", "", "input file with one sequence per line");
    commonOpts.addOpt<strings>("graph-in", "G", "name of the input graph object");
    commonOpts.addOpt<string>("graph-out", "O", "name of the output graph object");
    commonOpts.addOpt<string>("input-file", "f", "input file name ('-' for standard input)");
    commonOpts.addOpt<uint64_t>("kmer-size", "k", "kmer size to use");
    commonOpts.addOpt<uint64_t>("cutoff", "C", "coverage cutoff");
    commonOpts.addOpt<double>("relative-cutoff", "", "relative coverage cutoff");
    commonOpts.addOpt<string>("output-file", "o", "output file name ('-' for standard output)");
    commonOpts.addOpt<bool>("no-sequence", "",
            "rather than produce a fasta file, produce a table containing the sequence header information");
    commonOpts.addOpt<uint64_t>("min-length", "", "the minimum length contig to print (default 0)");
    commonOpts.addOpt<uint64_t>("expected-coverage", "", "expected coverage");
    commonOpts.addOpt<uint64_t>("edge-cache-rate", "",
            "edge cache size as a proportion of edges (default 4)");
    commonOpts.addOpt<uint64_t>("min-link-count", "",
            "discard links with lower count (default 10)");
    commonOpts.addOpt<bool>("preserve-read-sense", "",
            "preserve the sense of reads (forward vs reverse complement)");
    commonOpts.addOpt<bool>("estimate-only", "",
            "only estimate coverage - don't run command");
    commonOpts.addOpt<uint64_t>("iterate", "",
            "repeat the graph editing operation");
    commonOpts.addOpt<uint64_t>("log-hash-slots", "S",
            "log2 of the number of hash slots to use (default 24)");
    commonOpts.addOpt<bool>("paired-ends", "",
            "paired-end reads, i.e. L -> <- R (default)");
    commonOpts.addOpt<bool>("mate-pairs", "",
            "mate-pair reads, i.e. R <- -> L");
    commonOpts.addOpt<bool>("innies", "",
            "innie oriented reads - synonymous with --paired-end");
    commonOpts.addOpt<bool>("outies", "",
            "outie oriented reads, i.e. L <- -> R");
    commonOpts.addOpt<uint64_t>("insert-expected-size", "",
            "expected insert size");
    commonOpts.addOpt<double>("insert-size-std-dev", "",
            "standard deviation of insert sizes as a percentage of length (default 10%)");
    commonOpts.addOpt<double>("insert-size-tolerance", "",
            "range of allowable insert sizes - in standard deviations (default 2.0)");
    commonOpts.addOpt<strings>("graphs-in", "",
            "read graph names (one per line) from the given file.");
    commonOpts.addOpt<uint64_t>("max-merge", "",
            "The maximum number of graphs to merge at once.");
    commonOpts.addOpt<bool>("delete-scaffold", "",
            "Delete any scaffold files associated with the supergraph before proceeding.");




}

