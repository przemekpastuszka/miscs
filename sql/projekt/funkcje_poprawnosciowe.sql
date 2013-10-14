/*
Przemysław Pastuszka
Funkcje sprawdzające czy po wykonaniu danej akcji nie nastąpi naruszenie integralności więzów

Tu naprawdę nic się nie dzieje. Wszystkie funkcje zbudowane są wg prostego schematu:
-sprawdź integralność
-jeśli naruszona - rzuć wyjątek z odpowiednim komunikatem

Nazwy funkcji + treści komunikatów mówią wszystko

Ponadto można zauważyć, że niektóre funkcje wywoływane są przed wstawieniem krotki, a niektóre przed wstawieniem i aktualizacją.
W wypadku tych pierwszych sprawdzanie integralności więzów przed aktualizacją jest bezcelowe, gdyż nie może ona zostać naruszona
ze względu na fakt, iż aktualizację można wykonywać tylko na niektórych atrybutach (dostęp ograniczony za pomocą widoków)
*/
create or replace function wstaw_do_uzytkownik()
returns trigger as
$$
	begin
		if NEW.nick is null then
			raise exception 'Nick użytkownika nie może być pusty';
		end if;
		if NEW.haslo is null then
			raise exception 'Hasło użytkownika nie może być puste';
		end if;
		if exists (select * from uzytkownik where nick = NEW.nick) then
			raise exception 'Użytkownik o podanym nicku już istnieje, przepraszamy';
		end if;
		
		return NEW;
	end;
$$	--wszystkie funkcje posiadają atrybut 'security definer', gdyż czasem mogą wykonywać operacje, które z poziomu aktualnej
language plpgsql security definer; --roli/użytkownika nie są dozwolone

create trigger wstaw_do_uzytkownik before insert on uzytkownik
	for each row execute procedure wstaw_do_uzytkownik();
	
	
/*******************************************************************/
create or replace function wstaw_do_kategoria()
returns trigger as
$$
	begin
		if NEW.nazwa_kat is null then
			raise exception 'Nazwa kategorii nie może być pusta';
		end if;
		if NEW.nick is null then
			raise exception 'Kategoria nie może zostać utworzona w sposób anonimowy';
		end if;
		if exists (select * from kategoria where nazwa_kat = NEW.nazwa_kat) then
			raise exception 'Kategoria o podanej nazwie już istnieje, przepraszamy';
		end if;
		if not exists (select * from uzytkownik where nick = NEW.nick) then
			raise exception 'Użytkownik o podanym nicku nie istnieje';
		end if;
		return NEW;
	end;
$$
language plpgsql security definer;

create trigger wstaw_do_kategoria before insert on kategoria
	for each row execute procedure wstaw_do_kategoria();
	
	
/*******************************************************************/
--dla trasy podzieliłem funkcję na dwie części:
--	pierwsza sprawdza tylko i wyłącznie unikalność nazwy i jest odpalana tylko przy wstawianiu nowej krotki
-- 	druga sprawdza pozostałe więzy i jest odpalana zarówno przed wstawieniem, jak i aktualizacją krotki
--taki podział był konieczny ze względu na to, że w przypadku aktualizacji rzucany byłby błąd o braku unikalności nazwy
create or replace function wstaw_do_trasa()
returns trigger as
$$
	begin
		NEW.data_dodania := now();

		if exists (select * from trasa where nazwa = NEW.nazwa) then
			raise exception 'Trasa o podanej nazwie już istnieje, przepraszamy';
		end if;

		return NEW;
	end;
$$
language plpgsql security definer;

create or replace function wstaw_do_trasa2()
returns trigger as
$$
	begin
		if NEW.nazwa_kat is null then
			raise exception 'Trasa musi przynależeć do jednej z kategorii';
		end if;
		
		if NEW.nazwa is null then
			raise exception 'Nazwa trasy nie może być pusta';
		end if;
		
		--if NEW.nick is null then
		--	raise exception 'Trasa nie może zostać utworzona w sposób anonimowy';
		--end if;

		--if NEW.nick is null or not exists (select * from uzytkownik where nick = NEW.nick) then
		--	raise exception 'Użytkownik o podanym nicku nie istnieje';
		--end if;
		
		if not exists (select * from kategoria where nazwa_kat = NEW.nazwa_kat) then
			raise exception 'Kategoria o podanej nazwie nie istnieje';
		end if;
		return NEW;
	end;
