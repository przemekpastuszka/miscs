#!/bin/sh

#przygotowanie informacji ze wszystkich plików wtmp do przetworzenia przez awk
for c in /var/log/wtmp*
do
    last -if $c >> tmp
done

#awk wypisze wszystkie ip, z których logowano się na konto użytkownika $1
awk '{
    if($1 == "'$1'"){
        hosts[$3] = $3;
    }
}
END{
    for(q in hosts){
        print q;
    }
}
' tmp
rm tmp
