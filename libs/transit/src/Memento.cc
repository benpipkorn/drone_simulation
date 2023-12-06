#include "Memento.h"

Memento::Memento(std::string& name){
    this->fileName = name;
}

bool Memento::loadFromCSV(){
    std::cout << "File path in memento: " << this->filePath << std::endl;
    return true;
}

std::string Memento::getName(){
    return this->fileName;
}