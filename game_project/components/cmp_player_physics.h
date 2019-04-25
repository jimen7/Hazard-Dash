#pragma once

#include "cmp_physics.h"

class PlayerPhysicsComponent : public PhysicsComponent {
protected:
  b2Vec2 _size;
  sf::Vector2f _maxVelocity;
  std::vector <sf::Vector2ul> _doors;
  bool _grounded;
  float _groundspeed;
  bool _AIMode = false;
  bool _goingLeft = true;
  bool _jumping = false;


  bool isGrounded() const;

public:
  void update(double dt) override;



  explicit PlayerPhysicsComponent(Entity* p, const sf::Vector2f& size, const std::vector <sf::Vector2ul> doors);

  PlayerPhysicsComponent() = delete;

  void setAIMode(bool AIMode) { _AIMode = AIMode; }
  void setGoingLeft(bool goingLeft) { _goingLeft = goingLeft; }
  void setJumping(bool jumping) { _jumping = jumping; }

};
