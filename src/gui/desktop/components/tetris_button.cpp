#include "../headers/tetris_button.h"

#include <QtGui/QPainter>
#include <cstdlib>
#include <ctime>

TetrisButton::TetrisButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent),
      icon_I(":/icons/I.png"),
      icon_J(":/icons/J.png"),
      icon_L(":/icons/L.png"),
      icon_O(":/icons/O.png"),
      icon_S(":/icons/S.png"),
      icon_T(":/icons/T.png"),
      icon_Z(":/icons/Z.png")

{
  icon_I = icon_I.scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  icon_J = icon_J.scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  icon_L = icon_L.scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  icon_O = icon_O.scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  icon_S = icon_S.scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  icon_T = icon_T.scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  icon_Z = icon_Z.scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  setAttribute(Qt::WA_OpaquePaintEvent);
  setAttribute(Qt::WA_TranslucentBackground);
  setAttribute(Qt::WA_Hover);
  setMouseTracking(true);
  std::srand(std::time(nullptr));

  connect(&timer, &QTimer::timeout, this, &TetrisButton::updateAnimation);
  timer.start(30);
}

void TetrisButton::paintEvent(QPaintEvent *event) {
  QPushButton::paintEvent(event);

  QPainter p(this);
  for (const Icon &iconData : icons) {
    QTransform transform;
    transform.translate(iconData.position.x() + iconData.pixmap.width() / 2,
                        iconData.position.y() + iconData.pixmap.height() / 2);
    transform.rotate(iconData.angle);
    transform.translate(-iconData.pixmap.width() / 2,
                        -iconData.pixmap.height() / 2);

    p.setTransform(transform);
    p.drawPixmap(0, 0, iconData.pixmap);
  }
  p.resetTransform();
}

void TetrisButton::updateAnimation() {
  for (Icon &iconData : icons) {
    iconData.position.ry() += iconData.speed;
    iconData.position.rx() += iconData.xDrift;
    iconData.angle += iconData.rotationSpeed;
  }

  icons.erase(std::remove_if(
                  icons.begin(), icons.end(),
                  [this](const Icon &i) { return i.position.y() > height(); }),
              icons.end());

  if (hovered) {
    if (icons.size() < 15) {
      spawnIcon();
    }
  }

  update();
}

void TetrisButton::spawnIcon() {
  Icon iconData;
  iconData.position = QPointF(std::rand() % width(), -32);
  iconData.speed = 1 + (std::rand() % 4);
  iconData.xDrift = -1 + (std::rand() % 3);
  iconData.angle = std::rand() % 360;
  iconData.rotationSpeed = (std::rand() % 5) - 2;

  int index = std::rand() % 7;
  switch (index) {
    case 0:
      iconData.pixmap = icon_I;
      break;
    case 1:
      iconData.pixmap = icon_J;
      break;
    case 2:
      iconData.pixmap = icon_L;
      break;
    case 3:
      iconData.pixmap = icon_O;
      break;
    case 4:
      iconData.pixmap = icon_S;
      break;
    case 5:
      iconData.pixmap = icon_T;
      break;
    case 6:
      iconData.pixmap = icon_Z;
      break;
  }

  icons.append(iconData);
}

void TetrisButton::enterEvent(QEvent *event) {
  QPushButton::enterEvent(event);
  hovered = true;
}

void TetrisButton::leaveEvent(QEvent *event) {
  QPushButton::leaveEvent(event);
  hovered = false;
}