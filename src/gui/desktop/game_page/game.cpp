#include <QtCore/QTimer>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>

#include "../headers/game_field.h"
#include "../headers/game_next.h"
#include "../headers/window.h"

GameWidget::GameWidget(QWidget *parent, void (*input)(int, bool),
                       GameInfo_t (*update)())
    : QWidget(parent), input_(input), update_(update) {
  setFocusPolicy(Qt::StrongFocus);

  QPalette palette;
  palette.setBrush(QPalette::Window, QColor("#f8d791"));
  setAutoFillBackground(true);
  setPalette(palette);

  auto *main_layout = new QVBoxLayout(this);

  auto game_layout = new QHBoxLayout(nullptr);
  game_layout->setAlignment(Qt::AlignHCenter);

  auto info_layout = InizializeInfo();

  field_ = new GameField(nullptr, this);

  auto overlay_layout = new QVBoxLayout(field_);
  overlay_layout->addWidget(overlay_);
  overlay_->setAlignment(Qt::AlignCenter);

  game_layout->addWidget(field_, 0, Qt::AlignVCenter);
  game_layout->addLayout(info_layout);

  auto *btn_exit = new QPushButton("Exit to main");
  btn_exit->setProperty("class", "btn-primary");
  btn_exit->setMaximumWidth(WINDOW_WIDTH);
  btn_exit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

  main_layout->addStretch();
  main_layout->addLayout(game_layout);
  main_layout->addStretch();
  main_layout->addWidget(btn_exit);
  main_layout->setAlignment(Qt::AlignHCenter);

  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, [this]() {
    GameInfo_t game_info = (*update_)();
    SetGameInfo(game_info);
  });
  timer->start(16);

  connect(btn_exit, &QPushButton::clicked, [this]() { onExitClicked(); });
}

void GameWidget::SetGameInfo(const GameInfo_t game_info) {
  game_info_ = game_info;
  if (field_) {
    field_->SetField(game_info_.field);
  }
  if (next_) {
    next_->SetNext(game_info_.next);
  }
  if (record_) {
    record_->setText("Record: " + QString::number(game_info_.high_score));
    record_->adjustSize();
  }
  if (score_) {
    score_->setText("Score: " + QString::number(game_info_.score));
    score_->adjustSize();
  }
  if (level_) {
    level_->setText("Level: " + QString::number(game_info_.level));
    level_->adjustSize();
  }
  if (speed_) {
    speed_->setText("Speed: " + QString::number(game_info_.speed) + " ms");
    speed_->adjustSize();
  }
  if (overlay_) {
    if (game_info_.speed == 0) {
      overlay_->setText("GAME\nOVER");
      overlay_->raise();
      overlay_->show();
    } else if (game_info_.pause) {
      overlay_->setText("PAUSED");
      overlay_->raise();
      overlay_->show();
    } else if (game_info_.level == VICTORY) {
      overlay_->setText("VICTORY!");
      overlay_->raise();
      overlay_->show();
    } else {
      overlay_->hide();
    }
  }
}

#include <iostream>
void GameWidget::keyPressEvent(QKeyEvent *event) {
  UserAction(event->key(), pressed_);
  pressed_ = true;
}

void GameWidget::keyReleaseEvent(QKeyEvent *event) {
  if (event->isAutoRepeat()) {
    return;
  }
  pressed_ = false;
  UserAction(-1, false);
}

void GameWidget::UserAction(int action, bool pressed) {
  switch (action) {
    case 'p':
    case 'P':
      (*input_)(Pause, pressed);
      break;
    case 'r':
    case 'R':
      (*input_)(Start, pressed);
      break;
    case 'e':
    case 'E':
      (*input_)(Action, pressed);
      break;
    case Qt::Key_Left:
      (*input_)(Left, pressed);
      break;
    case Qt::Key_Right:
      (*input_)(Right, pressed);
      break;
    case Qt::Key_Down:
      (*input_)(Down, pressed);
      break;
    case Qt::Key_Up:
      (*input_)(Up, pressed);
      break;
    case Qt::Key_Space:
      (*input_)(Terminate, pressed);
      break;
    default:
      (*input_)(-1, pressed);
      break;
  }
}

QVBoxLayout *GameWidget::InizializeInfo() {
  overlay_ = new QLabel("PAUSED", this);
  overlay_->setAlignment(Qt::AlignCenter);
  overlay_->setStyleSheet(
      "background-color: transparent;"
      "color: black;"
      "font-size: 32px;"
      "font-weight: bold;");
  overlay_->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  overlay_->setAttribute(Qt::WA_TransparentForMouseEvents);
  overlay_->setWordWrap(true);
  overlay_->raise();
  overlay_->hide();

  auto info_layout = new QVBoxLayout();

  next_ = new GameNext(nullptr, this);

  record_ = new QLabel("Record: 0");
  score_ = new QLabel("Score: 0");
  level_ = new QLabel("Level: 1");
  speed_ = new QLabel("Speed: 0 ms");
  auto *controls = new QLabel(
      "<div style='text-align: center; font-size: "
      "14px; font-weight: 600'>Controls:</div>\n"
      "\n"
      "- Move Left: <b>\342\206\220</b>\n"
      "\n"
      "- Move Right: <b>\342\206\222</b>\n"
      "\n"
      "- Move Down: <b>\342\206\223</b>\n"
      "\n"
      "- Move Up: <b>\342\206\221</b>\n"
      "\n"
      "- Action: <b>E</b>\n"
      "\n"
      "- Pause: <b>P</b>\n"
      "\n"
      "- Stop: <b>SPACE</b>\n"
      "\n"
      "- Start: <b>R</b>");
  controls->setTextFormat(Qt::MarkdownText);

  SetPropsLabelInfo(record_);
  SetPropsLabelInfo(score_);
  SetPropsLabelInfo(level_);
  SetPropsLabelInfo(speed_);

  controls->setWordWrap(true);
  controls->adjustSize();

  info_layout->addWidget(next_, 0, Qt::AlignHCenter);
  info_layout->addWidget(record_, 0, Qt::AlignHCenter);
  info_layout->addWidget(score_, 0, Qt::AlignHCenter);
  info_layout->addWidget(level_, 0, Qt::AlignHCenter);
  info_layout->addWidget(speed_, 0, Qt::AlignHCenter);
  info_layout->addWidget(controls, 0, Qt::AlignLeft);
  info_layout->setSpacing(8);
  info_layout->setAlignment(Qt::AlignLeft);
  info_layout->setAlignment(Qt::AlignVCenter);

  return info_layout;
}

void GameWidget::SetPropsLabelInfo(QLabel *label) {
  label->setProperty("class", "info");
  label->adjustSize();
  label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

void GameWidget::onExitClicked() { emit exitClicked(); }