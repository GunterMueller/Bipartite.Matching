// Hey emacs! This is -*- c++ -*-
// $Id: node.h,v 1.9 1997/07/03 14:02:33 mslevine Exp $

// BDFS stands for breadth/depth first search
// LDS stands for label-directed search

#ifndef NODE_H
#define NODE_H 

#if defined(EXCESSES) && defined(NO_EXCESSES)
#error you can only define one of EXCESSES and NO_EXCESSES
#endif

#if !(defined(NO_EXCESSES) || defined(EXCESSES))
#define NO_EXCESSES
#endif



/***********************************************************************/
/*                                                                     */
/*         CLASS node_basic_data                                       */
/*                                                                     */
/***********************************************************************/

class node_basic_data
{
  arc      *adj_list;      // adjacency list - pointer into array $arcs$

#ifdef EXCESSES
  CapType excess;     // current excess at node
#endif

protected:
#ifdef BDFS
  bool reached;
#elif LDS
  DistType distance;
  bool reached;
#else
  DistType distance;
#endif
//  static graph *G;
  
public:
//  static void setGraph(graph *grph) {G=grph;}
  inline void reinitialize();

  inline arc *firstOutArc();
  inline arc *lastOutArc(); 
  inline arc *succOutArc(arc *e);
  inline arc *prevOutArc(arc *e);
  inline bool contains(arc *e);

  void initAdjList(arc *e) {adj_list = e;}
#ifdef BDFS
  bool isReached() {return reached;}
  void setReached(bool i) {reached = i;}
#elif LDS
  DistType getDistance()        {return distance;}
  void setDistance(DistType l)  {distance = l;}
  bool isReached() {return reached;}
  void setReached(bool i) {reached = i;}
#else
  DistType getDistance()        {return distance;}
  void setDistance(DistType l)  {distance = l;}
  void changeDistance(DistType l) {distance += l;}
  void increaseDistance(DistType l) {distance += l;}
#endif

  long index();
  long name();

  bool isTerminus();
  bool isSink();
  bool isSource();

#ifdef EXCESSES
  bool hasExcess()               { return ((excess >0) ? true :false);}
  void increaseExcess(CapType x) {excess += x;}
  void incrementExcess()         {excess++;}
  void decreaseExcess(CapType x) {excess -= x;}
  void decrementExcess()         {excess--;}
  void setExcess(CapType x)      {excess=x;}
  long getExcess()               {return excess;}  
#else
  bool hasExcess()               {return false;}
  long getExcess()               {return 0;}
#endif

};

#endif
