/*
Przemysław Pastuszka
Schemat bazy danych

W zasadzie jest to dosłowna implementacja modelu konceptualnego - nie zaszły żadne zmiany.
Jedyne nowości to - dodanie dziedziny kat_oceny oraz indeksu (na końcu pliku).
Dlatego komentarze we właściwym kodzie ograniczone są do minimum.

Dodatkowego pola "średnia ocena" w tabeli trasa jednak nie dodałem. A to z kilku powodów:
-primo: należałoby dodać jedno pole "średnia ocena" dla każdej kategorii. "Zhardcodowanie" tego
	w schemacie tabeli psuje elastyczność bazy - dodanie nowej kategorii wymagałoby również
	modyfikacji schematu tabeli, dodania nowej funkcji itede...
-secundo: myślałem nad innym rozwiązaniem problemu - tj. stworzeniem odrębnej relacji, która miałaby
	tylko trzy atrybuty: id_trasa, kat_oceny i srednia_ocena. W ten sposób zachowałbym co prawda elastyczność
	w dodawaniu kategorii, ale są i minusy - w bazie pojawia się więcej zależności i jest ogółem brzydko
-tertio: mała modyfikacja powyższego pomysłu - wprowadzamy powyższą relację kosztem relacji 'ocena'. Tym samym
	ilość zależności pozostaje na niezmienionym poziomie i w ogóle jest prawie fajnie. Problem jednak tkwi gdzie indziej -
	otóż pozbywając się tabeli ocena tracimy cenne informacje na temat: kto i jak zagłosował na daną trasę.
	Dane tego typu, w trakcie przeprowadzania czystki wśród użytkowników, są nieocenione ;)
-ileś tam: w bazie nigdy nie pojawi się wiele ocen ze względu na fakt, iż dodawać je mogą tylko zarejestrowani
	użytkownicy, a każdy może to uczynić co najwyżej raz. I chociaż tras może być wiele i kategorii też -
	do 20 000 nijak liczba wierszy w relacji 'ocena' nie dojdzie
*/
drop table if exists uzytkownik cascade;
drop table if exists wycieczka cascade;
drop table if exists zapis cascade;
drop table if exists punkt cascade;
drop table if exists trasa cascade;
drop table if exists kategoria cascade;
drop table if exists ocena cascade;
drop table if exists komentarz cascade;
drop table if exists czas cascade;

drop domain if exists kat_oceny cascade;
create domain kat_oceny as varchar(3) --4 kategorie, o których wspomniałem w modelu konceptualnym
	check (value in ('stt', 'tpr', 'we', 'oo'));

create table uzytkownik
(
	nick varchar(15),
	nazwisko varchar(20),
	imie varchar(15),
	opis text,
	rodzaj_roweru varchar(50),
	haslo character(40) not null, --sha1 ma długość 40 znaków
	constraint uz_pkey primary key (nick)
);

create table kategoria
(
	nazwa_kat varchar(50),
	nick varchar(20),
	opis text,
	zdjecie varchar(44), --przy uploadzie nowego zdjęcia, w celu uniknięcia nadpisania, zmieniamy mu nazwę na
		--(sha1 (now())).x, gdzie x należy do {png, gif, jpg}. Stąd biorą się 44 znaki
		
	constraint kat_pkey primary key (nazwa_kat),
	constraint fk_nick foreign key (nick)
		references uzytkownik (nick)
		on delete set null
);

create table trasa
(
	id_trasa serial,
	nazwa_kat varchar(50) not null,
	nazwa varchar(50) not null unique, --o tym nie wspomniałem w modelu konceptualnym, jednak nazwa trasy też powinna być unikalna
	opis text,
	nick varchar(15),
	data_dodania timestamp not null default now(),
	zdjecie varchar(44), --patrz 'zdjecie' w 'kategoria'
	
	constraint trasa_pkey primary key (id_trasa),
	constraint fk_nick foreign key (nick)
		references uzytkownik (nick)
		on delete set null,
	constraint fk_kat foreign key (nazwa_kat)
		references kategoria (nazwa_kat)
		on delete restrict --nie wolno usuwać kategorii, do których przypisane są trasy
);

create table punkt
(
	id_trasa int,
	numer int,
	szerokosc real not null,
	dlugosc real not null,
	
	constraint punkt_pkey primary key (id_trasa, numer),
	constraint fk_id_trasa foreign key (id_trasa)
		references trasa (id_trasa)
		on delete cascade
);

create table wycieczka
(
	id_wycieczka serial,
	nick varchar(15),
	id_trasa int not null,
	data timestamp not null check (data > now()), --sprawdzenie poprawności daty
	opis text,
	
	constraint wycieczka_pkey primary key (id_wycieczka),
	constraint fk_id_trasa foreign key (id_trasa)
		references trasa (id_trasa)
		on delete cascade,
	constraint fk_nick foreign key (nick)
		references uzytkownik (nick)
		on delete set null
);

create table zapis
(
	id_wycieczka int,
	nick varchar(15),
	
	constraint zapis_pkey primary key (id_wycieczka, nick),
	constraint fk_id_wycieczka foreign key (id_wycieczka)
		references wycieczka (id_wycieczka)
		on delete cascade,
	constraint fk_nick foreign key (nick)
		references uzytkownik (nick)
		on delete cascade
);

create table czas
(
	id_czas serial,
	nick varchar(15) not null,
	id_trasa int not null,
	czas interval not null, --tu akurat nie jestem przekonany co do wyboranego typu (może się zmienić na real)
	
	constraint czas_pkey primary key (id_czas),
	constraint fk_nick foreign key (nick)
		references uzytkownik (nick)
		on delete cascade,
	constraint fk_trasa foreign key (id_trasa)
		references trasa (id_trasa)
		on delete cascade
);

create table komentarz
(
	id_komentarz serial,
	nick varchar(15),
	id_trasa int not null,
	tresc text not null,
	data timestamp not null default now(),
	
	constraint komentarz_pkey primary key (id_komentarz),
	constraint fk_nick foreign key (nick)
		references uzytkownik (nick)
		on delete set null,
	constraint fk_trasa foreign key (id_trasa)
		references trasa (id_trasa)
		on delete cascade
);

create table ocena
(
	nick varchar(15),
	id_trasa int,
	kat_oceny kat_oceny,
	ocena smallint not null check (ocena between 1 and 6),
	
	constraint ocena_pkey primary key (nick, id_trasa, kat_oceny),
		constraint fk_nick foreign key (nick)
		references uzytkownik (nick)
		on delete cascade,
	constraint fk_trasa foreign key (id_trasa)
		references trasa (id_trasa)
		on delete cascade
);

--będziemy często chcieli wyświetlać trasy należące do danej kategorii, więc taki indeks się przyda
create index trasa_kat_nazwa on trasa (nazwa_kat);
