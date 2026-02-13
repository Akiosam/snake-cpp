#include <QApplication>
#include "qt/game_widget.hpp"

int main(int argc, char** argv) {
  QApplication app(argc, argv);

  GameWidget window;
  window.show();

  return app.exec();
}
