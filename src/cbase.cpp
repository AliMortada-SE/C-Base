#include "cbase.h"
#include <iostream>
Node::~Node() {
    for (auto c : children) delete c;
    for (auto t : tables) delete t;
}
Node::Node(std::string n) : name(n) {
    this->name = n;
    this->path = n;
}
void Node::setParent(Node& parent){
    this->parent = &parent;
    parent.addChild(this);
    this->path = parent.path + "/" + this->path;
}
void Node::initDir(){
    fs::create_directories(path);
    this->nodeMap = this->path + "/" + this->name + ".map";
    if(!fs::exists(nodeMap)) std::ofstream(nodeMap).close();
}
bool Node::load(){
    std::fstream file;
    std::string line;
    std::vector<std::string> Tables;
    std::vector<std::string> Nodes;
    file.open(this->nodeMap,std::ios::in);      // 1 open Node Map
    if(!file.is_open()) return 0;
    while(std::getline(file,line)){             // 2 Load Tables and Children Nodes Name
        if(corda.isExist("TABLE",line)){
            Tables.push_back(corda.get("TABLE",line));
        }  
        else if (corda.isExist("NODE",line)){
            Nodes.push_back(corda.get("NODE",line));
        }
    }
    file.close();
    for (int x = 0; x < Nodes.size(); x++){
        Node* n = new Node(Nodes[x]);
        n->setParent(*this);
        n->initDir();
        n->load();                              // 3 Load Nodes
    }
    for (int x = 0; x < Tables.size(); x++){
        this->TablesMap.insert(Tables[x]);        // 4 Map  Tables
        Table* t = new Table(Tables[x]);
        t->setParent(*this);
        t->initDir(); 
        t->LoadMap();                           // 5 Load Items Map
        this->tables.push_back(t);             // 6 Load Table to Ram
    }
    return 1;
}
void Node::addTable(Table* t) { 
    if(!this->TablesMap.count(t->name)){
        std::cout<<"Table Creted.\n";
        TablesMap.insert(t->name);
        std::ofstream(nodeMap, std::ios::app) << corda.add("TABLE", t->name) << "\n";
        this->tables.push_back(t);                  
    }
}

void Node::addChild(Node* n)  { 
    if(!NodesMap.count(n->name)){
        NodesMap.insert(n->name);
        std::ofstream(nodeMap, std::ios::app) << corda.add("NODE", n->name) << "\n";
    }
    this->children.push_back(n); 
}
bool Table::LoadMap(){
    std::vector<uint8_t> data;
    std::vector<std::string> keys;
    keys.push_back("");
    for(int x=0;x<this->roomMap.nodes.size();x++){
        uint64_t size = this->roomMap.NodeSize(x);
        if(size == 0) continue;                            // ADDED
        data.assign(size, 0);                              // CHANGED (was data.resize(size))
        this->roomMap.ReadNode(x,(char*)data.data(),size);
        std::string str(data.begin(), data.end());
        str.resize(strnlen(str.c_str(), str.size()));      // ADDED
        keys = corda.keys(str);
        if(keys.empty() || keys[0].empty()) continue;      // ADDED
        std::string v = corda.get(keys[0], str);           // ADDED (was inline in stoi)
        if(v.empty()) continue;                            // ADDED
        this->map[keys[0]] = std::stoi(v);
    }
    // data.resize(0) removed — assign() overwrites anyway
    return 1;
}
Table::Table(std::string n) : name(n) {
    this->name = n;
}
void Table::setParent(Node& parent){
    if(!parent.TablesMap.count(this->name)){
        parent.addTable(this);
    }
    this->parent = &parent;
}
void Table::initDir(){
    if(!parent) { std::cout << "No parent set.\n"; return; }
    this->path = parent->path + "/" + this->parent->name + ".table";
    this->mapPath = parent->path + "/" + this->parent->name + ".map";
    this->room.SetFileName(this->path);
    this->room.open();
    this->roomMap.SetFileName(this->mapPath);
    this->roomMap.open();
}
Item Table::read(std::string ItemName) {
    Item item;
    std::vector<uint8_t> data;
    auto it = this->map.find(ItemName);
    if(it == this->map.end()) return {-1, "", ""};
    int ID = it->second;
    size_t size = room.NodeSize(ID);
    data.resize(size);
    this->room.ReadNode(ID,(char*)data.data(),size);
    std::string str(data.begin(), data.end());
    str.resize(strnlen(str.c_str(), str.size()));
    item.ID = ID;
    item.name = ItemName;
    item.data = str;
    this->map[item.name] =ID;
    item.SortCells();
    return item;
}


