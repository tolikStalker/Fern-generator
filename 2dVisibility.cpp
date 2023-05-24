#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <ctime>
#include <cmath>
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define SPHERE_RADIUS 5.f
using namespace std;
using namespace sf;

bool vectorsIntersect(Vector2f A, Vector2f B, Vector2f C, Vector2f D, Vector2f& intersectionPoint)
{
	Vector2f S1 = B - A;
	Vector2f S2 = D - C;

	//Calculate scalar parameters
	float denominator = (S1.x * S2.y - S1.y * S2.x);

	//We can't divide by 0!
	if (denominator == 0.0f)
		return false;

	//S & T have the same denominator
	float numeratorS = (S1.x * (A.y - C.y) - S1.y * (A.x - C.x));

	float numeratorT = (S2.x * (A.y - C.y) - S2.y * (A.x - C.x));

	float s, t;
	s = numeratorS / denominator;
	t = numeratorT / denominator;

	//Check for intersection point
	if (s > 0.0f && s < 1.0f && t > 0.0f && t < 1.0f)
	{
		//Return intersection point
		intersectionPoint.x = A.x + (t * S1.x);
		intersectionPoint.y = A.y + (t * S1.y);
		return true;
	}

	return false;
}

void drawRay(Vertex lin[], RenderWindow &window, vector<pair<Vertex, Vertex>> &lines, Color color)
{
	Vector2f temp;
	double dist = INFINITY;
	for (auto i : lines)
	{
		Vector2f temp2;
		if (vectorsIntersect(i.first.position, i.second.position, lin[0].position, lin[1].position, temp2))
		{
			double d;
			if ((d = hypot(temp2.x - Mouse::getPosition(window).x,
				temp2.y - Mouse::getPosition(window).y)) < dist)
				temp = temp2, dist = d;
		}
	}
	if (dist != INFINITY)
		lin[1] = temp, lin[1].color = color;

	window.draw(lin, 2, Lines);
}

int main()
{
	RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML");
	window.setFramerateLimit(60);
	window.display();

	int delay = 2;
	delay *= CLOCKS_PER_SEC;
	time_t now = clock();

	vector<pair<Vertex, Vertex>> lines;

	const double pi = 3.1415;
	srand(time(NULL));

	Vertex wall[2] = {
		Vertex(Vector2f(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT)),
		Vertex(Vector2f(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT))
	};
	lines.push_back(make_pair(wall[0], wall[1]));

	RectangleShape rect(Vector2f(100, 100));
	rect.setPosition(150, 290);
	rect.setFillColor(Color::Transparent);
	rect.setOutlineThickness(1);
	rect.setOutlineColor(Color::White);

	lines.push_back(make_pair(Vector2f(150, 290), Vector2f(250, 290)));
	lines.push_back(make_pair(Vector2f(150, 290), Vector2f(150, 390)));
	lines.push_back(make_pair(Vector2f(250, 290), Vector2f(250, 390)));
	lines.push_back(make_pair(Vector2f(150, 390), Vector2f(250, 390)));


	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			//if (event.type == Event::MouseButtonPressed)
			{
				window.clear();
				CircleShape circle(SPHERE_RADIUS, 8);
				circle.setFillColor(Color::Black);
				circle.setPosition(Mouse::getPosition(window).x, Mouse::getPosition(window).y);
				window.draw(circle);

				if (clock() - now > delay)
				{
					wall[0] = Vertex(Vector2f(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT));
					wall[1] = Vertex(Vector2f(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT));
					lines[0] = make_pair(wall[0], wall[1]);
					now = clock();
				}

				window.draw(wall, 2, Lines);
				window.draw(rect);

				/*for (double i = 0; i < 2 * pi; i += .1f)
				{
					Vertex lin[2] = {
						Vertex(Vector2f(Mouse::getPosition(window).x, Mouse::getPosition(window).y)),
						Vertex(Vector2f(Mouse::getPosition(window).x + 5000 * cos(i),Mouse::getPosition(window).y + 5000.f * sin(i)))
					};

					drawRay(lin, window, lines, Color::Green);
					
				}*/

				for (auto i : lines)
				{
					Vertex lin[2] = {
						Vertex(Vector2f(Mouse::getPosition(window).x, Mouse::getPosition(window).y)),
						i.first
					};
					drawRay(lin, window, lines, Color::Red);

					lin[1] = i.second;
					drawRay(lin, window, lines, Color::Yellow);
				}



				window.display();
			}
		}

		


	}

	return 0;
}