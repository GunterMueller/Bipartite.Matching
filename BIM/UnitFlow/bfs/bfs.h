// -*- c++ -*-

#ifndef UF_H
#define UF_H

class unit_flow_arc : public  arc_basic_data
{
  bool used;                        // true if already in use in this direction
  unit_flow_arc  *reverse;          // Pointer to reverse arc
#define REVERSE_ARCS                // Needed for correct parsing  
#define REVERSE_POINTER             // Needed for correct parsing  
  bool forward;
public:
  bool isUsed()                     {return used;}
  void setReverse(unit_flow_arc *e) {reverse = e;}
  arc *reverseArc()                 {return (arc *) reverse;}
  void use()                        {used=true; reverse->used=false;}
  void unuse()                      {used=false; reverse->used=true;}
  void setUsed(bool b)              {used = b;}      
  bool isForward()                  {return forward;}
  void setForward(bool b)           {forward = b;}
};

#endif \\  End of uf.h


