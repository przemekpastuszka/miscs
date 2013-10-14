/*
* Author: Przemysław Pastuszka
* Institute of Computer Science, Wrocław University, Poland
*/

// Defines one column from relation. Definition is straightforward

public class Attribute{
    protected String name;
    protected boolean primaryKey;
    protected String foreignKey;
    protected AttributeType type;
    protected String properties;
    
    protected DBObject<Relation> relation;
    
    public Attribute(String name, boolean primaryKey, String foreignKey, AttributeType type, String properties){
        this.name = name;
        this.primaryKey = primaryKey;
        this.foreignKey = foreignKey;
        this.type = type;
        this.properties = properties;
    }
    
    public String getName() { return name; }
    public String getProperties() { return properties; }
    public boolean isPrimaryKey() { return primaryKey; }
    public boolean isForeignKey() { return foreignKey != ""; }
    public String getForeignKey() { return foreignKey; }
    public DBObject<Relation> getRelation() { return relation; }
    public AttributeType getType() { return type; }
    
}
