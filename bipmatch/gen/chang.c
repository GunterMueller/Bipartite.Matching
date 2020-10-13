/* This program written by Frank Chang

================================================
Shin-Jyh Frank Chang
Network Planning           Email: fchang@gte.com
GTE Labs, MS-17            GTE telemail: f.chang
40 Sylvan Road             Tel: (617)466-2884
Waltham, MA 02254          Fax: (617)890-9320
================================================
*/

/* pick.c: random matrix generator, modified on 8/1/88 */
#include <stdio.h>
#define Ms 10000 
#define Ns 10000
#define Rs 500000 /* program does not work with constants larger than this */
#define max(A,B) ((A) >= (B) ? (A) : (B))
#define min(A,B) ((A) <= (B) ? (A) : (B))
FILE *fp1; /*output file cirp.dat*/

int row[Ns], M, N, R;
long seed;

main(argc, argv)
int argc;
char*argv[];
{
    int i, j, k, nn, ratio, M1, density, count, block, r1, r9, ii, garbage = 0;
    register int ci[Rs], rp[Ms+1], rn[Ms];
    int den;
    /* double coeff[Rs]; */
    short a,c,isign;
    long x0,x1,l,big;

    x0=1;
    a=5;
    c=257;
    big=65536;
    fp1 = stdout;
    M = atoi(argv[1]);
    N = atoi(argv[2]);
    ratio = atoi(argv[3]); /* ratio = structured : non-structured */
    density = atoi(argv[4]);
    seed = (argc == 6) ? atoi(argv[5]) :  M*N*ratio;
    srandom(seed);
    M1 = M/(ratio+1);
    M1 = max (M1,2); /* # of rows in non-structured area (NSA) */
    nn = (M1*N*density)/100;           /* # of nonzeros in NSA */
    nn = max(nn,M1);
    picker(M1-1, nn);  /* select M1-1 numbers from 1,2,..,nn-1 */
    sort(row, M1-1); /* these will be the positions for the end of row
2,..,M1 */
    rn[M1] = nn - row[M1-1];  /* rn[i]= # of nonzeros in row i */
    for (i=1; i<=M1-1; i++) {
         rn[i] = row[i] - row[i-1];
         if (rn[i] < N) {
             rn[i] = rn[i] + garbage;
             garbage = 0;
         }
         if (rn[i] >= N) {
             garbage = garbage + (rn[i]-N);
             rn[i] = N;
         }
    }
    count = 1;
    for (i=1; i<=M1; i++) {
         rp[i] = count;
         picker1(rn[i], i);
         for (j=1; j<=rn[i]; j++) {
              isign = (x0%2>0)? 1 : -1;
              x1=(a*x0+c)%big;
              /* coeff[count] = isign*0.5+(double)x1/big; */
              x0=x1;
              ci[count++] = row[j];
         }
    };
    block = 1+(random()%(4*ratio));    /* number of blocks in structured region */
    block = max(block,2);            /* may replace ratio by say 2, 3, 4 ...  */
    block = min(block,M-M1);
    picker(block-1, M-M1);
    sort(row, block-1);
    rn[block] = (M-M1) - row[block-1];
    /* printf("BLOCK %d, rn = %d\n",block,rn[block]);*/
    for (i=1; i<=block-1; i++) {
         rn[i] = row[i] - row[i-1];
         /* printf("BLOCK %d, rn = %d\n",i,rn[i]);*/
    }
    r1 = M1+1;
    for (i=1; i<=block; i++) {
         /* printf("\n\n Process BLOCK %d:\n",i);*/
         r9 = r1 + rn[i] - 1;
         j = N*density/100;
         if (j==0)
             k = 0;
         else
            k = random()%j;   /* k = number of fixed columns chosen for
block i */
         k = max(k,rn[i]+1+(random()%block));
         k = min(k,N);
         /* printf("# of columns selected for BLOCK %d is %d.\n",i,k);*/
         picker2(k, r1, r9);
         /* printf("The %d column indices chosen for block %d are:\n",k,i);
            for (j=1; j<=k; j++)
                 printf ("%d%c", row[j], (j==k)? '\n' : ' ');*/
         for (ii=r1; ii<=r9; ii++) { /* select col indices (from k) for row
ii */
             seed=seed*ii;
             srandom(seed);
             rp[ii] = count;
             /* printf("\n *** For row %d, rp = %d\n",ii,rp[ii]);*/
             for (j=1; j<=k; j++)  /* j is a col from amongst the chosen k */
                  if (random()%(50/density) == 0){
                      isign = (x0%2>0)? 1 : -1;
                      x1=(a*x0+c)%big;
                      /* coeff[count] = isign*0.5+(double)x1/big; */
                      x0=x1;
                      ci[count++] = row[j];
                      /* printf(" j=%d, row[%d]=%d. ",j,j,row[j]);*/
                  }
             if (count==rp[ii]) { /* if none of the k indices was selected
for */
                j=1+(random()%k);       /* row ii, then randomly select one
to   */
                isign = (x0%2>0)? 1 : -1;
                x1=(a*x0+c)%big;
                /* coeff[count] = isign*0.5+(double)x1/big; */
                x0=x1;
                ci[count++] = row[j]; /* make sure that row ii is not empty
*/
             }
         }
         r1 = r9 + 1;
    }
    rp[M+1] = count;
    R = rp[M+1] - 1;
     /*** print coeff[R], ci[R], and rp[M+1] ***/

     fprintf (fp1, "%3d %3d %3d\n",M, N, R);

/*
     for (i=1; i<=R; i++)
          fprintf (fp1, "%6.3f%c", coeff[i], (i==R)? '\n' : ' ');
*/

     for (i=1; i<=R; i++)
          fprintf (fp1, "%4d%c", ci[i], (i==R)? '\n' : ' ');

     for (i=1; i<=M+1; i++)
          fprintf (fp1, " %d%c", rp[i], (i==M+1)? '\n' : ' ');

/*
    printf ("Done! File cirp.dat has been changed for the new matrix.\n");
    printf ("M = %d,      N = %d,        R = %d\n",M,N,R);

    den = ((float) R)/(M*N);
    printf ("Density = %7.2f%%\n", den*100);
*/
}    /*end of main*/

