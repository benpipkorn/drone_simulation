#include "Memento.h"

Memento::Memento(std::string& name){
    this->fileName = name;
    this->filePath += this->file_name;
}

Memento::~Memento() {
    for (auto& it : objects) {
        delete it;
    }
}

bool Memento::collectData(std::map<int, IEntity*> entities) {
    for (auto it = entities.begin(); it != entities.end(); it++) {
        std::cout << "Memento being populated" << std::endl;
        JsonObject const *entityDetails = &(it->second->getDetails());
        this->objects.push_back(entityDetails);
    }
    return true;
}

bool Memento::writeToCSV(){
    // ofstream SaveFile(this->filePath);
    std::ofstream SaveFile("memento.csv");

    std::cout << this->objects.size()<<std::endl;
    for(int i=0; i < this->objects.size(); i++){
        std::cout << "Entered loop" <<std::endl;
        JsonObject const *obj = this->objects.at(i);
        std::cout << "Before keys " <<std::endl;

        std::vector<std::string> keys = obj->getKeys();
        std::cout << "After keys" <<std::endl;

        for(int j=0;j < keys.size(); j++){
            // SaveFile << keys.at(j) << "," << obj[keys[j]] << ",";
            std::cout << keys.at(j) << "\n";
        }
        SaveFile << "\n";
    }
    SaveFile.close();

    return true;
}

bool Memento::loadFromCSV(){
    std::cout << "File path in memento: " << this->filePath << std::endl;
    std::ifstream ToSim;
    ToSim.open(this->filePath, std::ifstream::in);
    // while (ToSim.good()) {
        
    // }
    ToSim.close();
    return true;
}

std::string Memento::getName(){
    return this->fileName;
}