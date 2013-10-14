package narzedzia;
import java.lang.Math;

public class LiczbyPierwsze{
	protected final static int POTEGA2 = 21 ;
	protected final static long[] SITO = new long[1<<POTEGA2];
	protected final static int maks = 1 << POTEGA2;
	
	static{
		SITO[0] = SITO[1] = 0;
		
		for(int i = 2; i < maks; ++i){
			SITO[i] = i;
		}
		
		for(int i = 2; i < maks; ++i){
			if(SITO[i] != i)
				continue;
			for(int j = 2 * i; j < maks; j += i){
				if(SITO[j] == j){
					SITO[j] = i;
				}
			}
		}
	}

	public static boolean czyPierwsza (long x){
		return true;
	}
	public static long[] naCzynnikiPierwsze (long x){
		long temp[] = new long[64];
		
		int i = 0;
		for(; x != 1; x /= temp[i++]){
			temp[i] = 0;
			if(x < maks){
				temp[i] = SITO[(int)x];
			}
			else{
				double pierwiastek = Math.sqrt(x);
				for(long j = 2; j < pierwiastek; ++j){
					if(x % j == 0){
						temp[i] = j;
						break;
					}
				}
				if(temp[i] == 0){
					temp[i] = x;
				}
			}
		}
		long wynik[] = new long[i];
		System.arraycopy(temp, 0, wynik, 0, i);
		return wynik;
	}
	
	private LiczbyPierwsze() {}
}
