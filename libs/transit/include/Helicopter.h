#ifndef Helicopter_H_
#define Helicopter_H_

#include "IEntity.h"
#include "IStrategy.h"

/**
 * @brief A helicopter that flies around the simulation - has no function other than to fly around
*/
class Helicopter : public IEntity {
 public:
  /**
   * @brief Helicopter are created with a name
   * @param obj JSON object containing the drone's information
   */
  Helicopter(JsonObject& obj);

  ~Helicopter();

  void update(double dt);

 private:
  IStrategy* movement = nullptr;
};

#endif
