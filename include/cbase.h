#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include "room.h"
#include "corda.h"
#include "filemanager.h"
CORDA corda;
namespace fs = std::filesystem;
class Node;
class Table;

class Cell {
public:
    std::string key;
    std::string value;
};

class Item {
public:
    int ID;
    std::string name;
    std::string data;
    std::unordered_map<std::string,std::pair<int,int>> CellsMap;
    bool SortCells();
};

class Table {
    public:
    std::string name;
    std::string mapPath;
    std::vector<Item> items;
    std::string path;
    std::unordered_map<std::string, int> map;
    ROOM room;
    ROOM roomMap;
    Table(std::string n, Node& parent);
    Item read(int id);
    bool edit(int ID, Item item);
    bool append(Item item, int size = 0);
    bool LoadMap();
};

class Node {
    public:
    std::vector<Table*> tables;
    std::vector<Node*> children;
    std::string name;
    std::string path;
    std::string nodeMap;
    ~Node();
    Node(std::string n);
    Node(std::string n, Node& parent);
    bool load();
    void addChild(Node* n);
    void addTable(Table* t);
};

class CBase {
    bool mount(std::string& path);

};