/* utility.c */

#include <stdio.h>
#include "misc.h"
#include "utility.h"

char *Alloc(n)
int n;
{
  char *malloc(), *sbrk(), *p;

  if ((p = sbrk(n)) == 0)
    Barf("Out of space");

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
