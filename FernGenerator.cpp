#include "Fractal.h"
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

std::vector<Vector2f> vec;

Vector2f p1;
double b = 0.75; int r = 77, n = 12;
void Matr(Vector2f p0, double a,
	double L, int n)
{
	for (int i = 0; i <= 2 * r; i += r)
	{
		p1.x = p0.x + cos(a + i * b) * L;
		p1.y = p0.y + sin(a + i * b) * L;
		vec.push_back(p0);
		vec.push_back(p1);
		if (n > 0) Matr(p1, a + (i - 1) * b, L / 2, n - 1);
	}
}

void Fern()
{
	RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Fractal tree");
	window.setFramerateLimit(30);

	srand(time(NULL));

	window.clear();
	window.display();
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::MouseButtonPressed)
			{
				window.clear();

				vec.push_back(Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT));

				Matr(Vector2f(WINDOW_WIDTH / 2, 3 * WINDOW_HEIGHT/4), M_PI, WINDOW_HEIGHT/3, n);
				for (int i = 1; i < vec.size(); ++i) {

					Vertex line[2];
					line[0] = vec[i-1], line[1] = vec[i];

					window.draw(line, 2, Lines);
				}
				vec.clear();
				window.display();
			}
		}
	}
}