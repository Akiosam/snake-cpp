#pragma once

#include "game.hpp"

#include <QRect>
#include <QTimer>
#include <QWidget>

class QKeyEvent;
class QMouseEvent;
class QPaintEvent;

class GameWidget final : public QWidget {
public:
  explicit GameWidget(QWidget* parent = nullptr);

protected:
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void paintEvent(QPaintEvent* event) override;

private:
  void onTick();

  Game game_;
  QTimer tick_timer_;
  QRect restart_button_rect_;
  Action held_direction_ = Action::None;
};
