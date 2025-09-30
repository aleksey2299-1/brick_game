#pragma once
#include <QtCore/QPoint>
#include <QtCore/QTimer>
#include <QtCore/QVector>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

#define PIXEL 8

enum e_SnakeState { MovingRight, MovingVertically, ReturningToBase };

class SnakeButton : public QPushButton {
  Q_OBJECT

 public:
  SnakeButton(const QString &text, QWidget *parent = (QWidget *)nullptr);

 protected:
  void paintEvent(QPaintEvent *event) override;
  void enterEvent(QEvent *event) override;
  void leaveEvent(QEvent *event) override;

 private slots:
  void updateSnake();

 private:
  QVector<QPoint> snake;
  QTimer timer;
  bool hovered = false;
  e_SnakeState state = MovingRight;
  QPoint direction = {1, 0};
  int stepsRemaining = 0;
  int baseY = 1;
  int verticalOffset = 0;

  QPoint NextPosition();
  void ResetSnake();
};
