#include "../include/cbase.h"
#include <iostream>
// --- Node ---
Node::Node(std::string n) : name(n) {
    fs::create_directories(n);
    path = n;
}

Node::Node(std::string n, Node& parent) : name(n) {
    path = parent.path + "/" + n;
    fs::create_directories(path);
    parent.addChild(this);
}

void Node::addChild(Node* n)  { children.push_back(n); }
void Node::addTable(Table* t) { tables.push_back(t); }

// --- Table ---
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


int main(){
    Node school("school");
    Table ClassA("ClassA",school);
    Item student {0,"ali","name:ali;age:21;city:baghdad;"};
    Item temp = ClassA.read(0);
    std::cout << temp.data;
    ClassA.append(student);
    return 0;
}