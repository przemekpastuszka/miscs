import utilities.List;

class Program {
    public static void main(String[] args) {
		List<Integer> lista = new List<Integer>();
		lista.pushBack(100);
		lista.pushFront(11);
		lista.pushBack(123);
		try{
			System.out.println(lista.getFront());
			System.out.println(lista.getBack());
			lista.popFront();
			System.out.println(lista.getFront());
		}
		catch(utilities.EmptyListException e){
			System.err.println("FUCK");
		}
    }
}

