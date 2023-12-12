#include "Memento.h"

Memento::Memento(std::string& name){
    this->fileName = name;
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
    std::ofstream SaveFile(this->fileName);
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
    std::cout << "File path in memento: " << this->fileName << std::endl;
    std::vector<const JsonObject*> entitiesToLoad;
    std::ifstream ToSim;
    ToSim.open(this->fileName, std::ifstream::in);
    JsonObject object;
    std::string objKey, objValue;
    while (ToSim.good()) {
        objKey = "";
        objValue = "";
        getline(ToSim, objKey, ',');
        std::cout << objKey << ":";
        getline(ToSim, objValue, ',');
        if (objValue[0] == '[') { // array
            while (objValue[objValue.size() - 1] != ']') {
                std::cout << objValue << ",";
                getline(ToSim, objValue, ',');
            }
        }
        std::cout << objValue << std::endl;
        // getline(ToSim, newJson);
        // std::cout << newJson << std::endl;
        // object = JSON.parse(newJson);
        // entities.push_back(&object);
        // std::cout << objValue << std::endl;
        
        // create JsonObject with CSV info
        // entities.push_back(object);
    }
    ToSim.close();
    return entitiesToLoad;
}

std::string Memento::getName(){
    return this->fileName;
}