bool Table::append(Item& item, int size) {
    int itemSize = item.data.size();
    if(size == 0){
        size = item.data.size() + 1;
    }
    if(itemSize > size){
        std::cout<<"Data Size Bigger than Item Size, Failed to Add.\n";
        return 0;
    }
    if(this->map.count(item.name)){
        std::cout<<"Item already exists.\n";
        return 0;
    }
    item.ID = this->room.AddNode(size);
    if(item.ID < 0) return 0;
    this->map[item.name] = item.ID;
    std::string MapData = corda.add(item.name,std::to_string(item.ID));
    int mapID = this->roomMap.AddNode(MapData.size() + 1);
    this->roomMap.WriteNode(mapID,MapData.data(),MapData.size());
    this->room.WriteNode(item.ID,item.data.data(),item.data.size());
    this->items.push_back(item);
    return true;
}

bool Item::SortCells(){
    if(this->data.size() == 0){
        std::cout<<"There are no data.\n";
        return 0;
    }
    std::vector<std::string> keys = corda.keys(data);
    for(int x = 0 ; x < keys.size();x++){
       std::pair<int,int> p = corda.find(keys[x], data);
       p.second -= 1;
       this->CellsMap[keys[x]] = p;
    }
    return 1;
}

std::optional<std::unordered_map<std::string,std::string>> CBase::Definer(){
    std::string defineFile = "define.cbase";
    std::string line;
    std::fstream file;
    if(!this->path.empty()){
        defineFile = this->path + "/" + defineFile;
    }
    file.open(defineFile,std::ios::in);
    if(!file.is_open()){
        std::cout<<"Failed to Open define file.\n";
        return std::nullopt;
    }
    std::string key;
    std::string value;
    std::unordered_map<std::string,std::string> definer;
    std::vector<std::string> Keys;
    while(std::getline(file,line)){
        key = corda.key(line);
        value = corda.get(key,line);
        definer[value] = key;
    }
    file.close();
    return definer;

}
std::optional<std::unordered_map<std::string,std::string>> CBase::Linker(std::unordered_map<std::string,std::string>& definer){
    std::string linkFile = "link.cbase";
    std::fstream file;
    std::string line;
    std::string data;
    std::string key;

    std::vector<std::string> childs;
    std::vector<std::string> parents;
    if(!this->path.empty()){
        linkFile   = this->path + "/" + linkFile;
    }
    file.open(linkFile,std::ios::in);
    if(!file.is_open()){
        std::cout<<"Failed to Open define file.\n";
        return std::nullopt;
    }
    int x = 0;
    while(std::getline(file,line)){
        childs.push_back(corda.key(line));
        parents.push_back(corda.get(childs[x],line));
        line.clear();
        x++;
    }
    x = 0;
    while(x<childs.size()){
        if(definer[childs[x]] == "Node"){
            Node* parent = new Node(childs[x]);
        }

    }
    return std::nullopt;
}
bool CBase::mount(std::string& path){
    // Path is the main entry.
    std::fstream file;
    std::string line;
    std::string key;
    std::string value;
    auto definer = this->Definer();
    std::vector<std::string> NODES;
    std::vector<std::string> TABLES;
    std::vector<std::string> Keys;
    std::vector<std::string> Chain;
    if(!definer) { std::cout << "No data.\n"; return 0; }
    for(auto& [key, value] : *definer){
        if (key == "Node"){
            NODES.push_back(key);
        }
        else if(key == "Table"){
            TABLES.push_back(key); 
        }
    }
    int x = 0;
    return 1;
}

// Each Node  is a Folder
// Each Table is a File
// Each Item  is a Line
// Each Cell  is a Key/Value

int main(){
    //Node school("school");
    //school.load();
    //Table* ClassA = school.tables.empty()
    //? new Table("ClassA", school)
    //: school.tables[0];
    //Item student {0,"ali","name:ali;age:21;city:baghdad;"};
    //ClassA->append(student);
    //std::cout<<"Size of Tables -> "<< school.tables.size()<<"\n";
    //Item temp = ClassA->read("ali");
    //std::cout << temp.data<<"\n";
    return 0;
}

