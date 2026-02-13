#include "game_widget.hpp"

#include "qt_input.hpp"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QString>

namespace {
QString actionToString(Action action) {
  switch (action) {
    case Action::Up: return "Up";
    case Action::Down: return "Down";
    case Action::Left: return "Left";
    case Action::Right: return "Right";
    case Action::Pause: return "Pause";
    case Action::Quit: return "Quit";
    case Action::None: return "None";
  }

  return "None";
}
} // namespace

GameWidget::GameWidget(QWidget* parent)
    : QWidget(parent) {
  setWindowTitle("Snake (Qt Playfield Prototype)");
  resize(720, 480);
  setFocusPolicy(Qt::StrongFocus);

  tick_timer_.setInterval(game_.config().tick_ms);
  QObject::connect(&tick_timer_, &QTimer::timeout, this, [this]() { onTick(); });
  tick_timer_.start();
}

void GameWidget::keyPressEvent(QKeyEvent* event) {
  if (game_.isGameOver()) {
    QWidget::keyPressEvent(event);
    return;
  }

  const Action action = mapQtKeyToAction(event->key());
  if (action == Action::Quit) {
    close();
  } else if (action == Action::Up || action == Action::Down || action == Action::Left || action == Action::Right) {
    held_direction_ = action;
    game_.applyAction(action);
  } else {
    game_.applyAction(action);
  }

  update();
  QWidget::keyPressEvent(event);
}

void GameWidget::keyReleaseEvent(QKeyEvent* event) {
  const Action action = mapQtKeyToAction(event->key());
  if (action == held_direction_) {
    held_direction_ = Action::None;
  }

  QWidget::keyReleaseEvent(event);
}

void GameWidget::mousePressEvent(QMouseEvent* event) {
  if (game_.isGameOver() && event->button() == Qt::LeftButton && restart_button_rect_.contains(event->pos())) {
    game_.reset();
    update();
  }

  QWidget::mousePressEvent(event);
}

void GameWidget::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  painter.fillRect(rect(), QColor(18, 24, 29));

  painter.setPen(QColor(236, 240, 241));
  QFont titleFont = painter.font();
  titleFont.setPointSize(18);
  titleFont.setBold(true);
  painter.setFont(titleFont);
  painter.drawText(24, 44, "Snake - Playfield");

  QFont bodyFont = painter.font();
  bodyFont.setPointSize(12);
  bodyFont.setBold(false);
  painter.setFont(bodyFont);
  painter.setPen(QColor(189, 195, 199));
  painter.drawText(24, 84, "Controls: WASD / Arrows  |  Q or Esc: Quit");

  painter.setPen(QColor(88, 214, 141));
  const QString status = game_.isGameOver() ? "Game Over" : "Running";
  painter.drawText(24, 112, "Last input: " + actionToString(game_.lastAction()) + "  |  Status: " + status +
                          "  |  Score: " + QString::number(game_.score()));

  const int top = 140;
  const int panelMargin = 24;
  const int panelW = width() - (panelMargin * 2);
  const int panelH = height() - top - panelMargin;
  const QRect panelRect(panelMargin, top, panelW, panelH);

  painter.setPen(QColor(59, 72, 84));
  painter.setBrush(QColor(27, 34, 40));
  painter.drawRoundedRect(panelRect, 10, 10);

  const int innerPadding = 20;
  const int gridCols = game_.config().grid_w;
  const int gridRows = game_.config().grid_h;
  const int usableW = panelRect.width() - (innerPadding * 2);
  const int usableH = panelRect.height() - (innerPadding * 2);
  const int cellW = usableW / gridCols;
  const int cellH = usableH / gridRows;
  const int cellSize = (cellW < cellH ? cellW : cellH);

  const int boardW = cellSize * gridCols;
  const int boardH = cellSize * gridRows;
  const int boardX = panelRect.x() + (panelRect.width() - boardW) / 2;
  const int boardY = panelRect.y() + (panelRect.height() - boardH) / 2;
  const QRect boardRect(boardX, boardY, boardW, boardH);

  painter.setPen(QColor(71, 88, 103));
  painter.setBrush(QColor(14, 19, 24));
  painter.drawRect(boardRect);

  painter.setPen(QColor(36, 48, 58));
  for (int c = 1; c < gridCols; ++c) {
    const int x = boardRect.x() + (c * cellSize);
    painter.drawLine(x, boardRect.y(), x, boardRect.y() + boardRect.height());
  }
  for (int r = 1; r < gridRows; ++r) {
    const int y = boardRect.y() + (r * cellSize);
    painter.drawLine(boardRect.x(), y, boardRect.x() + boardRect.width(), y);
  }

  painter.setPen(Qt::NoPen);
  painter.setBrush(QColor(88, 214, 141));
  for (const Vec2i& segment : game_.snake()) {
    painter.drawRect(
        boardRect.x() + (segment.x * cellSize),
        boardRect.y() + (segment.y * cellSize),
        cellSize,
        cellSize);
  }

  const Vec2i food = game_.food();
  painter.setBrush(QColor(231, 76, 60));
  painter.drawRect(boardRect.x() + (food.x * cellSize), boardRect.y() + (food.y * cellSize), cellSize, cellSize);

  if (game_.isGameOver()) {
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 140));
    painter.drawRect(boardRect);

    painter.setPen(QColor(255, 244, 214));
    QFont overFont = painter.font();
    overFont.setPointSize(20);
    overFont.setBold(true);
    painter.setFont(overFont);
    painter.drawText(boardRect, Qt::AlignHCenter | Qt::AlignTop, "Game Over");

    const int buttonW = 190;
    const int buttonH = 44;
    const int buttonX = boardRect.center().x() - (buttonW / 2);
    const int buttonY = boardRect.center().y() - (buttonH / 2);
    restart_button_rect_ = QRect(buttonX, buttonY, buttonW, buttonH);

    painter.setPen(QColor(55, 79, 91));
    painter.setBrush(QColor(250, 233, 167));
    painter.drawRoundedRect(restart_button_rect_, 8, 8);

    painter.setPen(QColor(34, 49, 56));
    QFont buttonFont = painter.font();
    buttonFont.setPointSize(12);
    buttonFont.setBold(true);
    painter.setFont(buttonFont);
    painter.drawText(restart_button_rect_, Qt::AlignCenter, "Restart");
  } else {
    restart_button_rect_ = QRect();
  }

  QWidget::paintEvent(event);
}

void GameWidget::onTick() {
  if (held_direction_ != Action::None) {
    game_.applyAction(held_direction_);
  }
  game_.step();
  update();
}
