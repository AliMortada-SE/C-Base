#pragma once
#ifndef CORDA_H
#define CORDA_H
#include <string>
#include <iostream>
#include <utility>
#include <vector>
class CORDA{
public:
std::pair<int,int> find(std::string key, std::string line){
    size_t pos = line.find(key);
    while(pos != std::string::npos){
        size_t after = pos + key.size();
        // skip spaces
        while(after < line.size() && line[after] == ' ') after++;
        // bounds check
        if(after >= line.size()){
            return {-1, -1};
        }
        if(line[after] == ':'){
            return {(int)pos, (int)(after + 1)};
        }
        // search again after this occurrence
        pos = line.find(key, pos + 1);
    }
    return {-1, -1};
}
int sizeOf(int index,std::string line){
    int size = 0;
    while(index<line.size() && line[index] != ';'){
        size++;
        index++;
    }
    return size;
}
bool isExist(std::string key,std::string line){
    std::pair<int,int> offset = find(key,line);
    if(offset.first == -1 && offset.second == -1) return 0;
    return 1;
}
std::string get(std::string key,std::string line){
    std::pair<int,int> offset = find(key,line);
    if(offset.first == -1 && offset.second == -1) return "";
    int x = offset.second;
    std::string value;
    while(x<line.size() &&  line[x]!=';'){
        value+=line[x];
        x++;
    }
    return value;
}
std::string add(std::string key,std::string value){
    return key + ':' + value + ';';
}
std::string remove(std::string key, std::string& line){
    std::pair<int,int> offset = find(key,line);
    if(offset.first == -1 && offset.second == -1) return "";
    int start = offset.first;
    int end   = sizeOf(offset.second,line);
    if("")
    line.erase(start,(offset.second - offset.first) + end + 1);
    return line;
}

int locateIndex(std::string key,std::string type, std::string line){
    std::string Key = get(type,line);
    Key+=';';
    std::string temp;
    int counter = 0;
    int x = 0;
    while(x<Key.size()){
        while(x < Key.size() && Key[x]!=','){
            temp+=Key[x];
            x++;
        }
        x++;
        if(temp == key){
            return counter;
        }
        else{
            if(x >= Key.size() || Key[x]==';') return -1;
            temp.clear();
            counter++;
        }
    }
    return -1;
}
std::string getByIndex(std::string key, std::string line, int index){
    std::string Key = get(key,line);
    Key+=';';
    int counter = 0;
    int x=0;
    std::string temp;
    while(counter != index && x<Key.size()){
        while(Key[x]!=','&& x<Key.size()){
            x++;
        }
        x++;
        counter++;
    }
    if(counter==index){
        while(Key[x]!=',' && Key[x]!=';' && x<Key.size()){
            temp+=Key[x];
            x++;
        }
    }
    return temp;
}
std::string createArray(std::string key, std::vector<std::string> list){
    std::string array;
    array += key + ':';
    int x = 0;
    while(x<list.size()){
        array += list[x];
        array += ',';
    }
    array+=';';
    return array;
}
};
#endif