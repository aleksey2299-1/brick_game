#include "./headers/window.h"

#include <dlfcn.h>

#include "./headers/game.h"
#include "./headers/main_menu.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setFixedHeight(WINDOW_HEIGHT);
  setFixedWidth(WINDOW_WIDTH);

  setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);

  setWindowTitle("Brick Game");

  stack_ = new QStackedWidget(this);
  setCentralWidget(stack_);

  main_menu_ = new MainMenu;

  stack_->addWidget(main_menu_);

  connect(main_menu_, &MainMenu::tetrisClicked,
          [this]() { StartGame("./lib_tetris.so"); });
  connect(main_menu_, &MainMenu::snakeClicked,
          [this]() { StartGame("./lib_snake.so"); });
  connect(main_menu_, &MainMenu::raceClicked,
          [this]() { StartGame("./lib_race.so"); });

  ShowMainMenu();
}

void MainWindow::ShowMainMenu() { stack_->setCurrentWidget(main_menu_); }

void MainWindow::StartGame(const std::string &name) {
  LoadLibrary(name);
  GameWidget *game = new GameWidget(this, input_, update_);
  stack_->addWidget(game);

  connect(game, &GameWidget::exitClicked, [this, game]() {
    ShowMainMenu();
    stack_->removeWidget(game);
    game->deleteLater();
    dlclose(handle_);
    handle_ = nullptr;
    input_ = nullptr;
    update_ = nullptr;
  });

  stack_->setCurrentWidget(game);
}

void MainWindow::LoadLibrary(const std::string &name) {
  char *error;
  handle_ = dlopen(name.c_str(), RTLD_NOW);
  if (!handle_) {
    throw std::runtime_error("Cant open " + name +
                             " library: " + std::string(dlerror()));
  }
  dlerror();
  input_ = reinterpret_cast<void (*)(int, bool)>(dlsym(handle_, "UserInput"));
  if ((error = dlerror()) != NULL) {
    dlclose(handle_);
    throw std::runtime_error("Cant load \"UserInput\" function: " +
                             std::string(error));
  }

  update_ =
      reinterpret_cast<GameInfo_t (*)()>(dlsym(handle_, "UpdateCurrentState"));
  if ((error = dlerror()) != NULL) {
    dlclose(handle_);
    throw std::runtime_error("Cant load \"UpdateCurrentState\" function: " +
                             std::string(error));
  }
  if (input_) {
    input_(Start, false);
  }
}

MainMenu *MainWindow::GetMainMenu() { return main_menu_; }