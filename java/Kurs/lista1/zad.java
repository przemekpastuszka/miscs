import java.io.*;

class Program{
	
	private static final String[][] liczby = {
		{"", "jeden", "dwa", "trzy", "cztery", "pięć", "sześć", "siedem", "osiem", "dziewięć"},
		{"", "dziesięć", "dwadzieścia", "trzydzieści", "czterdzieści", "pięćdziesiąt", "siedemdziesiąt", "osiemdziesiąt", "dziewięćdziesiąt"},
		{"", "sto", "dwieście", "trzysta", "czterysta", "pięćset", "sześćset", "siedemset", "osiemset", "dziewięćset"}
	};
	
	private static final String[] nastki ={
		"", "jedenaście", "dwanaście", "trzynaście", "czternaście", "piętnaście", "szesnaście", "siedemnaście", "osiemnaście", "dziewiętnaście"
	};
	
	public static void main(String[] args){
		BufferedReader wejscie = null;
		PrintWriter wyjscie = null, bledy = null;
	
		try{
			wejscie = new BufferedReader(new InputStreamReader(System.in, "ISO8859_2"));
			wyjscie = new PrintWriter(new OutputStreamWriter(System.out, "ISO8859_2"), true);
			bledy = new PrintWriter(new OutputStreamWriter(System.out, "ISO8859_2"), true);
		
			while(true){
				try{
					int x = Integer.parseInt(wejscie.readLine().trim());
					
					if(x >= 1000 || x <= -1000)
						throw new NumberFormatException();
					
					if(x == 0){
						wyjscie.println("zero");
						continue;
					}

					if( x % 100 < 20 && x % 100 > 10){
						wyjscie.println(liczby[2][(x / 100) % 10] + " " + nastki[x % 10]);
						continue;
					}
					
					String wynik = "";
					for(int i = 0; x != 0; x /= 10, ++i){
						if(x % 10 == 0)
							continue;
						wynik = liczby[i][x % 10] + " " + wynik;
					}
					wyjscie.println(wynik);
				}
				catch (NumberFormatException ex){
					bledy.println("Błędna liczba!");
				}
				catch (NullPointerException ex){
					break;
				}
				catch(IOException ex){
					break;
				}
			}
		}
		catch(IOException ex){
		}
		finally{
			try{
				if(wejscie != null)
					wejscie.close();
				if(wyjscie != null)
					wyjscie.close();
				if(bledy != null)
					bledy.close();
			}
			catch(IOException e){
				System.exit(1);
			}
		}
	}
}
