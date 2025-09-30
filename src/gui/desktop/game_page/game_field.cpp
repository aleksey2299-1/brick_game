#include "../headers/game_field.h"

#include <QtCore/QVariant>

#include "../headers/game.h"

GameField::GameField(int **field, QWidget *parent)
    : QWidget(parent), field_(field) {
  setFixedSize(WIDTH * CELL_SIZE + BORDER * 2.5,
               HEIGHT * CELL_SIZE + BORDER * 2.5);
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  setAutoFillBackground(true);
  setProperty("class", "field");
}

void GameField::paintEvent(QPaintEvent *) {
  QPainter p(this);

  QRect border(BORDER / 2, BORDER / 2, WIDTH * CELL_SIZE + 1.5 * BORDER,
               HEIGHT * CELL_SIZE + 1.5 * BORDER);
  QPen border_pen(Qt::darkBlue, BORDER);
  QPen cell_pen(Qt::black, 1);
  QRect inner = border.adjusted(BORDER, BORDER, -BORDER, -BORDER);

  p.setPen(border_pen);
  p.setBrush(Qt::NoBrush);
  p.drawRoundedRect(border, BORDER, BORDER);

  if (!field_) {
    return;
  }

  p.setPen(cell_pen);
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (field_[i][j] == 0) {
        continue;
      }
      switch (field_[i][j]) {
        case 1:
          p.setBrush(Qt::green);
          break;
        case 2:
          p.setBrush(Qt::red);
          break;
        case 3:
          p.setBrush(Qt::blue);
          break;
        case 4:
          p.setBrush(Qt::magenta);
          break;
        default:
          p.setBrush(Qt::green);
          break;
      }
      p.drawRoundedRect(
          inner.x() + j * CELL_SIZE + (CELL_SIZE - OBJECT_IN_CELL_SIZE) / 2,
          inner.y() + i * CELL_SIZE + (CELL_SIZE - OBJECT_IN_CELL_SIZE) / 2,
          OBJECT_IN_CELL_SIZE, OBJECT_IN_CELL_SIZE, CELL_ROUNDING,
          CELL_ROUNDING);
    }
  }
}

void GameField::SetField(int **field) {
  field_ = field;
  update();
}