#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class TextureRects{
public:
	static sf::IntRect character_jeremy_screaming;
	static sf::IntRect character_jeremy_neutral_0;
	static sf::IntRect character_jeremy_neutral_1;
	static sf::IntRect character_jeremy_lying_down;
	static sf::IntRect object_dialogBubble;
	static sf::IntRect title;
	static sf::IntRect object_rain;

	static sf::IntRect getTextureRectFromName(const std::string& p_name);
};