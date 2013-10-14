/*
* Author: Przemysław Pastuszka
* Institute of Computer Science, Wrocław University, Poland
*/

import java.util.LinkedHashMap;

// Stores information about one data node

public class Node implements DBObjectInterface{
    protected String id;
    
    protected String address, url, password, user, driver; // accessing data
    protected LinkedHashMap<String, DBObject<Partition>> partitions; // which partitions are stored in the node
    protected boolean unreachable; // node is unreachable if 'heartbeat' message has not been recieved for some specified time
    
    public Node(String id){
        this.id = id;
    }
    
    public DBObjectType getType() { return DBObjectType.NODE; }
    public String getId() { return id; }
    public void load(){
        // Here's what this method does:
        // - query catalog database with: 'select * from Node where node_id = id'
        // - fill address, url, password, user, driver with data from database
        // - query catalog database again with: 'select partition_id from Replica where node_id = id'
        // - for each partition_id:
        // -    partitions.put(partition_id, DBObjectsManager.getExistingObject(partition_id, PARTITION));
    }
    
    // When a node becomes deleted or unavailable there is a risk, that system may lost access to some data connected with partition
    // This method checks if that problem occured and alerts client
    protected void checkPartitions(){
        for(DBObject<Partition> part : partitions.values()){
            
            boolean hasAnyStorage = false;
            for(DBObject<Node> node : part.getObject().nodes.values()){
                if(!node.getObject().isUnreachable()){
                    hasAnyStorage = true;
                    break;
                }
            }
            if(!hasAnyStorage){
                // Data connected with Partition part is deleted or unavailable. Alert user.
            }
        }
    }
    
    public void delete(){
        // It should delete corresponding entry from Node relation from catalog database
        // Corresponding entries from Replica relation should be deleted automatically
        
        unreachable = true;
        checkPartitions();
    }
    
    public void toggleUnreachable(){
        unreachable = !unreachable;
        if(unreachable){
            checkPartitions();
        }
    }
    
    public LinkedHashMap<String, DBObject<Partition>> getPartitions(){
        return partitions;
    }
    public String getAddress() { return address; }
    public String getUrl() { return url; }
    public String getPassword() { return password; }
    public String getUser() { return user; }
    public String getDriver() { return driver; }
    public boolean isUnreachable() { return unreachable; }
}
