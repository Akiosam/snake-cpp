#pragma once

struct Vec2i {
  int x = 0;
  int y = 0;

  constexpr bool operator==(const Vec2i& other) const {
    return x == other.x && y == other.y;
  }

  constexpr bool operator!=(const Vec2i& other) const {
    return !(*this == other);
  }
};

enum class Dir {
  Up,
  Down,
  Left,
  Right
};

enum class Cell {
  Empty,
  Snake,
  Food
};

enum class GameState {
  Menu,
  Running,
  Paused,
  GameOver
};

struct Config {
  int cell_px = 1;
  int grid_w = 20;
  int grid_h = 15;
  int tick_ms = 180;
};
