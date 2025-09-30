#include "../headers/snake_button.h"

#include <QtGui/QPainter>

SnakeButton::SnakeButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent) {
  setAttribute(Qt::WA_OpaquePaintEvent);
  setAttribute(Qt::WA_TranslucentBackground);
  setAttribute(Qt::WA_Hover);
  setMouseTracking(true);
  connect(&timer, &QTimer::timeout, this, &SnakeButton::updateSnake);
  timer.start(50);
}

void SnakeButton::paintEvent(QPaintEvent *event) {
  QPushButton::paintEvent(event);

  if (snake.empty()) {
    return;
  }
  QPainter p(this);
  p.setRenderHint(QPainter::Antialiasing);
  p.setBrush(Qt::green);
  const int pixel = PIXEL;
  for (const QPoint &pt : snake) {
    p.drawRect(pt.x() * pixel, pt.y() * pixel, pixel, pixel);
  }
}

QPoint SnakeButton::NextPosition() {
  QPoint head = snake.first();

  switch (state) {
    case MovingRight:
      if (std::rand() % 5 == 0) {
        int dy = (std::rand() % 2 == 0) ? -1 : 1;
        verticalOffset = (std::rand() % 3) + 2;
        direction = {0, dy};
        stepsRemaining = verticalOffset;
        state = MovingVertically;
        return head + direction;
      } else {
        return head + QPoint(1, 0);
      }

    case MovingVertically:
      stepsRemaining--;
      if (stepsRemaining == 0) {
        direction = {1, 0};
        stepsRemaining = (std::rand() % 5) + 1;
        state = ReturningToBase;
      }
      return head + direction;
    case ReturningToBase:
      if (stepsRemaining == 0) {
        direction = {0, (baseY - head.y() > 0) ? 1 : -1};
        if (head.y() == baseY) {
          direction = QPoint(1, 0);
          state = MovingRight;
        }
      } else {
        stepsRemaining--;
      }
      return head + direction;
    default:
      return head + QPoint(1, 0);
  }
}

void SnakeButton::updateSnake() {
  if (!hovered && snake.empty()) {
    return;
  }
  if ((snake.empty() || snake.first().x() * PIXEL > width()) && hovered) {
    ResetSnake();
  }
  snake.prepend(NextPosition());
  snake.removeLast();
  update();
}

void SnakeButton::enterEvent(QEvent *event) {
  QPushButton::enterEvent(event);
  hovered = true;
}

void SnakeButton::leaveEvent(QEvent *event) {
  QPushButton::leaveEvent(event);
  hovered = false;
}

void SnakeButton::ResetSnake() {
  snake = {{0, 1}, {-1, 1}, {-2, 1}, {-3, 1}};
  direction = {1, 0};
  state = MovingRight;
}