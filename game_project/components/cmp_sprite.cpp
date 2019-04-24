
#include "cmp_sprite.h"
#include "system_renderer.h"
#include <engine.h>

using namespace std;

float tileSizeHERE = GAMEX / 64;
auto spriteOffset = sf::Vector2f(-tileSizeHERE / DIVIDER,- tileSizeHERE /DIVIDER );

void SpriteComponent::setTexure(std::shared_ptr<sf::Texture> tex)
{
  _texture = tex;
  _sprite->setTexture(*_texture);

}


SpriteComponent::SpriteComponent(Entity* p)
    : Component(p), _sprite(make_shared<sf::Sprite>()) {}

void SpriteComponent::update(double dt) {
  _sprite->setPosition(_parent->getPosition() + spriteOffset);
  _sprite->setRotation(_parent->getRotation());
}

void SpriteComponent::render() { Renderer::queue(_sprite.get()); }

void ShapeComponent::update(double dt) {
  _shape->setPosition(_parent->getPosition() + spriteOffset);
  _shape->setRotation(_parent->getRotation());
}

void ShapeComponent::render() { Renderer::queue(_shape.get()); }

sf::Shape& ShapeComponent::getShape() const { return *_shape; }

ShapeComponent::ShapeComponent(Entity* p)
    : Component(p), _shape(make_shared<sf::CircleShape>()) {}

sf::Sprite& SpriteComponent::getSprite() const { return *_sprite; }

void SpriteComponent::setTextureRect(const sf::IntRect & rect)
{
	_sprite->setTextureRect(rect);
}
