#!/bin/sh
#Script by Przemysław Pastuszka

date=$(date "+%d.%m.%Y") #pobierz stronę z dzisiejszym kursem EURO, wyodrębnij go, przecinek zamień na kropkę
wget -O tmp -q http://waluty.onet.pl/notowania-walut-on-line-forex,18906,notowania-online?datetime=$date
current=$(grep -A 5 EUR/PLN tmp | grep txtBold| grep -o '[0-9]*,[0-9]*' | sed 's/,/./')
rm tmp

#jak wyżej tylko z wczorajszym kursem
date=$(date --date='1 day ago' "+%d.%m.%Y")
wget -O tmp -q http://waluty.onet.pl/notowania-walut-on-line-forex,18906,notowania-online?datetime=$date
last=$(grep -A 5 EUR/PLN tmp | grep txtBold| grep -o '[0-9]*,[0-9]*' | sed 's/,/./')
rm tmp

#jeśli current i last różnią się o niewielką wartość wypisz, że kurs jest stały
if [ $(echo "($current - $last) ^ 2 < 0.0001 " | bc -l) = 1 ]
then
    echo STAŁY
else
    if [ $(echo "$current > $last" | bc) = 1 ]
    then
        echo ROŚNIE
    else
        echo SPADA
    fi
fi
