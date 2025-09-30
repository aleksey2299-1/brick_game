#include "./headers/main_menu.h"

#include <QtCore/QVariant>
#include <QtGui/QPalette>
#include <QtWidgets/QVBoxLayout>

#include "./headers/race_button.h"
#include "./headers/snake_button.h"
#include "./headers/tetris_button.h"
#include "./headers/window.h"

MainMenu::MainMenu(QWidget *parent) : QWidget(parent) {
  QPixmap pixmap(":/BrickGame.jpeg");
  pixmap = pixmap.scaled(this->size(), Qt::KeepAspectRatio,
                         Qt::SmoothTransformation);
  QPalette palette;
  palette.setBrush(QPalette::Window, QBrush(pixmap));
  this->setAutoFillBackground(true);
  this->setPalette(palette);

  this->update();

  auto *layout = new QVBoxLayout(this);
  auto *btn_tetris = new TetrisButton("Tetris");
  auto *btn_snake = new SnakeButton("Snake");
  auto *btn_race = new RaceButton("Race");
  auto *btn_exit = new QPushButton("Exit");

  btn_tetris->setProperty("class", "btn-main");
  btn_snake->setProperty("class", "btn-main");
  btn_race->setProperty("class", "btn-main");
  btn_exit->setProperty("class", "btn-main");

  btn_tetris->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  btn_snake->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  btn_race->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  btn_exit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

  btn_tetris->setMaximumWidth(WINDOW_WIDTH);
  btn_snake->setMaximumWidth(WINDOW_WIDTH);
  btn_race->setMaximumWidth(WINDOW_WIDTH);
  btn_exit->setMaximumWidth(WINDOW_WIDTH);

  connect(btn_tetris, &QPushButton::clicked, this,
          &MainMenu::onTetrisButtonClicked);
  connect(btn_snake, &QPushButton::clicked, this,
          &MainMenu::onSnakeButtonClicked);
  connect(btn_race, &QPushButton::clicked, this,
          &MainMenu::onRaceButtonClicked);
  connect(btn_exit, &QPushButton::clicked, this,
          &MainMenu::onExitButtonClicked);

  layout->addStretch();
  layout->addWidget(btn_tetris);
  layout->addWidget(btn_snake);
  layout->addWidget(btn_race);
  layout->addWidget(btn_exit);
  layout->setAlignment(Qt::AlignHCenter);
}

void MainMenu::resizeEvent(QResizeEvent *event) {
  QPixmap pixmap(":/BrickGame.jpeg");
  pixmap =
      pixmap.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

  QPalette palette;
  palette.setBrush(QPalette::Window, QBrush(pixmap));
  setPalette(palette);

  QWidget::resizeEvent(event);
}

void MainMenu::onTetrisButtonClicked() { emit tetrisClicked(); }
void MainMenu::onSnakeButtonClicked() { emit snakeClicked(); }
void MainMenu::onRaceButtonClicked() { emit raceClicked(); }

void MainMenu::onExitButtonClicked() { emit exitClicked(); }