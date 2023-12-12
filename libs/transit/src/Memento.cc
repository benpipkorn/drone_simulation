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
    std::ifstream ToSim;
    ToSim.open(this->fileName, std::ifstream::in);
    JsonObject object;
    std::string objKey, objValue;
    while (ToSim.good()) {
        // objKey = "";
        // objValue = "";
        getline(ToSim, objKey, ',');
        // std::cout << objKey << ":";
        getline(ToSim, objValue, ',');
        if (objValue[0] == '[') { // array
            JsonArray arrayToAdd;
            std::cout << objValue.substr(1) << std::endl;
            JsonValue toPush = std::stod(objValue.substr(1)); // ERROR HERE
            arrayToAdd.push(toPush);
            while (objValue[objValue.size() - 1] != ']') {
                getline(ToSim, objValue, ',');
                toPush = std::stod(objValue);
                arrayToAdd.push(toPush);
            }
            getline(ToSim, objValue, ']');
            toPush = std::stod(objValue);
            arrayToAdd.push(toPush);
            object[objKey] = arrayToAdd;
        }
        // else if (strcmp(objValue, std::to_string(std::stod(objValue))) == 0) // checking if string or double, error here
        else { // string
            object[objKey] = objValue.substr(1, objValue.size() - 2); // gets rid of "" in string
            std::cout << "Added string pair to object" << std::endl;
        }
        // getline(ToSim, newJson);
        // std::cout << newJson << std::endl;
        // object = JSON.parse(newJson);
        // entities.push_back(&object);
        // std::cout << objValue << std::endl;
        
        // create JsonObject with CSV info
        std::cout << objKey << ":" << object[objKey] << std::endl;
        this->objects.push_back(&object);
        std::cout << "Added object to list of entity objects" << std::endl;
    }
    ToSim.close();
    return this->objects;
}

std::string Memento::getName(){
    return this->fileName;
}