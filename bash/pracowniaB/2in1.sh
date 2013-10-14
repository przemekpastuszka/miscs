#! /bin/sh

echo '%!PS-Adobe-2.0
%%DocumentFonts: (atend)
%%BoundingBox: 0 0 554 770
%%Orientation: Portrait
%%Pages: (atend)
%%EndComments
%%BeginProlog' > result.ps

x=( 0 0 350 )
y=( 0 0 0 )

for i in $(seq 1 2)
do
    echo gsave >> result.ps
    
    echo -90 rotate >> result.ps
    echo -784 -30 translate >> result.ps
    echo ${x[i]} ${y[i]} translate >> result.ps
    echo 0.7 0.7 scale >> result.ps
    
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
