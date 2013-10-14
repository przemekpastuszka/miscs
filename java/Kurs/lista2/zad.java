import java.io.*;
import narzedzia.LiczbyPierwsze;	
	
class Program{
	public static void main(String[] args){
		if(args.length <= 0){
			System.err.println("java Program <number list>");
		}
		for(String num : args){
			try{
				long x = Long.parseLong(num);
				long[] czynniki = LiczbyPierwsze.naCzynnikiPierwsze(x);
				System.out.print(num + " =");
				for(long i : czynniki){
					System.out.print(" * " + i);
				}
				System.out.println("");
			}
			catch(NumberFormatException ex){
				System.err.println("Bledna liczba");
			}
		}
	}
}
