#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include "filemanager.h"
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
};

class Table {
    std::string name;
    std::string path;
    std::string mapPath;
    std::vector<Item> items;
    std::unordered_map<std::string, int> map;
public:
    Table(std::string n, Node& parent);
    void insert(Item item);
    Item read(int id);
    bool edit(int ID, Item item);
    bool append(Item item);
};

class Node {
    std::string name;
    std::vector<Table*> tables;
    std::vector<Node*> children;
public:
    std::string path;
    Node(std::string n);
    Node(std::string n, Node& parent);
    void addChild(Node* n);
    void addTable(Table* t);
};

class CBase {

};