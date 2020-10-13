void printArc(arc *a)
{
  cout << "   (" << a->reverseArc()->head()->index()+1 << "," <<
    a->head()->index()+1 << ")";
}

// forw declaration
void globalUpdate(simpleQueue &Q);

void Relabel(node *u) 
{
  arc  *a;
  arc  *minA;  // the arc out of u with lowest head
  node *v;
  long minD;   // the height of the head 
  long d;

  minD = LARGE;
  minA = u->lastOutArc();
  forallOutArcs(a,u) {
#ifdef STATS
    stats.searchArcCnt++;
#endif
    if (a->isAvailable()) {
      d = a->head()->getDistance();
      if (d < minD) {
	minD = d;
	minA = a;
      }
    }
  }
  assert(minD+1 > u->getDistance());
  u->setDistance(minD+1);       // relabel u relative to minA
  if (minD+1 < G.numNodes()) {
    u->setCurrent(minA);        // set the current arc pointer of u
  }
}

void augment(simpleQueue &S)
{
  node *u;
  arc *a;

  while (!S.isEmpty()) {
    u = S.pop();
    a = u->getCurrent();     
    a->use();
#ifdef STATS
    stats.searchArcCnt++;
    stats.flowArcCnt++;
#endif
  }
}   

void ARUnitFlow(simpleQueue &S)
{
  node *u, *v;
  arc  *a, *curr, *a2;
  long relsSinceUpdate = 0; // used to determine when to perform global update

  forallNodes(u,G) u->initialize();
  // We use the same queue/stack structure for global update and
  // for holding the augmenting path, since their use is totally disjoint.
  // Note that we only place a node u on the stack after we know
  // that there's an admissible arc (u,v). Hence, when we are scanning
  // a node v, v is the head of an arc whose tail is on the top of
  // the stack.
  globalUpdate(S);
  S.reinitialize();
  u = G.getSource();
  do {
    curr = u->getCurrent();
    forallOutArcsAfter(a, u, curr)
      {
#ifdef STATS
	stats.searchArcCnt++;
#endif
	if (a->isUsed()) continue; // cap == flow
	v = a->head();
	if (u->getDistance() <= v->getDistance()) continue; // wrong label
	// found admissible arc
	if (v == G.getSink())
	  {
	    // found augmenting path
	    a->use(); // because this arc is not on the stack
	    augment(S);
	    S.reinitialize();
	    u = G.getSource();
	    a2 = u->getCurrent();
	    u->setCurrent(++a2); // low level!!!
#ifdef STATS
	    stats.searchArcCnt++;
#endif
	    goto next;
	  }
	else
	  {
	    // advance
	    S.enqueue(u);
	    u->setCurrent(a);
	    u = v;
	    goto next;
	  }
      }
    // we can only be here if there's NO admissible arc 
    // (end of for loop was reached)
    Relabel(u);
    relsSinceUpdate++;
    if (relsSinceUpdate > param.globalUpdateFreq *(double)G.numNodes()) {
      globalUpdate(S);
      relsSinceUpdate = 0;
      S.reinitialize();
      u = G.getSource();
    }
    else if (u != G.getSource())
      {
	/* retreat */
	u = S.pop();
	a2 = u->getCurrent();
	u->setCurrent(++a2); // low level!!!
#ifdef STATS
	stats.searchArcCnt++;
#endif
      }
  next: ;
  } while (G.getSource()->getDistance() < G.numNodes());
}      
		
void  globalUpdate(simpleQueue &Q)
{
  node *u, *v;
  arc  *e;
  DistType  currentDistance; // the current depth in the breadth first search

#ifdef STATS
  stats.updateCnt++;
#endif

  Q.reinitialize();
  forallNodes(u,G) u->setDistance(VERY_FAR); // mark all nodes as unlabeled
  G.getSink()->setDistance(0);           // set sink to distance 0
  Q.enqueue(G.getSink());                // add the sink to the queue
  while (!Q.isEmpty()) {      
    v = Q.dequeue();
    currentDistance = v->getDistance();  // distance of node being scanned
    forallOutArcs(e,v) {
#ifdef STATS
      stats.searchArcCnt++;
#endif
      arc &ee = *e;
      u = ee.head();
      if (u->getDistance() < VERY_FAR) continue;
      if (e->isAvailable()) continue;
      u->setCurrent(e->reverseArc());
      u->setDistance(currentDistance+1);
      Q.enqueue(u);
    }
  }
}

void CheckFlow(simpleQueue &Q)
{
  node *u, *v;
  long f, fs = 0, ft = 0;
  arc *a;

  forallOutArcs(a,G.getSource()) if (a->isUsed()) fs++;
  forallOutArcs(a,G.getSink()) if (a->isAvailable()) ft++;
  if (fs != ft)
    {
      cout << "ERROR: flow out of s(" << fs << ") != flow into t(" << ft << ")\n";
    }
  // maximality check with BFS
  forallNodes(u,G)
    {
      u->setDistance(VERY_FAR);
    }
  u = G.getSource();
  u->setDistance(0);
  Q.reinitialize();
  Q.enqueue(u);
  do 
    {      
      v = Q.dequeue();
      forallOutArcs(a,v) {
	if (a->isUsed()) continue;
	u = a->head();
	if (u->getDistance() < VERY_FAR) continue;
	if (u == G.getSink()) 
	  {
	    cout << "ERROR: flow not maximum!\n";
	    break;
	  }
	else
	  {
	    u->setDistance(0);
	    Q.enqueue(u);
	  }
      }
    } while (!Q.isEmpty());
}
