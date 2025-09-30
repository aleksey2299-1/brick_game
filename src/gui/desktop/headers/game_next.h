#pragma once
#include <QtGui/QPainter>
#include <QtWidgets/QWidget>

#include "../../../headers/game.h"
#include "game.h"

class GameNext : public QWidget {
 public:
  explicit GameNext(int **next, QWidget *parent = nullptr);

  void SetNext(int **next);

 protected:
  void paintEvent(QPaintEvent *) override;

 private:
  int **next_;
};