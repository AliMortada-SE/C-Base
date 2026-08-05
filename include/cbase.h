#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <fstream>
#include <optional>
#include "room.h"
#include "corda.h"


#define Create x_0_x201CC


inline CORDA corda;
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
    Node* parent = nullptr;
    Table(std::string n);
    Item read(std::string ItemName);
    void setParent(Node& parent);
    void initDir();
    bool edit(int ID, Item item);
    bool append(Item& item, int size = 0);
    bool LoadMap();
};

class Node {
    public:
    std::vector<Table*> tables;
    std::vector<Node* > children;
    std::string name;
    std::string path;
    std::string nodeMap;
    Node* parent = nullptr;
    std::unordered_set<std::string> TablesMap;
    std::unordered_set<std::string> NodesMap;

    ~Node();
    Node(std::string n);
    void setParent(Node& parent);
    void initDir();
    bool load();
    void addChild(Node* n);
    void addTable(Table* t);
};

class CBase {
    public:
    std::optional<std::unordered_map<std::string,std::string>>  Definer();
    std::optional<std::unordered_map<std::string,std::string>>  Linker(std::unordered_map<std::string,std::string>& definer);
    bool mount(std::string& path);
    private:
    std::string path = "";

};
