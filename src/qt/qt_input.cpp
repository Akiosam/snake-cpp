#include "qt_input.hpp"

#include <QtCore/Qt>

Action mapQtKeyToAction(int key) {
  switch (key) {
    case Qt::Key_W:
    case Qt::Key_Up:
      return Action::Up;
    case Qt::Key_S:
    case Qt::Key_Down:
      return Action::Down;
    case Qt::Key_A:
    case Qt::Key_Left:
      return Action::Left;
    case Qt::Key_D:
    case Qt::Key_Right:
      return Action::Right;
    case Qt::Key_Q:
    case Qt::Key_Escape:
      return Action::Quit;
    default:
      return Action::None;
  }
}
