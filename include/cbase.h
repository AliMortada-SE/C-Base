#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>
class Cell{
    std::string key;
    std::string value;
};
class Item{
    int ID;
    std::string name;
    std::string data;
};
class Table{
    std::vector<Item> items;
    std::unordered_map<std::string, int> map;
    public:
    void insert(Cell c) {
        c.index = items.size();
        items.push_back(c);
    }
    std::string read(int x){
        if(x >= items.size()){
            return {-1,"",""};
        }
        return items[x];
    }

};
Table students;
Item ali;
ali.index = 0;
ali.name = "ali";
ali.data = "name:ali;age:21;city:baghdad;";
students.insert(ali);
students.read(ali.index);


class Node{
//insert Table and cell here
};

class CBase{
//insert all here
};