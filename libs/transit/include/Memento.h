#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "IEntity.h"

class Memento {
    private:
        std::vector<const JsonObject*> objects;
        std::string filePath = "saves/";
        std::string fileName;
    public:
        /**
         * @brief Constructor for a Memento object
         * @param filePath The filepath to the CSV file to load from
        */
        Memento(std::string& name);

        /**
         * @brief Destructor for a Memento object. Deletes the objects vector
        */
        ~Memento();

        /**
         * @brief Attempts to collect data from a given set of entities, and stores
         * their values as JSON object pointers
         * @param entities The entities to be stored
         * @return True if storing was a success, false if an error occured
        */
        bool collectData(std::map<int, IEntity*> entities, std::vector<const JsonObject*> trips);

        /**
         * @brief Attemps to write data stored in the vector of JSON object pointers
         * to a CSV file, which will be stored in a folder within the project directory
         * @return True if the writing was successful, false if an error occured
        */
        bool writeToCSV();

        /**
         * @brief Attempts to load data from a CSV file defined by the filePath to 
         * this Memento object
         * @return True if the loading was successful, false if an error occured
        */
        std::vector<const JsonObject*> loadFromCSV();

        /**
         * @brief Gets the path atribute from the memento
         * 
         * @return A string that contains the file path/name
        */
        std::string getName();
        
};