#include "../headers/race_button.h"

#include <QtGui/QPainter>

RaceButton::RaceButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent) {
  setAttribute(Qt::WA_OpaquePaintEvent);
  setAttribute(Qt::WA_TranslucentBackground);
  setAttribute(Qt::WA_Hover);
  setMouseTracking(true);
  connect(&timer, &QTimer::timeout, this, &RaceButton::updateRace);
  timer.start(50);
}

void RaceButton::paintEvent(QPaintEvent *event) {
  QPushButton::paintEvent(event);

  QPainter p(this);
  p.setRenderHint(QPainter::Antialiasing);
  const int pixel = RACE_PIXEL;
  if (position_.x() < 100 && position_ != QPoint{-5, 2}) {
    for (int i = 0; i < 5; i++) {
      if (i % 2 == 0) {
        p.setBrush(Qt::white);
      } else {
        p.setBrush(Qt::black);
      }
      p.drawRect(80 * pixel, i * pixel, pixel, pixel);
      if (i % 2 == 0) {
        p.setBrush(Qt::black);
      } else {
        p.setBrush(Qt::white);
      }
      p.drawRect(81 * pixel, i * pixel, pixel, pixel);
    }
  }
  p.setBrush(Qt::green);
  int x = position_.x();
  int y = position_.y();
  p.drawRect(x * pixel, y * pixel, pixel, pixel);
  p.drawRect((x - 1) * pixel, y * pixel, pixel, pixel);
  p.drawRect((x - 1) * pixel, (y - 1) * pixel, pixel, pixel);
  p.drawRect((x - 1) * pixel, (y + 1) * pixel, pixel, pixel);
  p.drawRect((x - 2) * pixel, y * pixel, pixel, pixel);
  p.drawRect((x - 3) * pixel, y * pixel, pixel, pixel);
  p.drawRect((x - 3) * pixel, (y - 1) * pixel, pixel, pixel);
  p.drawRect((x - 3) * pixel, (y + 1) * pixel, pixel, pixel);
}

void RaceButton::NextPosition() { position_.setX(position_.x() + 2); }

void RaceButton::updateRace() {
  if (!hovered && position_ == QPoint{-5, 2}) {
    update();
    return;
  }
  if (position_.x() * RACE_PIXEL > width() && hovered) {
    ResetRace();
  }
  NextPosition();
  update();
}

void RaceButton::enterEvent(QEvent *event) {
  QPushButton::enterEvent(event);
  hovered = true;
}

void RaceButton::leaveEvent(QEvent *event) {
  QPushButton::leaveEvent(event);
  hovered = false;
}

void RaceButton::ResetRace() { position_ = {-5, 2}; }