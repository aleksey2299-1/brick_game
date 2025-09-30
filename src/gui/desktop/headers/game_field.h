#pragma once
#include <QtGui/QPainter>
#include <QtWidgets/QWidget>

#include "../../../headers/game.h"

class GameField : public QWidget {
 public:
  explicit GameField(int **field, QWidget *parent = nullptr);

  void SetField(int **field);

 protected:
  void paintEvent(QPaintEvent *) override;

 private:
  int **field_;
};