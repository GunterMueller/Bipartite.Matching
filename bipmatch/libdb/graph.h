#define MAX_N 		65600
#define MAX_VERTICES	MAX_N

typedef struct enode {
  struct enode *next;
  int h;
} Edge;

typedef struct {
  Edge *firstEdge;
  Edge *nextEdge;
  int  mate;		/* mate == 0 means no mate */
} U_vertex;

typedef struct {
  U_vertex A[MAX_N];
  int size;
  int edges;
} U_partition;

typedef struct {
  int  mate;		 /* mate == 0 means no mate */
} V_vertex;

typedef struct {
  V_vertex A[MAX_N];
  int size;
} V_partition;

