#pragma once
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

class MainMenu : public QWidget {
  Q_OBJECT
 public:
  MainMenu(QWidget *parent = nullptr);

 signals:
  void tetrisClicked();
  void snakeClicked();
  void raceClicked();
  void exitClicked();

 protected:
  void resizeEvent(QResizeEvent *event) override;

 private slots:
  void onTetrisButtonClicked();
  void onSnakeButtonClicked();
  void onRaceButtonClicked();
  void onExitButtonClicked();
};