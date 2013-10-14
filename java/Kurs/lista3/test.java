import narzedzia.Lista;

class Test{
	public static void main(String[] args){
		Lista a = new Lista();
		a.insert(5);
		Lista b = new Lista();
		b.insert(5);
		System.out.println(a.equals(b));
	}
}
