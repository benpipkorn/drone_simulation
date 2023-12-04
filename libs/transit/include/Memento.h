#include <vector.h>
#include <string.h>

class Memento {
    private:
        std::vector<JsonObject*> objects;
        const std::string& filePath;
    public:
        /**
         * @brief Constructor for a Memento object
         * @param filePath The filepath to the CSV file to load from
        */
        Memento(const std::string& filePath);

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
        bool collectData(std::vector<IEntity*> entities);

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
        bool loadFromCSV();
        
};