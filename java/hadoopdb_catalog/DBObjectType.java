/*
* Author: Przemysław Pastuszka
* Institute of Computer Science, Wrocław University, Poland
*/

// One can easily add new component to existing project. Few steps are required:
// - extend DBObjectType with new type
// - add new class implementing DBObjectInterface
// - add one line in DBObjectsFactory
// - extend existing classes (if needed)

public enum DBObjectType{ 
    PARTITION, RELATION, NODE
}
