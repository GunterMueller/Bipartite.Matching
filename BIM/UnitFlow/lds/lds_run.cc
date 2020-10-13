// Hey emacs! This is -*- c++ -*-
// $Id: lds_run.cc,v 1.8 1997/08/15 17:51:09 pmartin Exp $

#define MAIN
#define UNIT_FLOW

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>

#include "param.h"
#include "statistics.h"
#include "uf.h"

#include "inline.h"

#include "timer.c"
#include "lds.cc"
#include "../pr/uf_parser.h"
#include "base_parser.cc"
#include "parse_arguments.cc"

main(int argc, char *argv[])
{
  CapType flowValue;
  arc *a;

  parse_arguments(argc,argv,"go:u:");

#ifndef CONCISE
  cout << "Begin Parsing...\n";
#endif
  parse();

#ifndef CONCISE
  cout << "Parsing has finished. Begin max-flow calculations...\n";
  cout << "\n";
  cout << "c Nodes: " << G.numNodes() << "\n";  
  cout << "c Arcs:  " << G.numArcs()/2 << "\n";
  cout << "\n";
#endif
  simpleQueue S1(G.numNodes()); // contains augmenting paths
  simpleQueue S2(G.numNodes()); // contains visited vertices
  simpleQueue Q(G.numNodes()); // for global relabeling

  float t = timer();
  LDSUnitFlow(S1,S2,Q);
  t = timer() - t;
#if CHECK
  // cout << "checking flow...\n";
  CheckFlow(Q);
  // cout << "flow checked.\n";
#endif
  flowValue = 0;
  forallOutArcs(a,G.getSource())
    if (a->isUsed())
      flowValue++;

#ifndef CONCISE
  cout << "c Running Time: " << t << '\n';
  cout << "c Flow: " << flowValue << '\n';
#endif
#ifdef CONCISE
printf ("%8ld %8ld %8ld %21llu %21llu %11.2f\n",
         G.numNodes(),G.numArcs()/2,
	flowValue,
	stats.flowArcCnt,stats.searchArcCnt,t);
#endif
}
