#!/bin/sh
#Script by Przemysław Pastuszka

#w razie braku podania początku i końca okresu program powinien skanować wszystkie wpisy, stąd poniżsi wartownicy:
p="1900-01-01"
k=$(date "+%Y-%m-%d")

#parsowanie wektora argumentów
optstring=$(getopt -auo "-p: -k:" -- $*)
if [ $? != 0 ] #sprawdzenie czy linia argumentów została podana poprawnie
then
	echo "usage: $0 [-p poczatek okresu] [-k koniec okresu] PLIK"; exit 2;
fi
set -- $optstring
while [ $# -ge 1 ]
do
	case $1 in
	-p)
		shift; p=$(date --date="$1" "+%Y-%m-%d"); shift;;
	-k)
		shift; k=$(date --date="$1" "+%Y-%m-%d"); shift;;
	--)
		shift;break;;
	*)
		break;;
	esac
done

#Program oblicza średnie saldo dzienne dla okresu zadanego granicznymi datami za pomocą zwykłej średniej arytmetycznej, przy czym:
#-w każdym dniu wybierane jest saldo końcowe (tj. po wykonaniu wszystkich operacji)
#-jeśli w danym dniu nie było operacji to za saldo dzienne przyjmowane jest saldo z dnia poprzedniego
#-dni pomiędzy zadaną początkową datą p a datą pierwszego wystąpieniem operacji o wartości większej niż p są pomijane w trakcie liczenia
#   salda
awk -F ';' '
    function ToTimeStamp(date){
        return mktime(substr(date, 0, 4)" "substr(date, 6, 2)" "substr(date, 9, 2)" 00 00 00");
    }
    function DaysBetween(date1, date2){
        return (ToTimeStamp(date1) - ToTimeStamp(date2)) / (60 * 60 * 24);
    }
    BEGIN{
        nrOfDays = 0;
        sum = 0;
        lastBalance = 0;
        lastDate = "'$p'";
        occurence = 0;
    }
    {
        if("'$p'" <= $1 && $1 <= "'$k'"){
            if($1 != lastDate){
                if(occurence == 1)
                    diff = DaysBetween($1, lastDate);
                else
                    diff = 0;
                nrOfDays += diff;
                sum += lastBalance * diff;
                lastDate = $1;
            }
            lastBalance = $4;
            occurence = 1;
        }
    }
    END{
        diff = DaysBetween("'$k'", lastDate) + 1;
        nrOfDays += diff;
        sum += lastBalance * diff;
        print "Średnie saldo dzienne wyniosło "(sum / nrOfDays);
    }' $1
