#pragma once
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStackedWidget>

#include "../headers/game.h"

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 400

class GameWidget;
class MainMenu;

class MainWindow : public QMainWindow {
 public:
  explicit MainWindow(QWidget *parent = nullptr);

  MainMenu *GetMainMenu();

 private:
  void *handle_;
  void (*input_)(int, bool) = nullptr;
  GameInfo_t (*update_)() = nullptr;
  void ShowMainMenu();
  void StartGame(const std::string &name);
  void LoadLibrary(const std::string &name);

  QStackedWidget *stack_;
  MainMenu *main_menu_;
};