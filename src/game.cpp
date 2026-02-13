#include "game.hpp"

#include <algorithm>
#include <iterator>
#include <vector>

Game::Game(Config cfg)
    : cfg_(cfg), rng_(std::random_device{}()) {
  reset();
}

void Game::reset() {
  snake_.clear();
  snake_.push_back({8, 7});
  snake_.push_back({7, 7});
  snake_.push_back({6, 7});
  dir_ = Dir::Right;
  next_dir_.reset();
  queued_dir_.reset();
  last_action_ = Action::None;
  game_over_ = false;
  score_ = 0;
  food_ = {13, 4};
}

void Game::applyAction(Action action) {
  if (action == Action::None) {
    return;
  }

  last_action_ = action;

  if (game_over_ || action == Action::Pause) {
    return;
  }

  std::optional<Dir> candidate;
  if (action == Action::Up) candidate = Dir::Up;
  if (action == Action::Down) candidate = Dir::Down;
  if (action == Action::Left) candidate = Dir::Left;
  if (action == Action::Right) candidate = Dir::Right;

  if (!candidate.has_value()) {
    return;
  }

  const Dir wanted = *candidate;
  if (!next_dir_.has_value()) {
    if (wanted == dir_ || areOpposite(dir_, wanted)) {
      return;
    }
    next_dir_ = wanted;
    return;
  }

  if (wanted == *next_dir_ || areOpposite(*next_dir_, wanted)) {
    return;
  }

  if (!queued_dir_.has_value()) {
    queued_dir_ = wanted;
    return;
  }

  if (wanted == *queued_dir_) {
    return;
  }

  queued_dir_ = wanted;
}

void Game::step() {
  if (game_over_ || snake_.empty()) {
    return;
  }

  if (next_dir_.has_value()) {
    dir_ = *next_dir_;
    next_dir_ = queued_dir_;
    queued_dir_.reset();
  }

  const Vec2i head = snake_.front();
  const Vec2i delta = headDelta();
  const Vec2i next{head.x + delta.x, head.y + delta.y};

  if (!isInside(next)) {
    game_over_ = true;
    return;
  }

  const bool growing = (next == food_);
  if (collidesWithBody(next, growing)) {
    game_over_ = true;
    return;
  }

  snake_.push_front(next);
  if (growing) {
    ++score_;
    food_ = nextFoodSpawn();
  } else {
    snake_.pop_back();
  }
}

const Config& Game::config() const { return cfg_; }

const std::deque<Vec2i>& Game::snake() const { return snake_; }

Vec2i Game::food() const { return food_; }

Action Game::lastAction() const { return last_action_; }

bool Game::isGameOver() const { return game_over_; }

int Game::score() const { return score_; }

bool Game::areOpposite(Dir a, Dir b) {
  return (a == Dir::Up && b == Dir::Down) ||
         (a == Dir::Down && b == Dir::Up) ||
         (a == Dir::Left && b == Dir::Right) ||
         (a == Dir::Right && b == Dir::Left);
}

Vec2i Game::headDelta() const {
  if (dir_ == Dir::Up) return {0, -1};
  if (dir_ == Dir::Down) return {0, 1};
  if (dir_ == Dir::Left) return {-1, 0};
  return {1, 0};
}

bool Game::isInside(const Vec2i& pos) const {
  return pos.x >= 0 && pos.x < cfg_.grid_w && pos.y >= 0 && pos.y < cfg_.grid_h;
}

bool Game::snakeOccupies(const Vec2i& pos) const {
  return std::find(snake_.begin(), snake_.end(), pos) != snake_.end();
}

bool Game::collidesWithBody(const Vec2i& pos, bool growing) const {
  const auto end_it = growing ? snake_.end() : std::prev(snake_.end());
  return std::find(snake_.begin(), end_it, pos) != end_it;
}

Vec2i Game::nextFoodSpawn() {
  std::vector<Vec2i> free_cells;
  free_cells.reserve(static_cast<std::size_t>(cfg_.grid_w * cfg_.grid_h));

  for (int y = 0; y < cfg_.grid_h; ++y) {
    for (int x = 0; x < cfg_.grid_w; ++x) {
      const Vec2i candidate{x, y};
      if (!snakeOccupies(candidate)) {
        free_cells.push_back(candidate);
      }
    }
  }

  if (!free_cells.empty()) {
    std::uniform_int_distribution<std::size_t> pick(0, free_cells.size() - 1);
    return free_cells[pick(rng_)];
  }

  return food_;
}
