/*
* Author: Przemysław Pastuszka
* Institute of Computer Science, Wrocław University, Poland
*/

import java.util.LinkedHashMap;
import java.util.ArrayList;
import java.util.Map;


public class Catalog{
    
    protected long heartbeatTimeout; // how long Catalog can wait for heartbeat message from node before marking it as unreachable
    
    protected Map<String, Long> heartbeats; // when Catalog recieved heartbeat from node
    
    protected void createDatabaseSchema(String databaseSchemaFile){
        // create relations described by databaseSchemaFile
    }
    
    // Marks nodes as unreachable if they do not send heartbeat message in specified time
    protected void monitorNodes(){
        // For each node in catalog database:
        //      connect to it
        //      inform to start sending heartbeat messages
        //      add (node_name, current_time) to heartbeats map
        
        // In new thread repeat:
        //      wait for few (mili)seconds
        //      collect incoming heartbeats
        //          if node, which sent heartbeat, figures in heartbeats map
        //              update timestamp
        //          else
        //              add (node_name, current_timestamp) to heartbeats
        //              mark node as available
        //      mark all nodes, which exceeded time limit, as unreachable and remove corresponding entries from heartbeats
        
        // I'm aware, that using java.util.Map for this task is not the best asymptotic choice, but it only shows the idea
        // Final code should use structure similar to minimum heap with increase key operation and some hash map, which would allow
        // direct access to heap nodes.
    }
    
    public Catalog(String configurationFilePath, String databaseSchemaFile){
        // read heartbeatTimeout from configuration file
        DBManager.connect(configurationFilePath); // connect to database
        createDatabaseSchema(databaseSchemaFile);
        monitorNodes();
    }
    
    public DBObject<Node> getNode(String id){
        return DBObjectsManager.<Node>getExistingObject(id, DBObjectType.NODE);
    }
    public DBObject<Relation> getRelation(String id){
        return DBObjectsManager.<Relation>getExistingObject(id, DBObjectType.RELATION);
    }

    // copies Partition data to selected nodes
    public void createReplicas(DBObject<Partition> source, ArrayList<DBObject<Node>> targets){
        // for each available node in source.getObject().nodes.values()
        //      assign node to one target
        //      perform copying partition data from node to target in new thread
        //      create new relation in node database (using informations from source.getObject().relation)
        // as soon as copying operation from A to B in one thread is finished
        //      assign A and B nodes to two unprocessed targets. Perform copying and creating new relations.
        //  repeat above until all targets are fed with data
    }
    
    public DBObject<Node> createNode(String id, String address, String url, String password, String user, String driver){
        // insert data into catalog database
        // connect to new node to check if it is available
        //      inform new node to start sending heartbeat messages
        //      add appropriate data to heartbeats map
        return getNode(id);
    }
    public DBObject<Relation> createRelation(String id, ArrayList<Attribute> attributes, Attribute partitioningAttribute){
        // insert data into catalog database
        return getRelation(id);
    }
    public DBObject<Partition> createPartition(String id, String sourceUrl, DBObject<Node> firstReplicaTarget, String partitioningValue, DBObject<Relation> relation){
        // insert data into catalog database
        // copy partition data from file at sourceUrl into firstReplicaTarget node
        return DBObjectsManager.<Partition>getExistingObject(id, DBObjectType.PARTITION);
    }
}
