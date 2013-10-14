/*
* Author: Przemysław Pastuszka
* Institute of Computer Science, Wrocław University, Poland
*/

import java.util.Calendar;

// DBObject is a wrapper for classes implementing DBObjectInterface. This solution has 2 main advantages:
// - Safety: DBObject always ensures, that data in the underlying object is valid 
// - Speed: DBObject is lazy - it queries catalog database only if something has changed and only for chosen object.

public class DBObject<T extends DBObjectInterface>{
    protected long lastModificationTimestamp; // information about the time of last synchronization with catalog database
    protected boolean valid = true; // DBObject becomes invalid, when underlying resource is deleted
    protected T object;
    
    public DBObject(T object){
        this.object = object;
        lastModificationTimestamp = 0; // 0 value ensures that object will be loaded from catalog database, when accessed for the first time
    }
    
    // every time when client wants to invoke any method of Node, Partition or Relation, he or she should follow the convention:
    // dbobject.getObject().methodToBeInvoked(args)
    // it'll ensure, that data he / she gets is synchronized with catalog database and valid
    public T getObject(){
        if(!valid){
            // clients requested access to non-existing object.
            // an exception should be thrown here
        }
        if(lastModificationTimestamp <= DBManager.getModificationTimestamp()){ // has anything changed in catalog database since loading the object
            try{
                object.load(); // if so, reload it.
            }
            catch(Exception e){ // problem occured while interacting with database (in final code Exception should be changed to something more specific)
                valid = false;
                // throw some exception
            }
            lastModificationTimestamp = Calendar.getInstance().getTime().getTime(); // update timestamp
        }
        return object;
    }
    
    public DBObjectType getType() { return object.getType(); }
    public boolean isValid() { return valid; }
    
    public void delete(){
        if(valid){
            valid = false;
            this.getObject().delete(); // delete underlying object
            DBObjectsManager.removeObject(object.getId()); // delete reference from ObjectsManager
        }
    }
}
