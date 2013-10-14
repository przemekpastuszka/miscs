package narzedzia;

public interface ZbiorOsob{
	public void wstaw(Osoba os);
	public void usun(long pesel);
	public Osoba[] znajdz(String imie, String nazwisko);
}
