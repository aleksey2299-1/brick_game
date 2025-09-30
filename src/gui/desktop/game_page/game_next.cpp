#include "../headers/game_next.h"

#include <QtCore/QVariant>

GameNext::GameNext(int **next, QWidget *parent) : QWidget(parent), next_(next) {
  setFixedSize(4 * CELL_SIZE + BORDER * 2.5, 4 * CELL_SIZE + BORDER * 2.5);
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  setAutoFillBackground(true);
  setProperty("class", "next");
}

void GameNext::paintEvent(QPaintEvent *) {
  QPainter p(this);

  QRect border(BORDER / 2, BORDER / 2, 4 * CELL_SIZE + 1.5 * BORDER,
               4 * CELL_SIZE + 1.5 * BORDER);
  QPen border_pen(Qt::darkYellow, BORDER);
  QPen cell_pen(Qt::black, 1);
  QRect inner = border.adjusted(BORDER, BORDER, -BORDER, -BORDER);

  p.setPen(border_pen);
  p.setBrush(Qt::NoBrush);
  p.drawRoundedRect(border, BORDER, BORDER);

  if (!next_) {
    return;
  }

  p.setPen(cell_pen);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (next_[i][j] == 1) {
        p.setBrush(Qt::green);
        p.drawRoundedRect(
            inner.x() + j * CELL_SIZE + (CELL_SIZE - OBJECT_IN_CELL_SIZE) / 2,
            inner.y() + i * CELL_SIZE + (CELL_SIZE - OBJECT_IN_CELL_SIZE) / 2,
            OBJECT_IN_CELL_SIZE, OBJECT_IN_CELL_SIZE, CELL_ROUNDING,
            CELL_ROUNDING);
      }
    }
  }
}

void GameNext::SetNext(int **next) {
  next_ = next;
  update();
}