// -*- c++ -*-
// $Id :$

#ifndef DFS_H
#define DFS_H

#include <assert.h>

typedef long  CapType;
class match_arc;
class match_node;
typedef match_node node;
typedef match_arc arc;

#include "constant.h"
#include "stack_queue.h"
#include "macros.h"
#include "node.h"
#include "graph.h"

#define FLOW_FORMULATION
#define UNMATCHED G.getSink()

typedef node Ynode;
typedef node Xnode;

class match_node
   : public node_basic_data
{
#define NOT_LABELED LARGE
#define LABELED 0
  node *mtch;
  arc *current;
  arc *lookAhead;

public:
  operator long();
  bool isMatched();
  node *getMatchNode()           {return mtch;}
  void match(node *v)            {mtch = v;}
  void unmatch();
  void setCurrent(arc *a)       {current = a;}
  arc  *getCurrent()            {return current;}
  void setLookAhead(arc *a)       {lookAhead = a;}
  arc  *getLookAhead()            {return lookAhead;}
  void initialize(); 
};

class match_arc
{
//  static graph *G;
  node *hd;
public:
//  static void setGraph(graph *grph) {G=grph;}
  void setHead(node *v)             {hd=v;}
  node *head()                      {return hd;}
};

inline void match_node::initialize() {
  mtch = UNMATCHED;
  node_basic_data::reinitialize();
  lookAhead = node_basic_data::firstOutArc();
}

inline  bool match_node::isMatched()  {
return (mtch != UNMATCHED);
}

inline void match_node::unmatch()  {mtch = UNMATCHED;}

#define forallXNodes(u,G) \
    for( {arc *dfs_ee=(G.getSource())->firstOutArc(); \
         arc *dfs_stopA=G.getSource()->lastOutArc(); \
         u=dfs_ee->head();} \
	 dfs_ee <= dfs_stopA;\
	 u = (++dfs_ee)->head())

#define forallYNodes(u,G) \
    for( {arc *dfs_ee=(G.getSink())->firstOutArc(); \
         arc *dfs_stopA=G.getSink()->lastOutArc(); \
         u=dfs_ee->head();} \
	 dfs_ee <= dfs_stopA;\
	 u = (++dfs_ee)->head())

#endif
