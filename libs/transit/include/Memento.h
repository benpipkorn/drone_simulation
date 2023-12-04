#include "SimulationModel.h"

class Memento {
    private:
        std::vector<JsonObject*> objects;
    public:
        Memento();
        ~Memento();
        bool collectData(std::vector<IEntity*>);
        bool writeToCSV();
        bool loadFromCSV(const std::string& filePath);
        bool dataToSim();
        

};