$$
language plpgsql security definer;

create trigger wstaw_do_trasa before insert on trasa
	for each row execute procedure wstaw_do_trasa();
	
create trigger wstaw_do_trasa2 before insert or update on trasa
	for each row execute procedure wstaw_do_trasa2();
/*******************************************************************/
create or replace function wstaw_do_punkt()
returns trigger as
$$
	begin
		
		if NEW.id_trasa is null then
			raise exception 'Podany punkt musi należeć do pewnej trasy';
		end if;
		
		if NEW.numer is null then
			raise exception 'Punkt musi mieć określoną kolejność';
		end if;
		
		if exists (select * from punkt where id_trasa = NEW.id_trasa and numer = NEW.numer) then
			raise exception 'Podany punkt już istnieje, przepraszamy';
		end if;
		
		if not exists (select * from trasa where id_trasa = NEW.id_trasa) then
			raise exception 'Trasa o podanym numerze nie istnieje';
		end if;
		
		if NEW.szerokosc is null or NEW.dlugosc is null then
			raise exception 'Szerokość i długość geograficzna muszą zostać określone';
		end if;
		
		return NEW;
	end;
$$
language plpgsql security definer;

create trigger wstaw_do_punkt before insert on punkt
	for each row execute procedure wstaw_do_punkt();
	
	
/*******************************************************************/
create or replace function wstaw_do_wycieczka()
returns trigger as
$$
	begin
		if NEW.id_trasa is null then
			raise exception 'Wycieczka musi odnosić się do istniejącej trasy';
		end if;
		
		--if NEW.nick is null then
		--	raise exception 'Wycieczka nie może zostać utworzona anonimowo';
		--end if;
		
		if NEW.data is null then
			raise exception 'Data wycieczki musi zostać określona';
		end if;
		
		if not exists (select * from trasa where id_trasa = NEW.id_trasa) then
			raise exception 'Trasa o podanym numerze nie istnieje';
		end if;
		
		--if not exists (select * from uzytkownik where nick = NEW.nick) then
		--	raise exception 'Użytkownik o podanym nicku nie istnieje';
		--end if;
		
		if NEW.data <= now() then
			raise exception 'Data wycieczki musi być ściśle większa od aktualnej';
		end if;
		
		return NEW;
	end;
$$
language plpgsql security definer;

create trigger wstaw_do_wycieczka before insert or update on wycieczka
	for each row execute procedure wstaw_do_wycieczka();
	
/*******************************************************************/
create or replace function wstaw_do_zapis()
returns trigger as
$$
	begin
		if NEW.id_wycieczka is null then
			raise exception 'Zapis musi odnosić się do istniejącej wycieczki';
		end if;
		
		if NEW.nick is null then
			raise exception 'Zapis na wycieczkę nie może być anonimowy';
		end if;
		
		if not exists (select * from wycieczka where id_wycieczka = NEW.id_wycieczka) then
			raise exception 'Wycieczka o podanym numerze nie istnieje';
		end if;
		
		if not exists (select * from uzytkownik where nick = NEW.nick) then
			raise exception 'Użytkownik o podanym nicku nie istnieje';
		end if;
		
		if exists (select * from zapis where nick = NEW.nick and id_wycieczka = NEW.id_wycieczka) then
			raise exception 'Jesteś już zapisany na tę wycieczkę';
		end if;
		
		return NEW;
	end;
$$
language plpgsql security definer;

create trigger wstaw_do_zapis before insert on zapis
	for each row execute procedure wstaw_do_zapis();
	
