#include <numeric>
#include <queue>

#include "Fractal.h"

constexpr auto FIELD_SIZE = 480,  // 
    CELL_SIZE = 24,               // 
    SIZE = 18;
constexpr auto ARRAY_SIZE = SIZE * SIZE;

bool visit[FIELD_SIZE][FIELD_SIZE] = {false};
int len[FIELD_SIZE][FIELD_SIZE] = {0};

Vector2f indexToCoord(int index) {
  return Vector2f(index % SIZE, index / SIZE);
}

void BFS(Vertex st, Vertex end,
         std::vector<std::pair<RectangleShape, bool>>& vec) {
  int x = st.position.x, y = st.position.y, fx = end.position.x,
      fy = end.position.y;
  int min;

  std::queue<std::pair<int, int>> q;

  bool Find = false;

  std::fill(len[0], &len[0][0] + FIELD_SIZE * FIELD_SIZE, 999999999);
  std::fill(visit[0], &visit[0][0] + FIELD_SIZE * FIELD_SIZE, false);

  visit[x][y] = true;
  len[x][y] = 0;
  q.push(std::make_pair(x, y));
  while (!q.empty() && !Find) {
    std::pair<int, int> u = q.front();
    q.pop();
    if (!visit[u.first - 1][u.second] && u.first - 1 >= 0 && !Find &&
        vec[u.second * SIZE + u.first - 1].second == true) {
      len[u.first - 1][u.second] = len[u.first][u.second] + 1;
      visit[u.first - 1][u.second] = true;
      if (u.first - 1 == fx && u.second == fy) {
        Find = true;
      } else
        q.push(std::make_pair(u.first - 1, u.second));
    }
    if (!visit[u.first + 1][u.second] && u.first + 1 < SIZE && !Find &&
        vec[u.second * SIZE + u.first + 1].second == true) {
      len[u.first + 1][u.second] = len[u.first][u.second] + 1;
      visit[u.first + 1][u.second] = true;
      if (u.first + 1 == fx && u.second == fy) {
        Find = true;
      } else
        q.push(std::make_pair(u.first + 1, u.second));
    }
    if (!visit[u.first][u.second - 1] && u.second - 1 >= 0 && !Find &&
        vec[(u.second - 1) * SIZE + u.first].second == true) {
      len[u.first][u.second - 1] = len[u.first][u.second] + 1;
      visit[u.first][u.second - 1] = true;
      if (u.first == fx && u.second - 1 == fy) {
        Find = true;
      } else
        q.push(std::make_pair(u.first, u.second - 1));
    }
    if (!visit[u.first][u.second + 1] && u.second + 1 < SIZE && !Find &&
        vec[(u.second + 1) * SIZE + u.first].second == true) {
      len[u.first][u.second + 1] = len[u.first][u.second] + 1;
      visit[u.first][u.second + 1] = true;
      if (u.first == fx && u.second + 1 == fy) {
        Find = true;
      } else
        q.push(std::make_pair(u.first, u.second + 1));
    }
    if (Find) {
      min = len[fx][fy];
      u.first = fx, u.second = fy;
      while (min > 0) {
        for (int i = -1; i < 2; i += 2) {
          if (len[u.first + i][u.second] < min && u.first + i >= 0 &&
              u.first + i < SIZE) {
            min = len[u.first + i][u.second];

            vec[u.second * SIZE + u.first + i].first.setFillColor(Color::Red);
            u.first += i;
            break;
          }
          if (len[u.first][u.second + i] < min && u.second + i >= 0 &&
              u.second + i < SIZE) {
            min = len[u.first][u.second + i];

            vec[(u.second + i) * SIZE + u.first].first.setFillColor(Color::Red);
            u.second += i;
            break;
          }
        }
      }
    }
  }
}

void Cell() {
  RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML");
  window.setFramerateLimit(30);
  unsigned int elements[ARRAY_SIZE];
  bool enableEditor = true;

  std::vector<std::pair<RectangleShape, bool>> rects;
  std::queue<int> q;

  RectangleShape shape(Vector2f(FIELD_SIZE, FIELD_SIZE));

  shape.setSize(Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
  shape.setOutlineThickness(2.f);
  shape.setOutlineColor(Color::Black);
  shape.setFillColor(Color::White);

  std::iota(elements, elements + ARRAY_SIZE, 1);

  for (unsigned int i = 0; i < ARRAY_SIZE; ++i) {
    // Вычисление позиции плашки для отрисовки
    Vector2f position(i % SIZE * CELL_SIZE + 10.f, i / SIZE * CELL_SIZE + 10.f);
    shape.setPosition(position);
    // Отрисовываем рамку плашки
    // window.draw(shape);
    rects.push_back(std::make_pair(shape, true));
  }

  for (auto& it : rects) window.draw(it.first);

  window.display();

  Vector2f start(0, 0), end(0, 0);

  while (window.isOpen()) {
    Event event;
    while (window.pollEvent(event)) {
      window.clear();
      Vector2i Mpos(Mouse::getPosition(window));

      if (event.type == Event::Closed) window.close();

      if (event.type == Event::MouseButtonPressed) {
        if (event.mouseButton.button == Mouse::Left && enableEditor)
          for (unsigned int i = 0; i < ARRAY_SIZE; i++) {
            FloatRect bounds = rects[i].first.getGlobalBounds();
            if (bounds.contains(Mpos.x, Mpos.y)) {
              rects[i].first.setFillColor(Color::Black);
              rects[i].second = false;
              break;
            }
          }

        if (event.mouseButton.button == Mouse::Right && enableEditor)
          for (unsigned int i = 0; i < ARRAY_SIZE; i++) {
            FloatRect bounds = rects[i].first.getGlobalBounds();
            if (bounds.contains(Mpos.x, Mpos.y) &&
                rects[i].first.getFillColor() != Color::Blue) {
              rects[i].first.setFillColor(Color::Blue);
              q.push(i);
              break;
            }
          }
        for (auto& it : rects) window.draw(it.first);

        window.display();
      }

      if (Keyboard::isKeyPressed(Keyboard::A)) {
        // enableEditor = !enableEditor;

        if (!q.empty()) {
          start = indexToCoord(q.front());
          q.pop();
        }
        if (!q.empty()) {
          end = indexToCoord(q.front());
        }

        BFS(start, end, rects);
        for (auto& it : rects) window.draw(it.first);

        window.display();
      }
    }
  }
}