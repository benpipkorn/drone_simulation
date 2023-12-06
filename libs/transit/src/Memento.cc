#include "Memento.h"

Memento::Memento(std::string& path){
    this->filePath = path;
}

bool Memento::loadFromCSV(){
    std::cout << "File path in memento: " << this->filePath << std::endl;
    return true;
}