/*******************************************************************/
create or replace function wstaw_do_czas()
returns trigger as
$$
	begin
		NEW.id_czas := nextval('czas_id_czas_seq');
		
		if NEW.id_trasa is null then
			raise exception 'Czas musi odnosić się do istniejącej trasy';
		end if;
		
		if NEW.nick is null then
			raise exception 'Czas przejechania trasy nie może być anonimowy';
		end if;
		
		if NEW.czas is null then
			raise exception 'Czas przejechania trasy nie może być pusty';
		end if;
		
		if not exists (select * from trasa where id_trasa = NEW.id_trasa) then
			raise exception 'Trasa o podanym numerze nie istnieje';
		end if;
		
		if not exists (select * from uzytkownik where nick = NEW.nick) then
			raise exception 'Użytkownik o podanym nicku nie istnieje';
		end if;
		
		return NEW;
	end;
$$
language plpgsql security definer;

create trigger wstaw_do_czas before insert on czas
	for each row execute procedure wstaw_do_czas();
	
/*******************************************************************/
--sytuacja podobna jak w przypadku trasy
create or replace function wstaw_do_komentarz()
returns trigger as
$$
	begin
		NEW.data = now();
		return NEW;
	end;
$$
language plpgsql security definer;

create or replace function wstaw_do_komentarz2()
returns trigger as
$$
	begin
		if NEW.id_trasa is null then
			raise exception 'Komentarz musi odnosić się do istniejącej trasy';
		end if;
		
		if NEW.nick is null then
			raise exception 'Komentarz nie może być anonimowy';
		end if;
		
		if NEW.tresc is null then
			raise exception 'Treść komentarza nie może być pusta';
		end if;
		
		if not exists (select * from trasa where id_trasa = NEW.id_trasa) then
			raise exception 'Trasa o podanym numerze nie istnieje';
		end if;
		
		if not exists (select * from uzytkownik where nick = NEW.nick) then
			raise exception 'Użytkownik o podanym nicku nie istnieje';
		end if;
		
		return NEW;
	end;
$$
language plpgsql security definer;

create trigger wstaw_do_komentarz before insert on komentarz
	for each row execute procedure wstaw_do_komentarz();
	
create trigger wstaw_do_komentarz2 before insert or update on komentarz
	for each row execute procedure wstaw_do_komentarz2();
/*******************************************************************/
create or replace function wstaw_do_ocena()
returns trigger as
$$
	begin
		
		if NEW.id_trasa is null then
			raise exception 'Ocena musi odnosić się do istniejącej trasy';
		end if;
		
		if NEW.nick is null then
			raise exception 'Ocena nie może być anonimowa';
		end if;
		
		if NEW.kat_oceny is null then
			raise exception 'Kategoria oceny nie może być pusta';
		end if;
		
		if NEW.ocena is null then
			raise exception 'Ocena nie może być pusta';
		end if;
		
		if not exists (select * from trasa where id_trasa = NEW.id_trasa) then
			raise exception 'Trasa o podanym numerze nie istnieje';
		end if;
		
		if not exists (select * from uzytkownik where nick = NEW.nick) then
			raise exception 'Użytkownik o podanym nicku nie istnieje';
		end if;
		
		if exists (select * from ocena where nick = NEW.nick and id_trasa = NEW.id_trasa
			and kat_oceny = NEW.kat_oceny) then
			raise exception 'Już oceniłeś tę trasę w tej kategorii';
		end if;
		
		if NEW.ocena < 1 or NEW.ocena > 6 then
			raise exception 'Ocena powinna mieścić się w przedziale 1 - 6';
		end if;
		
		return NEW;
	end;
$$
language plpgsql security definer;

create trigger wstaw_do_ocena before insert on ocena
	for each row execute procedure wstaw_do_ocena();
	
/*******************************************************************/
--jedyna funkcja wywoływana przed operacją usuwania
create or replace function usun_z_kategoria()
returns trigger as
$$
	begin
		if exists (select * from trasa where nazwa_kat = OLD.nazwa_kat) then
			raise exception 'Nie można usunąć kategorii, ponieważ istnieją związane z nią trasy';
		end if;
		
		return OLD;
	end;
$$
language plpgsql security definer;	

create trigger usun_z_kategoria before delete on kategoria
	for each row execute procedure usun_z_kategoria();
