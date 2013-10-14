/*
* Author: Przemysław Pastuszka
* Institute of Computer Science, Wrocław University, Poland
*/

// DBObjectFactory makes project extensible. For example if one wants to store more informations about
// partition, it's suffice to define new class A (which inherits from Partition) and
// change line:
// case PARTITION: return new Partition(id);
// into:
// case PARTITION: return new A(id);

public class DBObjectsFactory{
    
    public static DBObjectInterface create(String id, DBObjectType type){
        switch(type){
            case PARTITION: return new Partition(id);
            case RELATION: return new Relation(id);
            case NODE: return new Node(id);
        }
        return null;
    }
}
