#include "Memento.h"

Memento::Memento(std::string& name){
    this->fileName = name;
    this->filePath += this->fileName;
}

Memento::~Memento() {
    for (auto& it : objects) {
        delete it;
    }
}

bool Memento::collectData(std::map<int, IEntity*> entities, std::vector<const JsonObject*> trips) {
    for (auto it = entities.begin(); it != entities.end(); it++) {
        std::cout << "Memento being populated" << std::endl;
        JsonObject const *entityDetails = &(it->second->getDetails());
        this->objects.push_back(entityDetails);
    }
    for (auto i = trips.begin(); i != trips.end(); i++) {
        this->objects.push_back(*i);
    }
    return true;
}

bool Memento::writeToCSV(){
    std::ofstream SaveFile(this->filePath);
    for(int i=0; i < this->objects.size(); i++){
        JsonObject const obj = *(this->objects.at(i));
        std::vector<std::string> keys = obj.getKeys();

        for(int j=0;j < keys.size(); j++){
            SaveFile << keys[j] << "," << obj[keys[j]] << ",";
        }
        SaveFile << "\n";
    }
    SaveFile.close();

    return true;
}

std::vector<const JsonObject*> Memento::loadFromCSV(){
    std::cout << "File path in memento: " << this->filePath << std::endl;
    std::vector<const JsonObject*> entitiesToLoad;
    std::ifstream ToSim;
    ToSim.open(this->filePath, std::ifstream::in);
    // while (ToSim.good()) {
    //     create JsonObject with CSV info
    //     entities.push_back(object);
    // }
    ToSim.close();
    return entitiesToLoad;
}

std::string Memento::getName(){
    return this->fileName;
}