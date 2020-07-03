#include "Interface.hpp"

Interface::Interface() {
	//inicjalizacja paliwa
	fuelShape.setSize(fuelSize);
	fuelShape.setFillColor(Color::Green);
	fuelShape.setPosition(fuelPos);
	fuelShape.setOutlineThickness(5);
	fuelShape.setOutlineColor(Color::Black);

	fuelText.setString("Fuel: ");
	fuelText.setFont(fuelFont);
	fuelText.setCharacterSize(fuelTextSize);

	fuelAmountText.setFont(fuelFont);
	fuelAmountText.setCharacterSize(fuelTextSize);
}

void Interface::applyFuel(float &fuel) {
	fuelShape.setSize(Vector2f(fuel, fuelSize.y));
	std::stringstream liczba; liczba << fuel;
	fuelAmountText.setString(liczba.str());
}
void Interface::applyCameraPosition(Vector2f &pos, Vector2f &range) {
	fuelShape.setPosition(pos.x - range.x / 2 + fuelPos.x, pos.y - range.y / 2 + fuelPos.y);
	fuelText.setPosition(pos.x - range.x / 2 + fuelTextPos.x, pos.y - range.y / 2 + fuelTextPos.y);
	fuelAmountText.setPosition(pos.x - range.x / 2 + fuelTextPos.x + 60, pos.y - range.y / 2 + fuelTextPos.y);
}

void Interface::update(float fuel, Vector2f pos, Vector2f range) {
	applyCameraPosition(pos, range);
	applyFuel(fuel);
}

void Interface::drawTo(RenderWindow *window) {
	window->draw(fuelShape);
	window->draw(fuelText);
	window->draw(fuelAmountText);
}
