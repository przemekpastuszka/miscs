/*
* Author: Przemysław Pastuszka
* Institute of Computer Science, Wrocław University, Poland
*/

// DBManager creates and manages connection with catalog database
// Type of database is arbitrary - I'd use PostgreSQL.

public class DBManager{
    static private long lastModificationTimestamp; // information when the database was changed for the last time
    
    static public long getModificationTimestamp(){
        return lastModificationTimestamp;
    }
    
    static public void connect(String configurationFilePath){
        // read configuration from file and connect to database
        // set last modification timestamp
    }
    
    /* other methods this class should contain: */
    // disconnect
    // perform select query - lastModificationTimestamp is not updated
    // perform insert / delete query - update lastModificationTimestamp
}
