package narzedzia;

class Wezel implements Cloneable{
	protected double wart;
	protected Wezel nast;
	
	public Wezel clone(){
		Wezel nowy = new Wezel();
		nowy.wart = wart;
		nowy.nast = nast == null ? null : nast.clone();
		return nowy;
	}
	
	public boolean equals(Object ob){
		if(ob instanceof Wezel){
			Wezel t = (Wezel) ob;
			if((t.nast == null && nast != null) || (t.nast != null && nast == null))
				return false;
			return t.wart == wart && (nast == null ? true : nast.equals(t.nast));
		}
		else{
			return false;
		}
	}
	
	public String toString(){
		return wart + (nast == null ? "" : ", " + nast.toString());
	}
	
	public void insert(double w){
		if(nast == null || nast.wart >= w){
			Wezel nowy = new Wezel();
			nowy.wart = w;
			nowy.nast = nast;
			nast = nowy;
		}
		else{
			nast.insert(w);
		}
	}
	
	public boolean search(double w){
		if(wart == w)
			return true;
		else
			return nast == null ? false : nast.search(w);
	}
	
	public void delete(double w){
		if(nast == null)
			return;
		if(nast.wart == w)
			nast = nast.nast;
	}
}

public class Lista implements Cloneable{
	protected Wezel glowa;
	
	public Lista clone(){
		Lista nowa = new Lista();
		nowa.glowa = glowa == null ? null : glowa.clone();
		return nowa;
	}
	
	public boolean equals(Object ob){
		if(ob instanceof Lista){
			Lista t = (Lista) ob;
			if(glowa == null)
				return t.glowa == null;
			if(t.glowa == null)
				return glowa == null;
			return glowa.equals(t.glowa);
		}
		else
			return false;
	}
	
	public String toString(){
		return "{" + (glowa == null ? "" : glowa) + "}";
	}
	
	public void insert(double w){
		if(glowa == null || glowa.wart >= w){
			Wezel nowy = new Wezel();
			nowy.wart = w;
			nowy.nast = glowa;
			glowa = nowy;
		}
		else{
			glowa.insert(w);
		}
	}
	
	public void delete(double w){
		if(glowa == null)
			return;
		if(glowa.wart == w)
			glowa = glowa.nast;
		else
			glowa.delete(w);
	}
	
	public boolean search(double w){
		return glowa == null ? false : glowa.search(w);
	}
}
