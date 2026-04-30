#include "../include/cbase.h"
#include <iostream>
Node::Node(std::string n) : name(n) {
    fs::create_directories(n);
    path = n;
    nodeMap = path + "/" + n + ".map";
    fs::create_directories(path);
    if(!fs::exists(nodeMap)) std::ofstream(nodeMap).close();
}

Node::Node(std::string n, Node& parent) : name(n) {
    path = parent.path + "/" + n;
    nodeMap = parent.path + "/" + n + ".map";
    fs::create_directories(path);
    if(!fs::exists(nodeMap)) std::ofstream(nodeMap).close();
    parent.addChild(this);
}
void Node::addChild(Node* n)  { 
    children.push_back(n); 
}
void Node::addTable(Table* t) { 
    tables.push_back(t); 
}

bool Node::load(){
    std::fstream file;
    std::string line;
    std::string Key;
    std::string Value;
    std::vector<std::string> Tables;
    std::vector<std::string> Nodes;
    file.open(this->nodeMap,std::ios::in);
    if(!file.is_open()) return 0;
    while(std::getline(file,line)){
        if(corda.isExist("TABLE",line)){
            Tables.push_back(corda.get("TABLE",line));
        }    
        else{
            Nodes.push_back(corda.get("NODE",line));
        }
    }
    file.close();
    for (int x = 0; x < Nodes.size();x++){
        Node n(Nodes[x],*this);
    }

    for (int x = 0; x < Tables.size();x++){
        Table t(Tables[x],*this);
    }

    return 1;
}

Table::Table(std::string n, Node& parent) : name(n) {
    path = parent.path + "/" + n + ".table";
    mapPath = parent.path + "/" + n + ".map";
    if (!fs::exists(path)) std::ofstream(path).close();
    if (!fs::exists(mapPath)) std::ofstream(mapPath).close();
    parent.addTable(this);
}

void Table::insert(Item item) {
    item.ID = items.size();
    map[item.name] = item.ID;
    items.push_back(item);
}

Item Table::read(int id) {
    if (id >= (int)items.size()) {
        return {-1, "", ""};
    }
    return items[id];
}

bool Table::edit(int ID, Item item) {
    if (ID >= (int)items.size()) return false;
    items[ID] = item;
    return true;
}

bool Table::append(Item item) {
    Append(item.data, path);
    Append(item.name + ":" + std::to_string(item.ID) + ";", mapPath);
    return true;
}



// Each Node  is a Folder
// Each Table is a File
// Each Item  is a Line
// Each Cell  is a Key/Value

int main(){
    Node school("school");
    school.load();
    std::cin.get();
    std::cout<<school.tables.size();
    return 0;
    Table ClassA("ClassA",school);
    Item student {0,"ali","name:ali;age:21;city:baghdad;"};
    Item temp = ClassA.read(ClassA.map["ali"]);
    std::cout << temp.data;
    ClassA.append(student);
    return 0;
}