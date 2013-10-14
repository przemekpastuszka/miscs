import narzedzia.Lista;
import java.io.*;

class Program{

	private static class Para{
		public String nazwa;
		public Lista lista; 
	}
	
	public static Lista find(String nazwa, Para[] listy){
		for(Para i : listy){
			if(i.nazwa.equals(nazwa))
				return i.lista;
		}
		return null;
	}
	
	public static boolean execute(String[] komendy, Para[] listy) throws NumberFormatException{
		if(komendy.length == 3 && komendy[0].equals("insert")){
			find(komendy[1], listy).insert(Double.parseDouble(komendy[2]));
		}
		else if(komendy.length > 0 && komendy[0].equals("list")){
			if(komendy.length == 1){
				for(Para i : listy){
					System.out.println(i.nazwa + ": " + i.lista);
				}
			} 
			else{
				System.out.println(find(komendy[1], listy));
			}
		}
		else if(komendy.length == 3 && komendy[0].equals("clone")){
			for(Para i : listy){
				if(i.nazwa.equals(komendy[2]))
					i.lista = find(komendy[1], listy).clone();
			}
		}
		else if(komendy.length == 3 && komendy[0].equals("search")){
			System.out.println(find(komendy[1], listy).search(Double.parseDouble(komendy[2])));
		}
		else if(komendy.length == 3 && komendy[0].equals("delete")){
			find(komendy[1], listy).delete(Double.parseDouble(komendy[2]));
		}
		else if(komendy.length > 0 && komendy[0].equals("quit")){
			return true;
		}
		else{
			System.out.println("?!?");
		}
		return false;
	}
	
	public static void main(String[] args){
		if(args.length <= 0){
			System.err.println("java Program <names list>");
			System.exit(0);
		}
		Para[] listy = new Para[args.length];
		for(int i = 0; i < args.length; ++i){
			listy[i] = new Para();
			listy[i].nazwa = args[i];
			listy[i].lista = new Lista();
		}
		
		BufferedReader wejscie = null;
		wejscie = new BufferedReader(new InputStreamReader(System.in));
		
		while(true){
			System.out.print("komenda: ");
			try{
				String s = wejscie.readLine().trim();
				String[] komendy = s.split("\\s+");

				if(execute(komendy, listy))
					break;
			}
			catch(NumberFormatException e){
				System.out.println("?!?");
			}
			catch(NullPointerException e){
				break;
			}
			catch(IOException e){
				break;
			}
		}
		
		try{
			if(wejscie != null)
				wejscie.close();
		}
		catch(IOException e){
			System.exit(1);
		}
	}
}
