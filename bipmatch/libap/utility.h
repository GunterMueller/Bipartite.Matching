/* exported routines */

extern char 		*Alloc();
extern void		Barf();
extern int 		EOF_Test();
extern int		SkipLine();
extern void 		Skip();
extern int 		GetString();
extern int		Strcmp();
extern void 		StrAppend();
extern int 		GetInt();
extern void 		PutInt();
extern char		ReadChar();
extern int 		Min();
extern int 		Max();
extern FILE		*OpenFile();

/* exported data type */

typedef struct {
  int head, tail, size;
  int *data;
} Queue;


extern Queue *MakeQueue();
extern void InitQueue();
extern int Dequeue();
extern void Enqueue();
extern int QSize();
Boolean QueueEmpty();
