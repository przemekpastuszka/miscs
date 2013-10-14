/*
Przemysław Pastuszka
Pozostałe funkcje
*/

--zdecydowałem, że nie będę tworzył dodatkowej roli, która wspomagałaby logowanie użytkownika
--zamiast tego poniższa funkcja będzie sprawdzać hasło użytkownika
create or replace function sprawdz_haslo(n uzytkownik.nick%TYPE, h uzytkownik.haslo%TYPE)
returns boolean as
$$
	begin
		if exists (select * from uzytkownik where nick = n and haslo = h) then
			return true;
		else
			return false;
		end if;
	end;
$$
language plpgsql security definer;
