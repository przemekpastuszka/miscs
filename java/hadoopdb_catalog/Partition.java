/*
* Author: Przemysław Pastuszka
* Institute of Computer Science, Wrocław University, Poland
*/

import java.util.LinkedHashMap;

// Stores information about partition

public class Partition implements DBObjectInterface{
    protected String id;
    
    protected LinkedHashMap<String, DBObject<Node>> nodes;
    protected DBObject<Relation> relation;
    protected String partitioningValue;
    protected long size; // estimated size in kilobytes
    protected long nrOfRows;
    
    public Partition(String id){
        this.id = id;
    }
    
    public DBObjectType getType() { return DBObjectType.PARTITION; }
    public String getId() { return id; }
    public void load(){
        // Here's what this method does:
        // - query catalog database with: 'select * from Partition where partition_id = id'
        // - fill relation, size, nrOfRows and partitionigValue with data from database
        // - query catalog database again with: 'select node_id from Replica where partition_id = id'
        // - for each node_id:
        // -    nodes.put(node_id, DBObjectsManager.getExistingObject(node_id, NODE));
    }

    public void delete(){
        // - for each node in nodes:
        // -    connect to node database and delete corresponding relation (if node is unreachable do nothing)
        // - delete entries from Partition and Replica relations in catalog database
    }
    
    public LinkedHashMap<String, DBObject<Node>> getNodes(){
        return nodes;
    }
    public DBObject<Relation> getRelation() { return relation; }
    public String getPartitioningValue() { return partitioningValue; }
}
