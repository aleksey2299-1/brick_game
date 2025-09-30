#include "./headers/snake.h"

namespace brick_game {

Snake::Snake() {
  std::initializer_list<std::pair<int, int>> items(
      {{9, 6}, {9, 5}, {9, 4}, {9, 3}});
  body_ = std::list<std::pair<int, int>>(items);
}

Snake::~Snake() = default;

Snake::Snake(const Snake &s) : body_(s.body_), direction_(s.direction_) {}

Snake &Snake::operator=(Snake &&s) {
  if (this != &s) {
    std::swap(body_, s.body_);
    std::swap(direction_, s.direction_);
  }
  return *this;
}

void Snake::Clear() {
  if (!body_.empty()) {
    body_.clear();
    direction_ = 0;
  }
}

void Snake::Spawn() {
  if (body_.empty()) {
    std::initializer_list<std::pair<int, int>> items(
        {{9, 6}, {9, 5}, {9, 4}, {9, 3}});
    body_ = std::list<std::pair<int, int>>(items);
  }
}

void Snake::GetBody(int **field) {
  for (auto it = body_.begin(); it != body_.end(); ++it) {
    field[(*it).first][(*it).second] = 1;
  }
}

int Snake::GetLength() { return body_.size(); }

void Snake::RotateSnake(int key) {
  switch (key) {
    case Left:
      if (direction_ == 0) {
        direction_ = 3;
      } else {
        direction_--;
      }
      break;
    case Right:
      if (direction_ == 3) {
        direction_ = 0;
      } else {
        direction_++;
      }
      break;
    default:
      break;
  }
}

bool Snake::Move(const int eat[2]) {
  value_type head = body_.front();
  switch (direction_) {
    case R:
      if (head.second + 1 == WIDTH) {
        return 1;
      }
      body_.push_front({head.first, head.second + 1});
      break;
    case D:
      if (head.first + 1 == HEIGHT) {
        return 1;
      }
      body_.push_front({head.first + 1, head.second});
      break;
    case L:
      if (head.second == 0) {
        return 1;
      }
      body_.push_front({head.first, head.second - 1});
      break;
    case U:
      if (head.first == 0) {
        return 1;
      }
      body_.push_front({head.first - 1, head.second});
      break;
    default:
      break;
  }
  if (eat[0] != body_.front().first || eat[1] != body_.front().second) {
    body_.pop_back();
  }
  std::list<value_type> body_for_check = body_;
  body_for_check.sort();
  body_for_check.unique();
  if (body_for_check.size() != body_.size()) {
    return true;
  }
  return false;
}

}  // namespace brick_game
