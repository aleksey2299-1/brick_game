#pragma once
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

#include "../../../headers/game.h"

#define CELL_SIZE 20
#define OBJECT_IN_CELL_SIZE CELL_SIZE - 2
#define CELL_ROUNDING 2
#define BORDER 5

class GameField;
class GameNext;

class GameWidget : public QWidget {
  Q_OBJECT
 public:
  explicit GameWidget(QWidget *parent = nullptr,
                      void (*input)(int, bool) = nullptr,
                      GameInfo_t (*update)() = nullptr);

 signals:
  void exitClicked();

 protected:
  void keyPressEvent(QKeyEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;

 private slots:
  void onExitClicked();

 private:
  void (*input_)(int, bool) = nullptr;
  GameInfo_t (*update_)() = nullptr;
  GameInfo_t game_info_ = {};
  GameField *field_ = nullptr;
  GameNext *next_ = nullptr;
  QLabel *record_ = nullptr;
  QLabel *score_ = nullptr;
  QLabel *level_ = nullptr;
  QLabel *speed_ = nullptr;
  QLabel *overlay_ = nullptr;
  bool pressed_ = false;

  void UserAction(int action, bool pressed);
  void SetGameInfo(GameInfo_t game_info);
  QVBoxLayout *InizializeInfo();
  void SetPropsLabelInfo(QLabel *label);
};
