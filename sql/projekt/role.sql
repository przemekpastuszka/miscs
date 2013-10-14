/*
Przemysław Pastuszka
Role

Role są trzy - gosc, uzytkownik, administrator - tak jak opisano w modelu konceptualnym
Zmiany w zakresie przywilejów ról są niewielkie.

Jedyną znaczącą zmianą jest odebranie możliwości obejrzenia wartości krotek z relacji 'uzytkownik' na atrybucie 'haslo'.
Wszystko w imię bezpieczeństwa
*/
revoke all on kategoria, trasa, ocena, punkt, czas, wycieczka, uzytkownik, zapis, komentarz, wycieczka_id_wycieczka_seq,
	komentarz_modyfikacja, wycieczka_modyfikacja, trasa_id_trasa_seq, komentarz_id_komentarz_seq, czas_id_czas_seq,
	trasa_modyfikacja, kategoria_modyfikacja, uzytkownik_bez_hasla from gosc;
revoke all on kategoria, trasa, ocena, punkt, czas, wycieczka, uzytkownik, zapis, komentarz, wycieczka_id_wycieczka_seq,
	komentarz_modyfikacja, wycieczka_modyfikacja, trasa_id_trasa_seq, komentarz_id_komentarz_seq, czas_id_czas_seq,
	trasa_modyfikacja, kategoria_modyfikacja, uzytkownik_bez_hasla from uzytkownik;
revoke all on kategoria, trasa, ocena, punkt, czas, wycieczka, uzytkownik, zapis, komentarz, wycieczka_id_wycieczka_seq,
	komentarz_modyfikacja, wycieczka_modyfikacja, trasa_id_trasa_seq, komentarz_id_komentarz_seq, czas_id_czas_seq,
	trasa_modyfikacja, kategoria_modyfikacja, uzytkownik_bez_hasla from administrator;
drop role if exists gosc;
drop role if exists uzytkownik;
drop role if exists administrator;

--hasła oczywiście zostaną zmienione
create role gosc with password 'test' login;
grant select on kategoria, trasa, ocena, punkt, czas, wycieczka to gosc;
grant insert on uzytkownik to gosc;

--użytkownik został nieco okrojony jeśli chodzi o możliwość usuwania krotek (przywileje, których go pozbawiłem,
--pozostawiłem zakomentowane)
create role uzytkownik with password 'test' login;
grant select on kategoria, trasa, ocena, punkt, czas, wycieczka, uzytkownik_bez_hasla, zapis, komentarz,
	komentarz_modyfikacja, wycieczka_modyfikacja to uzytkownik;
grant select, usage on trasa_id_trasa_seq, komentarz_id_komentarz_seq, czas_id_czas_seq, wycieczka_id_wycieczka_seq to uzytkownik;
grant insert on kategoria, trasa, ocena, punkt, czas, wycieczka, zapis, komentarz to uzytkownik;
grant delete on /*uzytkownik, czas, komentarz,*/ zapis, wycieczka to uzytkownik;
grant update on uzytkownik_bez_hasla, komentarz_modyfikacja, wycieczka_modyfikacja to uzytkownik;

create role administrator with password 'test' login;
grant select, usage on trasa_id_trasa_seq, wycieczka_id_wycieczka_seq, czas_id_czas_seq, komentarz_id_komentarz_seq to administrator;
grant select, insert, delete on kategoria, trasa, ocena, punkt, czas, wycieczka, uzytkownik, zapis, komentarz, uzytkownik_bez_hasla,
	komentarz_modyfikacja, wycieczka_modyfikacja,
	trasa_modyfikacja, kategoria_modyfikacja to administrator;
grant update on uzytkownik_bez_hasla, komentarz_modyfikacja, wycieczka_modyfikacja,
	trasa_modyfikacja, kategoria_modyfikacja to administrator;
