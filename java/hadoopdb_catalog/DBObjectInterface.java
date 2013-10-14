/*
* Author: Przemysław Pastuszka
* Institute of Computer Science, Wrocław University, Poland
*/

// A class which implements DBObjectInterface can 
// take advantage of safety and laziness of DBObject

public interface DBObjectInterface{
    public DBObjectType getType();
    public String getId();
    public void load();
    public void delete();
}
