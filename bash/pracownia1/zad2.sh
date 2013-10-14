#!/bin/sh

#Script by Przemysław Pastuszka

#wartości domyślne dla argumentów wywołania
text=""
start=0
end=0 #0 oznacza, że plik ma zostać wypisany aż do końcowej linii

#parsowanie wektora argumentów - s oraz e (jeśli użyte) muszą być podane razem z argumentami
optstring=$(getopt -auo "-s: -e:" -- $*)
if [ $? != 0 ] #sprawdzenie czy linia argumentów została podana poprawnie
then
	echo "usage: $0 [-s start] [-e end]"; exit 2;
fi
set -- $optstring
while [ $# -ge 1 ]
do
	case $1 in
	-s)
		shift; start=$1; shift;;
	-e)
		shift; end=$1; shift;;
	--)
		shift; break;;
	*)
		break;;
	esac
done


if [ $# -ge 1 ] #przypadek nr 1 - w linii argumentów zostały podane pliki do przetworzenia
then
	while [ $# -ge 1 ] #dopóki zostały pliki do przetworzenia
	do
		if [ $1 = "--" ]
		then
			break
		fi
		if [ $end -gt 0 ] #jeśli podano linię końcową trzeba będzie również przyciąć odpowiednio tekst
		then
			tail -n +$start $1 | head -n $(($end - $start + 1))
		else
			tail -n +$start $1
		fi
		shift #przejdź do kolejnego pliku
	done
else
	if [ $end -gt 0 ] #tu następuje czytanie ze standardowego wejścia
	then
		tail -n +$start | head -n $(($end - $start + 1))
	else
		tail -n +$start
	fi
fi

