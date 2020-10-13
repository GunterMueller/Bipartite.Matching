// 98-03-03: new version. It now includes a look-ahead heuristic. - JCS

void augment(simpleQueue &Q)
{
  Xnode *v;
  Ynode *w;

  while (!Q.isEmpty())
    {
      v = Q.pop();
      w = v->getCurrent()->head();
      v->match(w);
      w->match(v);
#ifdef STATS
      stats.flowArcCnt++;
      stats.searchArcCnt++;
#endif
    }
#ifdef STATS
  stats.mVal++;
#endif
}   

void greedyMatch()
{
  Xnode *u;
  Ynode *v;
  arc *e;

  forallXNodes(u,G) {
#ifdef STATS
    stats.searchArcCnt++;
#endif
    assert(!u->isMatched());
    forallOutArcs(e,u) {
#ifdef STATS
      stats.searchArcCnt++;
#endif
      v = e->head();
      
      if (!v->isMatched()) {
#ifdef STATS
	stats.mVal++;
	stats.greedCnt++;
	stats.flowArcCnt++;
#endif
	v->match(u);
	u->match(v);
	break;
      }
    }
  }
}

void dfsBIM(simpleQueue &Q, simpleQueue &S)
  // Q is a DFS stack of X nodes
  // S contains all visited X nodes for reinitialization
{
  Xnode *u, *u2, *u3;
  // u is the base of a search; u2 is a visited node, not adjacent to
  // a free Y node; u3 is a node just visited from u2, maybe adjacent to
  // a free Y node.
  Ynode *v;
  arc *e, *e2, *stopA;
  bool foundPath, firstEdge;

  forallYNodes(v,G)
    {
      v->initialize();
#ifdef STATS
      stats.searchArcCnt++;
#endif
    }
  forallXNodes(u,G) 
    {
      u->initialize();
      u->setReached(false);
      u->setCurrent(u->firstOutArc());
#ifdef STATS
      stats.searchArcCnt++;
#endif
    }      

  if (param.greedy) greedyMatch();

  forallXNodes(u,G) 
    {
#ifdef STATS
      stats.searchArcCnt++;
#endif
      if (u->isMatched()) continue;
      assert(!u->isReached());
      // start DFS from u
      foundPath = false;
      firstEdge = true;
      u->setReached(true);
      Q.enqueue(u);
      S.enqueue(u);
      do
	{
	  u2 = Q.pop();
	  e = u2->getCurrent();
	  if (firstEdge)
	    firstEdge = false;
	  else {
	    e = G.succArc(e); // we examined e already
#ifdef STATS
	    stats.searchArcCnt++;
#endif
	  }
	  stopA = u2->lastOutArc();
	  while (e <= stopA) // try to move forward
	    {
#ifdef STATS
	      stats.searchArcCnt++;
#endif
	      v = e->head();
	      if (v == G.getSource())
		{
		  e = G.succArc(e); continue;
		}
	      assert(v->isMatched());
	      u3 = v->getMatchNode();
	      if (u3->isReached())  // includes v == u3
		{
		  e = G.succArc(e); continue;
		}
	      // u3 reached for the first time
	      u2->setCurrent(e);
	      Q.enqueue(u2);
	      u3->setReached(true);
	      S.enqueue(u3);
	      // look-ahead
	      forallOutArcsAfter(e2,u3,u3->getLookAhead())
		{
#ifdef STATS
		  stats.searchArcCnt++;
#endif
		  if (!e2->head()->isMatched())
		    {
		      foundPath = true;
		      break;
		    }
		}
	      u3->setLookAhead(e2);
	      if (foundPath)
		{
		  u3->setCurrent(e2);
		  Q.enqueue(u3);
		  augment(Q);
		  break;
		}
	      else
		{
		  u2 = u3;
		  e = u2->firstOutArc();
		  stopA = u2->lastOutArc();
		}
	    }
	} while (!foundPath && !Q.isEmpty());

      // get ready to start new search
      //      Q.reinitialize();
      if (foundPath)
	{
	  // clean up
	  while (!S.isEmpty())
	    {
	      u = S.pop();
	      assert(u->isMatched());
	      u->setReached(false);
	      //	      u->setCurrent(u->firstOutArc());
	    }
	}
      else
      	S.reinitialize(); //reached vertices remain marked
    }
}

