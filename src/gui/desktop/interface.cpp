#include <QtCore/QFile>
#include <QtWidgets/QApplication>

#include "./headers/main_menu.h"
#include "./headers/window.h"

void SetStyle(QApplication *app);

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  SetStyle(&app);

  app.setWindowIcon(QIcon(":/icons/S.png"));

  MainWindow w;
  w.show();

  app.connect(w.GetMainMenu(), &MainMenu::exitClicked,
              [&app]() { app.exit(); });

  return app.exec();
}

void SetStyle(QApplication *app) {
  QFile style_file(":/style.css");
  if (style_file.open(QFile::ReadOnly)) {
    QString style = QLatin1String(style_file.readAll());
    app->setStyleSheet(style);
  }
}