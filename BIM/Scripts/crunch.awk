# crunch.awk
# reads raw data for a given family and program and computes mean
# opcounts, time, and time standard deviation (in percentage of mean)
# Assumptions:
# input file contains two lines without data: one containing the character
# "_" (underscore) (this means that all program names must contain the
# underscore character, and that this character must not appear
# anywhere else in the input file); and another the string "Nodes". 
# data lines are separated by a line containing the string "-1" in the
# first field.
BEGIN{
first_line = 1;  n_instances = 0; aborted = 0;
}
! (/Nodes/ || /_/) {
  if ($1 == -1 && first_line) first_line = 0;
  else if ($1 == -1 && n_instances != 0) # not first line
    {
      fac_arc_mean /= n_instances;
      sac_arc_mean /= n_instances;
      time_mean /= n_instances;
#      usec_per_sac_mean /= n_instances;
# stmt above commented out because I changed the way usec_per_sac_mean is
# computed. Instead of computing the value for each instance and then
# getting the mean, the mean is computed from the mean time and mean #
# of arc fetches, as below.
      usec_per_sac_mean = (time_mean*1e6)/(sac_arc_mean*m);
      for (i = 0; i < n_instances; i++) {
	tmp = time[i] - time_mean;
	sum += tmp*tmp;
      }
      time_sd = sqrt(sum/(n_instances - 1));
      printf("%.2f %.0f %.2f %.2f %.1f %8ld %8ld\n", time_mean,
	     (time_sd/time_mean)*100, fac_arc_mean, sac_arc_mean,
	     usec_per_sac_mean, n, m);
# we must reset everything for the next batch
      n_instances = sum = 0;
      fac_arc_mean = sac_arc_mean = time_mean = sac_per_usec_mean = 0;
    }
  else if ($1 == -1 && n_instances == 0) 
    {
# this indicates aborted run
      aborted = 1;
      printf("n/a n/a n/a n/a n/a n/a n/a\n");
    }
  else
    {
      fac_arc_mean += $4/$2; # it's opcount per arc
      sac_arc_mean += $5/$2; # it's opcount per arc
      usec_per_sac_mean += ($6*1e6)/$5;
      time_mean += $6;
      time[n_instances] = $6;
      n_instances++;
      n = $1; m = $2;
    }
}
END{ # we repeat for the last set
  if (!aborted && n_instances != 0) {
    fac_arc_mean /= n_instances;
    sac_arc_mean /= n_instances;
    time_mean /= n_instances;
#    usec_per_sac_mean /= n_instances;
    usec_per_sac_mean = (time_mean*1e6)/(sac_arc_mean*m);
    for (i = 0; i < n_instances; i++) {
      tmp = time[i] - time_mean;
      sum += tmp*tmp;
    }
    time_sd = sqrt(sum/(n_instances - 1));
    printf("%.2f %.0f %.2f %.2f %.2f %8ld %8ld\n", time_mean,
	   (time_sd/time_mean)*100, fac_arc_mean, sac_arc_mean,
	   usec_per_sac_mean, n, m);
  }
  else {
    printf("n/a n/a n/a n/a n/a n/a n/a\n");
} 
}
