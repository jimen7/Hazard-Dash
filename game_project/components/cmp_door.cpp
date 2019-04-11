#include "cmp_door.h"
#include "cmp_physics.h"
#include "system_physics.h"




using namespace std;
using namespace sf;
using namespace Physics;

//std::vector<std::shared_ptr<Entity>>* DoorComponent::heroes_list;




void DoorComponent::update(double dt)
{
	if (_enterDoor) {
		auto collidingObjects = _parent->GetCompatibleComponent<PhysicsComponent>()[0]->getTouching();

		for (auto k : collidingObjects) {
			Entity* e1 = (Entity*)k->GetFixtureA()->GetUserData();
			Entity* e2 = (Entity*)k->GetFixtureB()->GetUserData();
			Entity* other;	//other is the player interacting
			if (e2 == _parent) {
				//e1 is the thing,e2 is us
				other = e1;
			}
			else {
				//e2 is the thing,e1 is us
				other = e2;
			}

			other->GetCompatibleComponent<PhysicsComponent>()[0]->teleport(_nextDoor);
			auto s = 1;
		}
	}
	
}