picker(n, k)
int n, k;
{
    int j, w[Rs], newk, index;

    for (j=0; j<=N; j++)
         row[j] = 0;
    for (j=1; j<=k; j++)
         w[j] = j;
    newk = k-1;
    for (j=1; j<=n; j++) {
         index = 1+(random()%newk);
         row[j] = w[index];
         w[index] = w[newk];
         newk = newk - 1;
    }
}

picker1(n, r)
int n, r;
{
    int j, w[Ns], index, count = 1;

    for (j=1; j<=N; j++)
         row[j] = 0;
    for (j=1; j<=N; j++)
         w[j] = j;
    for (j=N; j>=N-n+1; j--) {
         index = 1+(random()%j);
         row[count++] = w[index];
         w[index] = w[j];
    }
}

picker2(n, r1, r9) /* For the block (with rows r1 to r9) find a set of n     */
int n, r1, r9;     /* (from N) column indices, and register them in the      */
{                  /* global array called row[],later each row in this block */
                   /* will find its own set of column indices from row[].    */
    int j, w[Ns], index, count = 1, rs = N;

    for (j=1; j<=N; j++)
         row[j] = 0;
    for (j=1; j<=N; j++)
         w[j] = j;
    j = r1;
    while(j<=N && j<=r9) {  /* this while loop is trying to make the block */
          row[count++] = j; /* appear differently from the others */
          w[j] = rs--;
          j++;
    }
    for (j=rs; j>=N-n+1; j--) {
         index = 1+(random()%j);
         row[count++] = w[index];
         w[index] = w[j];
    }
}

sort(v, n)
int v[], n;
{
    int gap, i, j, temp;
    for (gap = n/2; gap > 0; gap/= 2)
         for (i = gap; i <= n; i++)
              for (j=i-gap; j>=1 && v[j]>v[j+gap]; j-=gap) {
                   temp = v[j];
                   v[j] = v[j+gap];
                   v[j+gap] = temp;
              }
}

/********* end *********/


