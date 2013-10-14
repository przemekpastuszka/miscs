#! /bin/sh

echo %! > g.ps
echo gsave >> g.ps 
echo 0 400 translate >> g.ps                  # for Gnuplot plots
cat $1 | sed -e "s/showpage//" >> g.ps 
echo grestore >> g.ps 
echo gsave >> g.ps 
echo 0 090 translate >> g.ps                  # for Gnuplot plots
cat $2 >> g.ps 
