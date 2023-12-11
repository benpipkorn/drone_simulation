#include <filesystem>

#include "SimulationModel.h"

#include "DroneFactory.h"
#include "PackageFactory.h"
#include "RobotFactory.h"
#include "HumanFactory.h"
#include "HelicopterFactory.h"

SimulationModel::SimulationModel(IController& controller)
    : controller(controller) {
  entityFactory.AddFactory(new DroneFactory());
  entityFactory.AddFactory(new PackageFactory());
  entityFactory.AddFactory(new RobotFactory());
  entityFactory.AddFactory(new HumanFactory());
  entityFactory.AddFactory(new HelicopterFactory());
  for (auto const& dir_entry : 
    std::filesystem::directory_iterator("saves")) {
    std::string name = dir_entry.path().string();
    Memento *m = nullptr;
    if (name.compare(name.size() - 4 , 4, ".csv") == 0){
      m = new Memento(name);
    }
    else {
      std::cout << "Memento was not created, incorrect file type: " << name << std::endl;
    }
    if (m != nullptr) {
      m->loadFromCSV();
      this->saves.push_back(m);
      std::cout << "Memento loaded" << std::endl;
    }
  }
}

SimulationModel::~SimulationModel() {
  // Delete dynamically allocated variables
  for (auto& [id, entity] : entities) {
    delete entity;
  }
  delete graph;
}

IEntity* SimulationModel::createEntity(JsonObject& entity) {
  std::string name = entity["name"];
  JsonArray position = entity["position"];
  std::cout << name << ": " << position << std::endl;

  IEntity* myNewEntity = nullptr;
  if (myNewEntity = entityFactory.CreateEntity(entity)) {
    // Call AddEntity to add it to the view
    myNewEntity->linkModel(this);
    controller.addEntity(*myNewEntity);
    entities[myNewEntity->getId()] = myNewEntity;
  }

  return myNewEntity;
}

void SimulationModel::removeEntity(int id) {
  removed.insert(id);
}

/// Schedules a Delivery for an object in the scene
void SimulationModel::scheduleTrip(JsonObject& details) {
  std::string name = details["name"];
  JsonArray start = details["start"];
  JsonArray end = details["end"];
  std::cout << name << ": " << start << " --> " << end << std::endl;

  Robot* receiver = nullptr;

  for (auto& [id, entity] : entities) {
    if (name == entity->getName()) {
      if (Robot* r = dynamic_cast<Robot*>(entity)) {
        if  (r->requestedDelivery) {
          receiver = r;
          break;
        }
      }
    }
  }

  Package* package = nullptr;

  for (auto& [id, entity] : entities) {
    if (name + "_package" == entity->getName()) {
      if (Package* p = dynamic_cast<Package*>(entity)) {
        if  (p->requiresDelivery) {
          package = p;
          break;
        }
      }
    }
  }

  if (receiver && package) {
    package->initDelivery(receiver);
    std::string strategyName = details["search"];
    package->setStrategyName(strategyName);
    scheduledDeliveries.push_back(package);
    // trips.push_back(&details);
    controller.sendEventToView("DeliveryScheduled", details);
  }
}

const routing::IGraph* SimulationModel::getGraph() {
  return graph;
}

/// Updates the simulation
void SimulationModel::update(double dt) {
  for (auto& [id, entity] : entities) {
    entity->update(dt);
    controller.updateEntity(*entity);
  }
  for (int id : removed) {
    removeFromSim(id);
  }
  removed.clear();
}

void SimulationModel::stop(void) {
  controller.stop();
}

void SimulationModel::removeFromSim(int id) {
  IEntity* entity = entities[id];
  if (entity) {
    // auto tripsIdx = trips.begin();
    for (auto i = scheduledDeliveries.begin();
      i != scheduledDeliveries.end(); ++i) {
      if (*i == entity) {
        scheduledDeliveries.erase(i);
        // trips.erase(tripsIdx);
        break;
      }
      // tripsIdx++;
    }
    controller.removeEntity(*entity);
    entities.erase(id);
    delete entity;
  }
}

Memento* SimulationModel::getMemento(std::string name){
  for(int i=0; i < saves.size(); i++){
    Memento* m = saves.at(i);
    std::string path = m->getName();
    std::string file_name = path.substr(6, path.length());
    if(file_name.compare(name) == 0){
        // Found correct memento
        return m;
    }
  }
  return NULL;
}

void SimulationModel::save(){ // will need a name given for new memento
  std::string name = "save" + std::to_string(numMementos) + ".csv";
  Memento* m = new Memento(name);
  numMementos++;
  if (m->collectData(entities, trips)) {
    saves.push_back(m);
  }
  else {
    std::cout << "Issue saving data" << std::endl;
    numMementos--;
    return;
  }

  if(m->writeToCSV()){
    std::cout << "CSV Created\n";
  }
  else{
    numMementos--;
    std::cout << "Error Creating CSV beep boop beep\n";
  }

}

void SimulationModel::restore(Memento* m){
  std::cout<< "In load function in model" << std::endl;
  if(m == NULL){
    std::cout << "Memento not found for file\n";
    return;
  }
  // doing a "fresh start", getting rid of starting entities
  for(int i=0; i < entities.size() + 2; i++){
    std::cout << "Removing an entity: "<< entities[i]->getName() << "\n";
    removeFromSim(i);
  }

  std::vector<const JsonObject*> entitiesToLoad = m->loadFromCSV();
  for (auto i = entitiesToLoad.begin(); i != entitiesToLoad.end(); i++) { // adding new entities with json objects
    JsonObject currObject = *(*i);
    std::string create_entity = std::string(currObject["cmd"]);
    if (create_entity == "CreateEntity") {
      createEntity(currObject);
    }
    else if (create_entity == "ScheduleTrip") {
      scheduleTrip(currObject);
    }
    else{
      std::cout << "Unknown cmd encountered\n";
    }
  }
}
