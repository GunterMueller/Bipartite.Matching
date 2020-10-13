#!/bin/csh -f
# table.csh
# main driver script to prepare tables from raw experimental data
# usage: table.csh <problem> <resultsdir>
# where <problem> is `bim' or `uf'
# SET BASE DIRECTORY HERE
set base = /home/lac/setubal/netflow/andrew/unit/UFBM-98-03-06
# set programs
if ($1 == "bim") then 
   set progs = (bim_dfs bim_bfs bim_lds bim_ar bim_fifo bim_lo bim_hi)
else if ($1 == "uf") then 
   set progs = (uf_dfs uf_bfs uf_lds uf_ar uf_fifo uf_lo uf_hi)
else 
   echo "Wrong problem parameter; must be bim or uf"
   exit
endif
# set families
if ($1 == "bim") then 
   set fams =  (HiLo fewg manyg Grid Hexa Rope Zipf)
else set fams =  (Karz rmfuL rmfuW rmfuC Blow Puff Saus Squa Wave)
endif
# results directory is a command-line parameter (subdir of $base assumed)
set results = $2
# set scripts directory here
set scripts = Scripts
cd $base/$results
echo "Moving to directory " $base/$results
foreach family ($fams)
   foreach program ($progs)
       echo "crunching results from $program"
       echo $program >> temp
       gawk -f $base/$scripts/crunch.awk $program.$family >> temp
   end
   echo "prettyprinting results for $family"
   echo "%% results for $family" > $family.tex
   gawk -f $base/$scripts/pp.awk family=$family temp >> $family.tex
   rm temp
end
#
