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
    std::cout << "Memento being populated" << std::endl;
    for (auto it = entities.begin(); it != entities.end(); it++) {
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
    std::cout << "Writing to " << this->fileName << std::endl;
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
    std::cout << "Loading " << this->fileName << std::endl;
    this->objects.clear();
    std::ifstream ToSim;
    ToSim.open(this->fileName, std::ifstream::in);
    JsonObject* object = new JsonObject();
    std::string objKey, objValue;
    while (ToSim.good()) {
        // create JsonObject with CSV info
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
                        this->objects.push_back(object);
                        object = new JsonObject();
                    }
                    (*object)[objKey] = objValue.substr(1, objValue.size() - 2);
                }
            }
            // create JsonObject with CSV info
        } else {
            std::cout << "Found empty key before eof" << std::endl;
            this->objects.clear();
            return objects;
        }
    }
    this->objects.push_back(object);

    ToSim.close();
    return this->objects;
}

std::string Memento::getName() {
    return this->fileName;
}
