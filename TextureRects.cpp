#include "TextureRects.hpp"

sf::IntRect TextureRects::character_jeremy_screaming = sf::IntRect(0, 0, 129, 161);
sf::IntRect TextureRects::character_jeremy_neutral_0 = sf::IntRect(129, 0, 82, 111);
sf::IntRect TextureRects::character_jeremy_neutral_1 = sf::IntRect(129, 111, 82, 111);
sf::IntRect TextureRects::character_jeremy_lying_down = sf::IntRect(0, 161, 119, 88);
sf::IntRect TextureRects::object_dialogBubble = sf::IntRect(119, 222, 90, 60);
sf::IntRect TextureRects::title = sf::IntRect(0, 249, 57, 48);
sf::IntRect TextureRects::object_rain = sf::IntRect(57, 249, 25, 23);

sf::IntRect TextureRects::getTextureRectFromName(const std::string& p_name) {
	if (p_name == "character_jeremy_screaming") return TextureRects::character_jeremy_screaming;
	if (p_name == "character_jeremy_neutral_0") return TextureRects::character_jeremy_neutral_0;
	if (p_name == "character_jeremy_neutral_1") return TextureRects::character_jeremy_neutral_1;
	if (p_name == "character_jeremy_lying_down") return TextureRects::character_jeremy_lying_down;
	if (p_name == "object_dialogBubble") return TextureRects::object_dialogBubble;
	if (p_name == "title") return TextureRects::title;
	if (p_name == "object_rain") return TextureRects::object_rain;
	return IntRect();
};