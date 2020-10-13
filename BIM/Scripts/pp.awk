# pp.awk
# reads a file containing crunched data for a given family and formats
# it in simple ASCII and in LaTeX table style. 
# The ASCII format is used to generate raw data for gnuplot.
# family=(must be supplied).
# Assumptions: 
# * the input file must contain lines identifying the
# implementation that generated the data that follows. This line must contain
# the character "_" (underscore) in it. Whatever string is on this line is
# used to identify the implementation in the output.
# * there must be exactly 7 implementation (this dependency appears only on
# LaTeX output because of column formatting)
BEGIN{
}
/_/ { # description line
progname = $1
i = 0
pn[n_progs++] = progname;
n_sizes = 0;
}
! /_/ { # data line
  if ($6 != "n/a") {
    n[n_sizes] = $6; m[n_sizes++] = $7;
  }
  time[i,progname] = $1;
  sd[i,progname] = $2;
  fac[i,progname] = $3;
  sac[i,progname] = $4;
  sacusec[i,progname] = $5;
  i++;
}
END {
# raw data for gnuplot
# first we print the program names
  printf("#       ") > family ".data";
  for (i = 0; i < n_progs; i++)
    {
      printf("%10s ", pn[i]) > family ".data";
    }
  printf("\n") > family ".data";
# now for each size
  for (s = 0; s < n_sizes; s++)
    {
# we print number of nodes and number of arcs
      printf("#%8ld %8ld\n", n[s], m[s]) > family ".data";
      printf("%8ld ", n[s]) > family ".data";
# now for each program, we print the running time
      for (i = 0; i < n_progs; i++)
	{
	  if (time[s,pn[i]] == "n/a") 
	    {
	      printf("*         ") > family ".data";
	    }
	  else printf("%10.2f ", time[s,pn[i]]) > family ".data";
	}
      printf("\n#        ") > family ".data";
# now for each program, we print its standard deviation (not used by gnuplot)
      for (i = 0; i < n_progs; i++)
	{
	  printf("%10.0f ", sd[s,pn[i]]) > family ".data";
	}
      printf("\n#        ") > family ".data";
# now for each program, we print its flowArcCnt per arc mean 
# (currently not used by gnuplot)
      for (i = 0; i < n_progs; i++)
	{
	  printf("%10.2f ", fac[s,pn[i]]) > family ".data";
	}
      printf("\n#        ") > family ".data";
# now for each program, we print its searchArcCnt per arc mean
# (currently not used by gnuplot)
      for (i = 0; i < n_progs; i++)
	{
	  printf("%10.2f ", sac[s,pn[i]]) > family ".data";
	}
      printf("\n#        ") > family ".data";
# now for each program, we print its usec per searchArcCnt mean
# (currently not used by gnuplot)
      for (i = 0; i < n_progs; i++)
	{
	  printf("%10.2f ", sacusec[s,pn[i]]) > family ".data";
	}
      printf("\n") > family ".data";
    }
  close(family ".data");
# latex output
# we must put a slash before the underscores in program names, because the underscore
# is a special character in LaTeX
  for (i = 0; i < n_progs; i++)
    {
      pn_s[i] = pn[i]
      gsub(/_/,"\\_",pn_s[i])
    }
# The LaTeX output is to be included in a larger document; but the output
# can be made to be a stand-alone LaTeX file with the two lines below, plus
# the one at the end.
#printf("\\documentstyle[12pt]{article}\n");
#printf("\\begin{document}\n");
printf("\\begin{table}[hb]\n");
printf("\\caption{Results for family {\\%s}}\n",family);
printf("\\label{tab:%s}\n",family);
printf("\\centerline{\n");
printf("\\begin{tabular}{|r||r|r|r|r|r|r|r|l} \\cline{1-8}\n");
printf("$|V|,|A|$ & {\\sc %s} & {\\sc %s} & {\\sc %s} & {\\sc %s} & {\\sc %s} & {\\sc %s} & {\\sc %s} &\\\\ \\cline{1-8} \\cline{1-8}\n",
pn_s[0],pn_s[1],pn_s[2],pn_s[3],pn_s[4],pn_s[5],pn_s[6]);
# now for each size
  for (s = 0; s < n_sizes; s++)
    {
# we print number of nodes
      printf("%d ", n[s]);
# now for each program, we print the running time
      for (i = 0; i < n_progs; i++)
	{
	  if (time[s,pn[i]] == "n/a") 
	    {
	      printf("& N/A ");
	    }
	  else printf("& {\\bf %.2f} ", time[s,pn[i]]);
      }
      if (s == 0) printf("& time\\\\ \n");
      else printf("&\\\\ \n");
# we print number of arcs
      printf("%d ", m[s]);
# now for each program, we print its standard deviation
      for (i = 0; i < n_progs; i++)
	{
	  if (sd[s,pn[i]] == "n/a") 
	    {
	      printf("& N/A ");
	    }
	  else 	printf(" & %.0f\\\% ", sd[s,pn[i]]);
      }
      if (s == 0) printf("& sd \\\\ \n");
      else printf("& \\\\ \n");
# now for each program, we print its flowArcCnt per arc mean
# we're omitting this information as of March, 98 because this
# statistic doesn't seem to add any useful information.
      printf("%");
      for (i = 0; i < n_progs; i++)
	{
	  if (fac[s,pn[i]] == "n/a") 
	    {
	      printf("& N/A ");
	    }
	  else printf("& %.2f ", fac[s,pn[i]]);
	}
      printf("\\\\\n");
# now for each program, we print its searchArcCnt per arc mean
      for (i = 0; i < n_progs; i++)
	{
	  if (sac[s,pn[i]] == "n/a") 
	    {
	      printf("& N/A ");
	    }
	  else printf("& %.2f ", sac[s,pn[i]]);
	}
      if (s == 0) printf("& af/arc \\\\ \n");
      else printf("& \\\\ \n");
# now for each program, we print its usec per searchArcCnt mean
      for (i = 0; i < n_progs; i++)
	{
	  if (sacusec[s,pn[i]] == "n/a") 
	    {
	      printf("& N/A ");
	    }
	  else printf("& %.2f ", sacusec[s,pn[i]]);
	}
      if (s == 0) printf("& $\\mu$sec/af\\\\ \\cline{1-8}\n");
      else printf("& \\\\ \\cline{1-8}\n");
    }
printf("\\end{tabular}\n");
printf("}\n");
printf("\\end{table}\n\\clearpage\n");
#printf("\\end{document}\n");
}

