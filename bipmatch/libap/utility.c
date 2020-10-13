/* utility.c */

#include <stdio.h>
#include "misc.h"
#include "utility.h"

/* Note: instead of calling `malloc', we are calling `sbrk'.  The stuff
   that is being allocated is being used for the life of the program and
   is only discarded on exit, so we're not losing any flexibility.  Since
   many things are being allocated an `malloc' has some overhead (wasted
   or bookkeeping space) on each allocation, the total wasted space
   overhead is very large.  `sbrk' does no bookkeeping so we save a lot
   of space.  Don't try to free anything returned by `Alloc' or you will
   lose! */

char *Alloc(n)
int n;
{
  char *malloc(), *sbrk(), *p;

  if ((p = sbrk(n)) == 0)
    Barf("Out of space");

#ifdef NDEF
  /* Here's some code that checks, for each allocation size, the
     alignment of the allocated storage.  It prints out the bit number
     of the lowest bit that is set for that size.  Note: allocates a
     bunch of space, particularly if any large things are allocated.
     (The allocation space is 4X the size of the largest allocation.)
     Each time a smaller alignment is used for a size it prints out
     that smaller alignment. */

  {
    static int *low = NULL;
    static int nsizes = 0;
    static int lowbit = 32;
    long addr;
    int i;

    if (low == NULL) {
      low = (int *)malloc(256);
      if (!low) {
	Barf ("Super-fail");
      }
    }
    if (n > nsizes) {
      low = (int *)realloc (low, (n+4)*sizeof(int));
      if (!low) {
	Barf ("Super-fail 2");
      }
      while (nsizes <= n) {
	low[nsizes] = 32;
	++nsizes;
      }
    }

    addr = (long)p;

    for (i=0; i<low[n]; ++i) {
      if (addr & 1) {
	low[n] = i;
	printf ("low[%d] == %d\n", n, low[n]);
	break;
      }
      addr = addr >> 1;
    }
  }
#endif

  return p;
}

void Barf(s)
char *s;
{
  fprintf(stderr, "%s\n", s);
  exit(-1);
}

int EOF_Test(f)
FILE *f;
{
  char c, ReadChar();

  c = ReadChar(f);
  if (c == EOF)
    return TRUE;
  ungetc(c, f);
  return FALSE;
}


int SkipLine(f)
FILE *f;
{
  char c;

  do
    c = getc(f);
  while (c != EOF && c != '\n');

}

/* Skip whitespace */
void Skip(f)
FILE *f;
{
  char c;
 
  while (isspace(c = getc(f)))
    ;
  ungetc(c,f);
}

   
/* Get a string terminated by whitespace */
int GetString(f, buff)
FILE *f;
char *buff;
{
  char c;

  Skip(f);
  while (!isspace(c = getc(f)))
    *buff++ = c;
  *buff = 0;
}

int Strcmp(s1, s2)
char *s1, *s2;
{
  while (*s1 && *s2){
    if (*s1++ != *s2++)
      return FALSE;
  }
  return *s1 == *s2;

}



void StrAppend(s1, s2, s3)
char *s1, *s2, *s3;
{
  while (*s1)
    *s3++ = *s1++;
  while (*s2)
    *s3++ = *s2++;
  *s3 = 0;
}


int GetInt(f)
FILE *f;
{
  char c, ReadChar();
  int v, sign;

  c = ReadChar(f);
  sign = FALSE;
  v = 0;

  if (c == '-'){
    sign = TRUE;
    c = getc(f);
  }
  while (isdigit(c)){
    v = 10*v + (c - '0');
    c = getc(f);
  }
  if (sign)
    v = -1*v;

  ungetc(c, f);
  return v;
}

void PutInt(i, f)
int i;
FILE *f;
{
  char c;
  int d;

  if (i == 0){
    putc('0', f);
    return;
  }

  if (i < 0){
    putc('-', f);
    i = -1*i;
  }
  
  d = 1;
  while (d <= i)
    d *= 10;
  d /= 10;
  while (d > 0){
    c = i / d;
    i %= d;
    d /= 10;
    putc('0'+c, f);
  }
  
}

char ReadChar(f)
FILE *f;
{
  char c;

  do {
    c = getc(f);
  } while (isspace(c));
  return c;
}

int Min(x, y)
int x, y;
{
  return (x > y) ? y : x;
}

int Max(x, y)
int x, y;
{
  return (x > y) ? x : y;
}


/* Open a file for reading - if the file doesn't exist,
then the extention .max is tried, if that doesn't work
then the data directory is checked - with or without the extention.
*/

FILE *OpenFile(c)
char *c;
{
  FILE *f;
  char buff1[100], buff2[100];

  if ((f = fopen(c,"r")) != NULL)
    return f;

  StrAppend(c,".max",buff1);
  if ((f = fopen(buff1,"r")) != NULL)
    return f;

  StrAppend(DATA_DIRECTORY,c,buff1);
  if ((f = fopen(buff1,"r")) != NULL)
    return f;

   StrAppend(buff1,".max",buff2);
   return fopen(buff2,"r");

  

}


Queue *MakeQueue(n)
int n;
{
  Queue *Q;

  Q = (Queue *) Alloc(sizeof(Queue));

  Q->data = (int *) Alloc(n * sizeof(int));
  Q->tail = 0;
  Q->head = 0;

  Q->size = n;

  return Q;
}

void InitQueue(Q)
Queue *Q;
{
  Q->tail = 0;
  Q->head = 0;
}

int Dequeue(Q)
Queue *Q;
{
  int v;

  if (Q->tail == Q->head)
    Barf("Attempt to dequeue from empty queue");

  v = Q->data[Q->head];
  Q->head = (Q->head == Q->size - 1) ? 0 : Q->head + 1;

  return v;
}

void Enqueue(Q, k)
Queue *Q;
int k;
{
  if (Q->head == Q->tail + 1 ||
      (Q->tail == Q->size - 1 && Q->head == 0))
    Barf("Queue overfull");

  Q->data[Q->tail] = k;
  Q->tail = (Q->tail == Q->size - 1) ? 0 : Q->tail + 1;
}

int QSize(Q)
Queue *Q;
{
  return (Q->tail >= Q->head) ? Q->tail - Q->head
                              : Q->tail - Q->head + Q->size;
}

Boolean QueueEmpty(Q)
Queue *Q;
{
  return Q->head == Q->tail;
}
