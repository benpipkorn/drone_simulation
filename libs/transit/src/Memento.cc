#include "Memento.h"

Memento::Memento(std::string& name){
    this->fileName = name;
}

Memento::~Memento() {
    for (auto& it : objects) {
        delete it;
    }
}

bool Memento::collectData(std::map<int, IEntity*> entities) {
    for (auto it = entities.begin(); it != entities.end(); it++) {
        std::cout << "Memento being populated" << std::endl;
        JsonObject entityDetails = it->second->getDetails();
        this->objects.push_back(&entityDetails);
    }
    return true;
}

bool Memento::loadFromCSV(){
    std::cout << "File path in memento: " << this->filePath << std::endl;
    return true;
}

std::string Memento::getName(){
    return this->fileName;
}