#!/bin/sh

#Script by Przemysław Pastuszka

#wartości domyślne dla argumentów wywołania
directory=""
time=30

#parsowanie wektora argumentów - t oraz d (jeśli użyte) muszą być podane razem z argumentami
optstring=$(getopt -auo "-t: -d:" -- $*)
if [ $? != 0 ] #sprawdzenie czy linia argumentów została podana poprawnie
then
	echo "usage: $0 [-t time] [-d directory]"; exit 2;
fi

set -- $optstring
while [ $# -ge 1 ]
do
	case $1 in
	-d)
		shift; directory=$1; shift;;
	-t)
		shift; time=$1; shift;;
	--)
		shift;;
	*)
		break ;;
	esac
done

#obliczona zostaje data sprzed $time dni
TIME=$(($(date +%s) - 60 * 60 * 24 * $time))

for file in ${directory}*.c
do
	#sprawdzenie czy plik istnieje i jest dostępny do odczytu
	if test -r $file
	then
		filetime=$(stat -c %Y ${file}) #pobranie daty ostatniej modyfikacji pliku
	
		#rozpatrywane są tylko pliki młodsze niż $time dni
		if [ $filetime -gt $TIME ]
		then
			echo "\n$file:"
			diff $file ${file}~
		fi
	fi
done
