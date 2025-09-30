#include "./headers/model.h"

namespace brick_game {

Model::Model()
    : last_time_(0),
      gen_(std::random_device{}()),
      state_(WAITING),
      speed_(0),
      level_(0),
      pause_(false),
      score_(0),
      high_score_(0) {
  Database db;
  db.CreateTable("snake");
};

Model::~Model() {
  if (field_) {
    for (auto i = 0; i < HEIGHT; i++) {
      delete[] field_[i];
    }
    delete[] field_;
    field_ = nullptr;
  }
  if (snake_.GetLength()) {
    snake_.Clear();
  }
}

Model::Model(const Model &m)
    : last_time_(m.last_time_),
      gen_(m.gen_),
      state_(m.state_),
      speed_(m.speed_),
      level_(m.level_),
      pause_(m.pause_),
      score_(m.score_),
      high_score_(m.high_score_),
      snake_(m.snake_) {
  eat_[0] = m.eat_[0];
  eat_[1] = m.eat_[1];
  CreateField();
  for (int i = 0; i < HEIGHT; ++i)
    for (int j = 0; j < WIDTH; ++j) field_[i][j] = m.field_[i][j];
}

Model &Model::operator=(Model &&m) {
  if (this != &m) {
    std::swap(state_, m.state_);
    std::swap(speed_, m.speed_);
    std::swap(level_, m.level_);
    std::swap(pause_, m.pause_);
    std::swap(score_, m.score_);
    std::swap(high_score_, m.high_score_);
    std::swap(field_, m.field_);
    std::swap(snake_, m.snake_);
    std::swap(eat_[0], m.eat_[0]);
    std::swap(eat_[1], m.eat_[1]);
    std::swap(gen_, m.gen_);
    std::swap(last_time_, m.last_time_);
  }
  return *this;
}

void Model::UpdateState() {
  switch (state_) {
    case STARTING:
      StartGame();
      break;
    case MOVING_LEFT:
    case MOVING_RIGHT:
    case MOVING:
      MoveInterval();
      break;
    case GAMEOVER:
      GameOver();
      break;
    default:
      break;
  }
}

void Model::CreateField() {
  if (!field_) {
    field_ = new int *[HEIGHT];
    for (auto i = 0; i < HEIGHT; i++) {
      field_[i] = new int[WIDTH]();
    }
  }
}

void Model::ClearField() {
  if (field_) {
    for (auto i = 0; i < HEIGHT; i++) {
      delete[] field_[i];
    }
    delete[] field_;
    field_ = nullptr;
  }
}

void Model::Start() {
  if (state_ == WAITING) {
    state_ = STARTING;
  }
}

int64_t Model::Now() {
  auto now = std::chrono::high_resolution_clock::now();
  auto micros = std::chrono::duration_cast<std::chrono::microseconds>(
                    now.time_since_epoch())
                    .count();
  return micros;
}

void Model::StartGame() {
  Database db;
  high_score_ = db.GetHighestScore("snake");
  ClearField();
  CreateField();
  speed_ = SPEED;
  eat_[0] = 5;
  eat_[1] = 5;
  snake_.Clear();
  snake_.Spawn();
  score_ = 0;
  level_ = 1;
  last_time_ = Now();
  state_ = MOVING;
  snake_.GetBody(field_);
  field_[5][5] = 2;
}

void Model::PauseGame() {
  if (state_ == GAMEOVER || state_ == WAITING) {
    return;
  }
  if (pause_) {
    pause_ = false;
  } else {
    pause_ = true;
  }
}

void Model::Clear() {
  state_ = WAITING;
  level_ = 0;
  speed_ = 0;
  score_ = 0;
  high_score_ = 0;
  pause_ = false;
  ClearField();
  snake_.Clear();
}

void Model::GameOver() {
  if (state_ == WAITING) {
    Clear();
  } else {
    Database db;
    db.InsertScore(score_, "snake");
    state_ = WAITING;
    speed_ = 0;
    pause_ = false;
  }
}

void Model::MoveLeft() {
  if (state_ == MOVING) {
    state_ = MOVING_LEFT;
  } else if (state_ == MOVING_RIGHT) {
    state_ = MOVING;
  }
}

void Model::MoveRight() {
  if (state_ == MOVING) {
    state_ = MOVING_RIGHT;
  } else if (state_ == MOVING_LEFT) {
    state_ = MOVING;
  }
}

void Model::MoveInterval() {
  auto now = Now();
  ;
  if (now - last_time_ > speed_) {
    last_time_ = now;
    MoveSnake();
  }
}

void Model::MoveSnake() {
  if ((state_ != MOVING && state_ != MOVING_LEFT && state_ != MOVING_RIGHT) ||
      pause_) {
    return;
  }
  if (state_ == MOVING_LEFT) {
    snake_.RotateSnake(Left);
    state_ = MOVING;
  }
  if (state_ == MOVING_RIGHT) {
    snake_.RotateSnake(Right);
    state_ = MOVING;
  }
  bool result = snake_.Move(eat_);
  if (result) {
    state_ = GAMEOVER;
  } else {
    CheckEat();
  }
}

void Model::SpawnEat() {
  if (score_ < 200) {
    std::uniform_int_distribution<> dist_height(0, HEIGHT - 1);
    std::uniform_int_distribution<> dist_width(0, WIDTH - 1);

    int random_height = dist_height(gen_);
    int random_width = dist_width(gen_);
    while (field_[random_height][random_width]) {
      random_height = dist_height(gen_);
      random_width = dist_width(gen_);
    }
    eat_[0] = random_height;
    eat_[1] = random_width;
  }
}

void Model::CheckEat() {
  ClearField();
  CreateField();
  snake_.GetBody(field_);
  if (field_[eat_[0]][eat_[1]] > 0) {
    AddScore();
    SpawnEat();
  }
  field_[eat_[0]][eat_[1]] = 2;
}

void Model::AddScore() {
  score_++;
  if (score_ % 5 == 0 && score_ < 50) {
    LvlUp();
  } else if (score_ >= 200) {
    Victory();
  }
}

void Model::Victory() {
  Database db;
  db.InsertScore(score_, "snake");
  state_ = GAMEOVER;
  level_ = VICTORY;
}

void Model::LvlUp() {
  level_++;
  speed_ = SPEED - SPEED_BOOST * (level_ - 1);
}

GameInfo_t Model::GetCurrentState() {
  GameInfo_t game_info = {0, 0, 0, 0, 0, 0, 0};
  game_info.next = nullptr;
  if (score_ > high_score_) {
    game_info.high_score = score_;
  } else {
    game_info.high_score = high_score_;
  }
  game_info.field = field_;
  game_info.level = level_;
  game_info.pause = pause_;
  game_info.speed = speed_ / 1000;
  game_info.score = score_;
  return game_info;
}

void Model::RefreshTimer() { last_time_ = 0; }

}  // namespace brick_game
