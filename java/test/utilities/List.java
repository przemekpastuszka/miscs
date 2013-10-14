package utilities;

public class List<T>{

	private static class Node<T>{
		public T element;
		public Node<T> next, previous;
	}
	
	private Node<T> head, tail;
	
	public void pushFront(T element){
		Node<T> temp = new Node<T>();
		temp.element = element;
		temp.next = head;
		if(head != null)
			head.previous = temp;
		head = temp;
		if(tail == null)
			tail = head;
	}
	
	public void pushBack(T element){
		Node<T> temp = new Node<T>();
		temp.element = element;
		temp.previous = tail;
		if(tail != null)
			tail.next = temp;
		tail = temp;
		if(head == null)
			head = tail;
	}
	
	public T getFront() throws EmptyListException{
		if(head == null)
			throw new EmptyListException();
		return head.element;
	}
	
	public T getBack() throws EmptyListException{
		if(tail == null)
			throw new EmptyListException();
		return tail.element;
	}
	
	public void popFront(){
		if(head == tail)
			head = tail = null;
		else
			head = head.next;
	}
	
	public void popBack(){
		if(head == tail)
			head = tail = null;
		else
			tail = tail.previous;
	}
}
