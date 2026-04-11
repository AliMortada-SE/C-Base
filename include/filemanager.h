std::fstream file;
bool Append(std::string data, std::string path){
    file.open(path, std::ios::app);
    if(!file.is_open()){
        return false;
    }
    file << data << "\n";
    file.close();
    return true;
}