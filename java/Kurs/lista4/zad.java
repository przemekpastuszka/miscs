//TU SĄ BŁĘĘĘDYYYYY!!!!
//NIE PATRZEĆ NAWET

import narzedzia.*;

class PrawdziwaOsoba implements Osoba{
	private String imie, nazwisko;
	private long pesel;
	
	public PrawdziwaOsoba(String imie, String nazwisko, long pesel){
		this.imie = imie;
		this.nazwisko = nazwisko;
		this.pesel = pesel;
	}
	
	public String podajImie() {return imie;}
	public String podajNazwisko() {return nazwisko;}
	public long podajPesel() {return pesel;}
	
	public int compareTo(Osoba os){
		int p = nazwisko.compareTo(os.podajNazwisko());
		if(p != 0)
			return p;
		p = imie.compareTo(os.podajImie());
		if(p != 0)
			return p;
		if(pesel < os.podajPesel())
			return -1;
		return pesel == os.podajPesel() ? 0 : 1;
	}
	
	public boolean equals(Object ob){
		if(ob instanceof Osoba){
			Osoba os = (Osoba) ob;
			return this.compareTo(os) == 0;
		}
		else
			return false;
	}
	
	public String toString(){
		return imie + " " + nazwisko + ": " + pesel;
	}
}

class TablicaDynamiczna implements ZbiorOsob{
	private Osoba[] osoby;
	private int pojemnosc, iloscElementow;
	
	private static final int startowaPojemnosc = 15;
	
	private void inicjalizuj(int wymaganaPojemnosc){
		wymaganaPojemnosc = wymaganaPojemnosc > 0 ? wymaganaPojemnosc : 1;
		osoby = new Osoba[wymaganaPojemnosc];
		pojemnosc = wymaganaPojemnosc;
		iloscElementow = 0;
	}
	
	public TablicaDynamiczna(int wymaganaPojemnosc){
		inicjalizuj(wymaganaPojemnosc);
	}
	
	public TablicaDynamiczna(){
		inicjalizuj(startowaPojemnosc);
	}

	
	private void zwieksz(){
		if(iloscElementow < pojemnosc)
			return;
		Osoba[] temp = new Osoba[pojemnosc * 2];
		for(int i = 0; i < pojemnosc; ++i)
			temp[i] = osoby[i];
		pojemnosc *= 2;
		osoby = temp;
	}
	
	private void zmniejsz(){
		if(iloscElementow >= pojemnosc / 4)
			return;
		Osoba[] temp = new Osoba[pojemnosc / 2];
		for(int i = 0; i < iloscElementow; ++i)
			temp[i] = osoby[i];
		pojemnosc /= 2;
		osoby = temp;
	}
	
	private static class TakaOsoba implements Osoba{
		public String imie, nazwisko;
		public String podajImie() {return imie;}
		public String podajNazwisko() {return nazwisko;}
		public long podajPesel() { return 0;}
		
		public int compareTo(Osoba os){
			int p = nazwisko.compareTo(os.podajNazwisko());
			if(p != 0)
				return p;
			return imie.compareTo(os.podajImie());
		}
	}
	
	private static class Para{
		public boolean czyZnaleziono;
		public int pozycja;
	}
	
	private Para znajdzPozycje(Osoba os){
		int p = 0, q = iloscElementow - 1;
		Para wynik = new Para();
		wynik.pozycja = 0;
		wynik.czyZnaleziono = false;
		while(p <= q){
			if(p == q){
				int temp = os.compareTo(osoby[p]);
				wynik.pozycja = p + temp > 0 ? 1 : 0;
			}
			int srodek = (p + q) / 2;
			int porownanie = os.compareTo(osoby[srodek]);
			if(porownanie == 0){
				wynik.pozycja = srodek;
				wynik.czyZnaleziono = true;
				break;
			}
			if(porownanie > 0)
				p = srodek + 1;
			else
				q = srodek - 1;
		}
		return wynik;
	}
	
	public Osoba[] znajdz(String imie, String nazwisko){
		TakaOsoba os = new TakaOsoba();
		os.imie = imie;
		os.nazwisko = nazwisko;
		Para p = znajdzPozycje(os);
		if(!p.czyZnaleziono)
			return null;
		int min = p.pozycja - 1, max = p.pozycja + 1;
		while(min >= 0 && os.compareTo(osoby[min]) == 0)
			--min;
		while(max < iloscElementow && os.compareTo(osoby[min]) == 0)
			++max;
		Osoba[] wynik = new Osoba[max - min - 1];
		for(int i = min + 1; i < max - min; ++i)
			wynik[i] = osoby[i];
		return wynik;
	}
	
	public void wstaw(Osoba os){
		zwieksz();
		Para p = znajdzPozycje(os);
		for(int i = iloscElementow - 1; i >= p.pozycja; --i)
			osoby[i + 1] = osoby[i];
		osoby[p.pozycja] = os;
		++iloscElementow;
	}
	
	public void usun(long pesel){
	}
	
	public void wypiszWszystkie(){
		for(int i = 0; i < iloscElementow; ++i)
			System.out.println(osoby[i]);
	}
}

class Program{
	public static void main(String[] args){
		TablicaDynamiczna zbior = new TablicaDynamiczna();
		zbior.wstaw(new PrawdziwaOsoba("Alfons", "Krawat", 112));
		zbior.wstaw(new PrawdziwaOsoba("Konrad", "Zekrzak", 11));
		zbior.wstaw(new PrawdziwaOsoba("Marian", "Jebomir", 320));
		zbior.wstaw(new PrawdziwaOsoba("Alfons", "Krawat", 97));
		zbior.wstaw(new PrawdziwaOsoba("Alfons", "Krawat", 907)); //tu się objawia jakiś błąd!!
		System.out.println((new PrawdziwaOsoba("Alfons", "Krawat", 907)).compareTo(new PrawdziwaOsoba("Marian", "Jebomir", 320)));
		zbior.wypiszWszystkie();
	}
}
