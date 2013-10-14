--zad1
drop type plan cascade;
create type plan as (nazwa text, rodzaj character(1), nazwisko varchar(30), termin character(13));

create or replace function plan_zajec(nr_sali grupa.sala%TYPE, semestr_id semestr.semestr_id%TYPE)
returns setof plan as
$$
	declare temp semestr.semestr_id%TYPE;
	begin
		temp := semestr_id;
		return query
			select przedmiot.nazwa, grupa.rodzaj_zajec, uzytkownik.nazwisko, grupa.termin from
			grupa join przedmiot_semestr using (kod_przed_sem)
			join przedmiot using (kod_przed)
			join uzytkownik using (kod_uz)
			where grupa.sala = nr_sali and przedmiot_semestr.semestr_id = temp;
		return;
	end;
$$
language plpgsql;

select distinct grupa.sala from grupa where not exists
	(select * from plan_zajec(sala, 28) where termin like '1:%');



--zad3
create or replace function przenies_do_oczekujacych()
returns trigger as
$$
	declare wolne int; max_os int; grupa_zapasowa int; nowa wybor;
	begin
		select max_osoby into max_os from grupa where grupa.kod_grupy = NEW.kod_grupy;
		select max_os - count(distinct wybor.kod_uz) into wolne
			from wybor where kod_grupy = NEW.kod_grupy;
		if wolne > 0 then return NEW;
		else
			select kod_grupy into grupa_zapasowa from grupa
				where kod_uz = 2341 and kod_przed_sem in
					(select kod_przed_sem from grupa where kod_grupy = NEW.kod_grupy);
			if grupa_zapasowa is null then return null;
			else
				select max_osoby into max_os from grupa where grupa.kod_grupy = grupa_zapasowa;
				select max_os - count(distinct wybor.kod_uz) into wolne
					from wybor where kod_grupy = grupa_zapasowa;
				if wolne = 0 then
					update grupa set max_osoby = max_osoby + 1 where kod_grupy = grupa_zapasowa;
				end if;
				nowa.kod_uz := NEW.kod_uz;
				nowa.kod_grupy := grupa_zapasowa;
				nowa.data := NEW.data;
				return nowa;
			end if;
		end if;
	end;
$$
language plpgsql;

create trigger zapis_z_limitem before insert or update on wybor
	for each row execute procedure przenies_do_oczekujacych();



--zad2
drop table wypis cascade;
create table wypis
(
  kod_grupy integer NOT NULL,
  kod_uz integer NOT NULL,
  data timestamp with time zone NOT NULL DEFAULT now()
);

create or replace function zapisz_wypis()
returns trigger as
$$
	begin
		insert into wypis values (OLD.kod_grupy, OLD. kod_uz, now());
		return null;
	end;
$$
language plpgsql;

create trigger przy_usuwaniu_wyboru after delete on wybor
for each row execute procedure zapisz_wypis();


--zad4
create or replace function pierwsi_spoznieni(kod_grupy grupa.kod_grupy%TYPE, k int)
returns setof uzytkownik.kod_uz%TYPE as
$$
	declare t wybor; temp grupa.kod_grupy%TYPE; max_os int; it int; j int; ok int;
	begin
		temp := kod_grupy;
		it := 0;
		j := 0;
		ok := 0;
		if k < 0 then
			return;
		end if;
		select max_osoby into max_os from grupa where grupa.kod_grupy = temp;
		for t in
			(select * from wybor where wybor.kod_grupy = temp order by data)
		loop
			if it = max_os then
				ok := 1;
			end if;
			it := it + 1;
		
			if j = k then
				exit;
			end if;
			
			if ok = 1 then
				return next t.kod_uz;
				j := j + 1;
			end if;
		end loop;
		return;
	end;
$$
language plpgsql;

select distinct nazwa from przedmiot join przedmiot_semestr using (kod_przed)
	join grupa using (kod_przed_sem) where
	semestr_id = 28 and exists (select pierwsi_spoznieni(kod_grupy, 1));
