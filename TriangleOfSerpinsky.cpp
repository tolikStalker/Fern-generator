#include "Fractal.h"

void trianlgeOfSerpinsky() {
  RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                      "Serpinsky triangle");
  window.setFramerateLimit(60);
  // VertexArray vertexes;
  std::vector<CircleShape*> GenCirc;
  std::vector<CircleShape*> circles;

  int NumOfUserCircles = 0;
  const int AllowNumOfCircles = 3;
  float delay = .001f;
  bool toggle = false;
  delay *= CLOCKS_PER_SEC;
  clock_t now = clock();

  Vector2f first(0, 0), second(0, 0);

  srand(time(NULL));

  while (window.isOpen()) {
    window.clear();
    Event event;
    while (window.pollEvent(event)) {
      if (event.type == Event::Closed) window.close();

      if (event.type == Event::MouseButtonPressed &&
          NumOfUserCircles < AllowNumOfCircles) {
        CircleShape* circle = new CircleShape(SPHERE_RADIUS, 5);
        circle->setFillColor(Color::White);
        circle->setPosition(event.mouseButton.x - SPHERE_RADIUS,
                            event.mouseButton.y - SPHERE_RADIUS);
        NumOfUserCircles++;
        circles.push_back(circle);
        window.display();
      }
    }

    if (clock() - now > delay && NumOfUserCircles == AllowNumOfCircles) {
      CircleShape* circle = new CircleShape(SPHERE_RADIUS, 5);
      circle->setFillColor(Color::White);

      if (!toggle) {
        for (first = circles[rand() % 3]->getPosition(),
            second = circles[rand() % 3]->getPosition();
             first == second;) {
          first = circles[rand() % 3]->getPosition();
          second = circles[rand() % 3]->getPosition();
        }
        toggle = !toggle;
      } else {
        second = circles[rand() % 3]->getPosition();
      }
      first.x = (first.x + second.x) / 2;
      first.y = (first.y + second.y) / 2;

      circle->setPosition(first.x, first.y);

      // NumOfUserCircles++;
      GenCirc.push_back(circle);

      // window.draw(*circle);

      now = clock();
    }
    for (auto it : circles) window.draw(*it);

    for (auto it : GenCirc) window.draw(*it);

    window.display();
  }
}