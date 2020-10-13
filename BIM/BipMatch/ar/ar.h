// -*- c++ -*-
// $Id :$

#ifndef AR_H
#define AR_H

#include <assert.h>

typedef long  CapType;
typedef long DistType;
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

#ifdef AR
#define AUGMENT
#endif

typedef node Ynode;
typedef node Xnode;

class match_node
   : public node_basic_data
{
#define NOT_LABELED LARGE
#define LABELED 0
  node *mtch;
  arc *current;

public:
  operator long();
  bool isMatched();
  node *getMatchNode()           {return mtch;}
  void match(node *v)            {mtch = v;}
  void unmatch();
  void unlabel()                 {setDistance(NOT_LABELED);} 
  void label()                   {setDistance(LABELED);}
  bool isUnlabeled()             {return (getDistance() == NOT_LABELED);}
  void XsetDistance(DistType l)  {setDistance(l);}
  void YsetDistance(DistType l)  {}
  DistType XgetDistance()        {return getDistance();}
  DistType YgetDistance()        {return mtch->getDistance()+1;}
  void setCurrent(arc *e)        {current = e;}
  arc  *getCurrent()             {return current;}
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
  setCurrent(node_basic_data::firstOutArc());
  node_basic_data::reinitialize();
  setDistance(0);
}

inline  bool match_node::isMatched()  {
return (mtch != UNMATCHED);
}

inline void match_node::unmatch()  {mtch = UNMATCHED;}

#define forallXNodes(u,G) \
    for( {arc *_ee; _ee=(G.getSource())->firstOutArc(); u=_ee->head();} \
	  _ee <= G.getSource()->lastOutArc(); \
    	  u = (_ee = G.succArc(_ee))->head() )

#define forallYNodes(u,G) \
    for( {arc *_ee; _ee=(G.getSink())->firstOutArc(); u=_ee->head();} \
	  _ee <= G.getSink()->lastOutArc(); \
    	  u = (_ee = G.succArc(_ee))->head() )

#endif // End of ar.h
