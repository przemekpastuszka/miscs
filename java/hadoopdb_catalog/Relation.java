/*
* Author: Przemysław Pastuszka
* Institute of Computer Science, Wrocław University, Poland
*/

import java.util.LinkedHashMap;

// Stores informations about relation

public class Relation implements DBObjectInterface{
    protected String id;
    
    protected Attribute partitioningAttribute;
    protected LinkedHashMap<String, DBObject<Partition>> partitions;
    protected LinkedHashMap<String, Attribute> attributes;
    protected long size; // estimated size in kilobytes
    protected long nrOfRows;
    
    public Relation(String id){
        this.id = id;
    }
    
    public DBObjectType getType() { return DBObjectType.RELATION; }
    public String getId() { return id; }
    public void load(){
        // perform operations similar to load() method in Node and Partition
    }
    public void delete(){
        for(DBObject<Partition> obj : partitions.values()){
            obj.delete();
        }
        // delete corresponding entries from Relation and Attribute relations in catalog database
    }
    
    public LinkedHashMap<String, DBObject<Partition>> getPartitions(){
        return partitions;
    }
    public LinkedHashMap<String, Attribute> getAttributes(){
        return attributes;
    }
    public Attribute getPartitioningAttribute() { return partitioningAttribute; }
}
