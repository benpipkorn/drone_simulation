#include "Memento.h"

Memento::Memento(std::string& name) {
    this->fileName = name;
}

Memento::~Memento() {
    for (auto& it : objects) {
        delete it;
    }
}

bool Memento::collectData(std::map<int, IEntity*> entities,
    std::vector<const JsonObject*> trips) {
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

bool Memento::writeToCSV() {
    std::ofstream SaveFile(this->fileName);
    std::cout << this->objects.size() << std::endl;
    for (int i = 0; i < this->objects.size(); i++) {
        JsonObject const obj = *(this->objects.at(i));
        std::vector<std::string> keys = obj.getKeys();

        for (int j = 0; j < keys.size(); j++) {
            SaveFile << keys[j] << "," << obj[keys[j]] << ",";
        }
        SaveFile << "\n";
    }
    SaveFile.close();

    return true;
}

std::vector<const JsonObject*> Memento::loadFromCSV() {
    this->objects.clear();
    std::cout << "File path in memento: " << this->fileName << std::endl;
    std::ifstream ToSim;
    ToSim.open(this->fileName, std::ifstream::in);
    JsonObject* object = new JsonObject();
    std::string objKey, objValue;
    while (ToSim.good()) {
        // objKey = "";
        // objValue = "";
        getline(ToSim, objKey, ',');
        if (!ToSim.good()) {
            break;
        }
        if (objKey != "") {
            getline(ToSim, objValue, ',');
            if (!ToSim.good()) {
                break;
            }
            if (objValue[0] == '[') {  // array
                JsonArray arrayToAdd;
                JsonValue toPush = std::stod(objValue.substr(1), NULL);
                arrayToAdd.push(toPush);
                while (objValue[objValue.size() - 1] != ']') {
                    getline(ToSim, objValue, ',');
                    toPush = std::stod(objValue, NULL);
                    arrayToAdd.push(toPush);
                }
                (*object)[objKey] = arrayToAdd;
            } else {
                try {  // try parsing as number
                    double numToAdd = std::stod(objValue, NULL);
                    (*object)[objKey] = numToAdd;
                }
                catch (std::invalid_argument) {  // string
                    if (objKey == "\ncommand") {
                        objKey = objKey.substr(1, objKey.size());
                        std::cout << "Pushing to entities array\n" << std::endl;
                        this->objects.push_back(object);
                        object = new JsonObject();
                    }
                    (*object)[objKey] = objValue.substr(1, objValue.size() - 2);
                }
            }
            // create JsonObject with CSV info
            std::cout << objKey << ":" << (*object)[objKey] << std::endl;
        } else {
            std::cout << "Found empty key before eof" << std::endl;
            this->objects.clear();
            return objects;
        }
    }
    std::cout << "Pushing last entity to array" << std::endl;
    this->objects.push_back(object);

    ToSim.close();
    return this->objects;
}

std::string Memento::getName() {
    return this->fileName;
}
