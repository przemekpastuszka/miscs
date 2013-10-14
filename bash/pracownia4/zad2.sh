#!/bin/sh
#Script by Przemysław Pastuszka

#w razie braku podania początku i końca okresu program powinien skanować wszystkie wpisy, stąd poniżsi wartownicy:
p="1900-01-01"
k=$(date "+%Y-%m-%d")

#czy AWK ma wykonać sprawdzenia kolejno dla wartości granicznej salda, maksymalnego obciążenia czy wypłaty na podany numer konta
czyS=0
czyO=0
czyN=0

#parsowanie wektora argumentów
optstring=$(getopt -auo "-p: -k: -s: -o: -n:" -- $*)
if [ $? != 0 ] #sprawdzenie czy linia argumentów została podana poprawnie
then
	echo "usage: $0 [-p poczatek okresu] [-k koniec okresu] [-s wartość graniczna salda] [-o max. obciążenie] [-n numer konta] PLIK"; exit 2;
fi
set -- $optstring
while [ $# -ge 1 ]
do
	case $1 in
	-p)
		shift; p=$(date --date="$1" "+%Y-%m-%d"); shift;;
	-k)
		shift; k=$(date --date="$1" "+%Y-%m-%d"); shift;;
    -s) shift; czyS=1;s=$1; shift;;
    -o) shift; czyO=1;o=$1; shift;;
    -n) shift; czyN=1;n=$1; shift;;
	--)
		shift;break;;
	*)
		break;;
	esac
done

#program w AWK skanujący podany na wejściu plik
#niewiele się tutaj dzieje, stąd brak komentarzy
awk -F ';' '{
    if("'$p'" <= $1 && $1 <= "'$k'"){
        if('$czyS' == 1 && $4 < '${s-0}')
            print "Spadek salda do "$4" dnia "$1;
        if('$czyO' == 1 && $3 < -'${o-0}')
            print "Obciążenie rachunku o "$3" dnia "$1;
        if('$czyN' == 1 && $2 ~ /PRZELEW ZEWNĘTRZNY WYCHODZĄCY*/ && substr($2, 32, 26) == "'${n-0}'")
            print "Wypłata na numer konta '$n' dnia "$1;
    }
    }' $1
