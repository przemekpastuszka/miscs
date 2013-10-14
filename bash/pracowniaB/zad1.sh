#! /bin/sh

# Script by Przemysław Pastuszka

# Linia opcji:
# plik_danych przedzial_czasu liczba_kolumn_wartosci nr_kolumny1 nr_kolumny2...
# przedzial czasu jest postaci ["%Y-%b-%d %H:%M:%S":"%Y-%b-%d %H:%M:%S"]
#

names=( "" "" "" "" "temperatura wewn" "temperatura na zewn" "punkt rosy" "wilgotnosc wewn" "wilgotnosc zewn" "predkosc wiatru" "kierunek wiatru (stopnie)" "kierunek wiatru (symbolicznie)" "i tak dalej...")

file=$1
range=$2
lines=$3
shift
shift
shift
cmd='set terminal png; set xdata time; set timefmt "%Y-%b-%d %H:%M:%S"; set xrange '$range';plot '
for i in $(seq 1 $lines)
do
    arg=$1
    shift
    cmd=$(echo $cmd '"'$file'" using 2:'$arg' with lines title "'${names[$arg]}'"')
    if [ $i -ne $lines ]
    then
        cmd="${cmd},"
    fi
done

echo $cmd | gnuplot > out.png

