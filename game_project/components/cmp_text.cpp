#include "cmp_text.h"
#include <system_renderer.h>
#include <system_resources.h>

void TextComponent::update(double dt) {}

void TextComponent::render() { Renderer::queue(&_text); }

TextComponent::TextComponent(Entity* const p, const std::string& str)
    : Component(p), _string(str) {
  _text.setString(_string);
  _font = Resources::get<sf::Font>("RobotoMono-Regular.ttf");
  _text.setFont(*_font);
}

void TextComponent::SetText(const std::string& str) {
  _string = str;
  _text.setString(_string);
}

void TextComponent::setPosition(sf::Vector2f pos) {
	_text.setPosition(pos);
}

void TextComponent::setSize(int s) {
	_text.setCharacterSize(s);
}

std::string TextComponent::getText() {
	return _string;
}

void TextComponent::setColour(sf::Color color) {
	_text.setColor(color);
}