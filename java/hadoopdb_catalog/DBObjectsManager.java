/*
* Author: Przemysław Pastuszka
* Institute of Computer Science, Wrocław University, Poland
*/

import java.util.HashMap;

// DBObjectManager is provided to store references to DBObjects.
// Again: it has two advantages, when accessing DBObjects:
// - new lookup to catalog database is not necessary (speed improvement)
// - only one instance of DBObject exists in memory (saves memory)

public class DBObjectsManager{
    private static HashMap<String, DBObject> objects;
    
    @SuppressWarnings("unchecked")
    public static <T extends DBObjectInterface> DBObject<T> getExistingObject(String id, DBObjectType type){
        
        if(objects.containsKey(id)){ // object is already in memory
            DBObject t = objects.get(id);
            if(t.getType() != type){ // type of object in memory differs from type of requested object
                // throw some exception
            }
            return t;
        }
        else{
            // create DBObject using factory to keep extensibility
            DBObject<T> t = new DBObject<T>((T) DBObjectsFactory.create(id, type));
            objects.put(id, t);
            return t;
        }
    }
    
    public static void removeObject(String id){
        if(objects.containsKey(id)){
            // objects.get(id).delete();
            objects.remove(id);
        }
    }
}
