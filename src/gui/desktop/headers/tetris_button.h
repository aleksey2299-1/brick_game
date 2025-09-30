#include <QtCore/QTimer>
#include <QtCore/QVector>
#include <QtGui/QPixmap>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

class TetrisButton : public QPushButton {
  Q_OBJECT

 public:
  explicit TetrisButton(const QString &text,
                        QWidget *parent = (QWidget *)nullptr);

 protected:
  void paintEvent(QPaintEvent *event) override;
  void enterEvent(QEvent *event) override;
  void leaveEvent(QEvent *event) override;

 private slots:
  void updateAnimation();

 private:
  struct Icon {
    QPointF position;
    float speed;
    float xDrift;
    float angle;
    float rotationSpeed;
    QPixmap pixmap;
  };
  QPixmap icon_I, icon_J, icon_L, icon_O, icon_S, icon_T, icon_Z;
  QVector<Icon> icons;
  QTimer timer;
  bool hovered = false;

  void spawnIcon();
};