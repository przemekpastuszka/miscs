#! /bin/sh

# Script by Przemysław Pastuszka

echo '%!PS-Adobe-2.0
%%DocumentFonts: (atend)
%%BoundingBox: 0 0 554 770
%%Orientation: Landscape
%%Pages: (atend)
%%EndComments
%%BeginProlog' > result.ps

x=( 0 0 0 250 250 )
y=( 0 0 350 0 350 )

for i in $(seq 1 4)
do
    echo gsave >> result.ps
    echo ${x[i]} ${y[i]} translate >> result.ps
    echo 0.5 0.5 scale >> result.ps
    tail -n +10 $1 | head -n -5 >> result.ps
    shift
    echo grestore >> result.ps
    
done
echo '
end
showpage
%%Trailer
%%DocumentFonts: Helvetica
%%Pages: 1' >> result.ps
ps2pdf result.ps
rm result.ps
