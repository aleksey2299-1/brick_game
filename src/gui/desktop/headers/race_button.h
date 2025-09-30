#pragma once
#include <QtCore/QPoint>
#include <QtCore/QTimer>
#include <QtCore/QVector>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

#define RACE_PIXEL 4

class RaceButton : public QPushButton {
  Q_OBJECT

 public:
  RaceButton(const QString &text, QWidget *parent = (QWidget *)nullptr);

 protected:
  void paintEvent(QPaintEvent *event) override;
  void enterEvent(QEvent *event) override;
  void leaveEvent(QEvent *event) override;

 private slots:
  void updateRace();

 private:
  QTimer timer;
  bool hovered = false;
  QPoint position_ = {-5, 2};
  int stepsRemaining = 0;
  int baseY = 1;
  int verticalOffset = 0;

  void NextPosition();
  void ResetRace();
};
