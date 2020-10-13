/* timer.c */

#include <sys/time.h>
#include <sys/resource.h>

struct rusage ru;
struct timeval start_time, end_time, sample_time;

begintiming()
{
  getrusage(0,&ru);
  start_time = ru.ru_utime;
}

/* Return difference between start and ending time in hundredths of a
second.
*/
int endtiming()
{
  int t;

  getrusage(0,&ru);
  end_time = ru.ru_utime;
  if (end_time.tv_usec < start_time.tv_usec){
    end_time.tv_usec += 1000000;
    end_time.tv_sec -= 1;
  }
  t = 100*(end_time.tv_sec - start_time.tv_sec) +
	(end_time.tv_usec - start_time.tv_usec) / 10000;
  printf("run time %.2f secs\n", (double) t/100.0);
  return t;
}

/* Return difference between start and sample time in .001 seconds.
*/
int sampletime()
{
  int t;

  getrusage(0,&ru);
  sample_time = ru.ru_utime;
  if (sample_time.tv_usec < start_time.tv_usec){
    sample_time.tv_usec += 1000000;
    sample_time.tv_sec -= 1;
  }
  t = 1000*(sample_time.tv_sec - start_time.tv_sec) +
	(sample_time.tv_usec - start_time.tv_usec) / 1000;
  return t;
}
