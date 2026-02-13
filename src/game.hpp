#pragma once

#include "input.hpp"
#include "types.hpp"

#include <deque>
#include <optional>
#include <random>

class Game {
public:
  explicit Game(Config cfg = {});

  void reset();
  void applyAction(Action action);
  void step();

  const Config& config() const;
  const std::deque<Vec2i>& snake() const;
  Vec2i food() const;
  Action lastAction() const;
  bool isGameOver() const;
 int score() const;

private:
  static bool areOpposite(Dir a, Dir b);
  Vec2i headDelta() const;
  bool isInside(const Vec2i& pos) const;
  bool snakeOccupies(const Vec2i& pos) const;
  bool collidesWithBody(const Vec2i& pos, bool growing) const;
  Vec2i nextFoodSpawn();

  Config cfg_;
  std::deque<Vec2i> snake_;
  Vec2i food_{13, 4};
  Dir dir_ = Dir::Right;
  std::optional<Dir> next_dir_;
  std::optional<Dir> queued_dir_;
  Action last_action_ = Action::None;
  bool game_over_ = false;
  int score_ = 0;
  std::mt19937 rng_;
};
