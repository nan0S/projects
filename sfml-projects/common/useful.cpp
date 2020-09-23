#include "useful.hpp"

int randInteger (int a, int b)
{
	int fb = b - a + 1;
	return rand() % fb + a;
}

long long randLongLong (long long a, long long b)
{
	long long fb = b - a + 1;
	return rand() % fb + a;
}

float randFraction (float a, float b)
{
	float fb = b - a + 0.001;
	int nb = fb * 1000;
	float ret = (rand() % nb); ret = ret / 1000 + a;
	return ret;
}

bool pressed (Keyboard::Key key)
{
	return (Keyboard::isKeyPressed(key));
}

float dist (Vector2f pos1, Vector2f pos2)
{
	return sqrt((pos1.x - pos2.x) * (pos1.x - pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y));
}

float constrain (float value, float valueA, float valueB, float targetA, float targetB)
{
	value = std::min(value, valueB);
	value = std::max(value, valueA);
	return (value - valueA) / (valueB - valueA) * (targetB - targetA) + targetA;
}

void line (Vector2f point1, Vector2f point2, RenderWindow *window)
{
	VertexArray vertices(Lines, 2);
	vertices[0].position = point1;
	vertices[1].position = point2;
	window->draw(vertices);
}

void Clamp (float &value, float mini, float maxi)
{
	value = std::max(value, mini);
	value = std::min(value, maxi);
}

sf::Color hsv(int hue, float sat, float val)
{
	hue %= 360;
	while (hue < 0) hue += 360;

	if (sat < 0.f) sat = 0.f;
	if (sat > 1.f) sat = 1.f;

	if (val < 0.f) val = 0.f;
	if (val > 1.f) val = 1.f;

	int h = hue / 60;
	float f = float(hue) / 60 - h;
	float p = val * (1.f - sat);
	float q = val * (1.f - sat * f);
	float t = val * (1.f - sat * (1 - f));

	switch (h)
	{
		default:
		case 0:
		case 6: return sf::Color(val * 255, t * 255, p * 255);
		case 1: return sf::Color(q * 255, val * 255, p * 255);
		case 2: return sf::Color(p * 255, val * 255, t * 255);
		case 3: return sf::Color(p * 255, q * 255, val * 255);
		case 4: return sf::Color(t * 255, p * 255, val * 255);
		case 5: return sf::Color(val * 255, p * 255, q * 255);
	}
}
