#pragma once

#include <string_view>

class Term;

enum class Action {
  None,
  Quit,
  Up,
  Down,
  Left,
  Right,
  Pause,
};

class Input {
public:
  Action poll(Term& term);

  // Short on-screen instructions for movement controls.
  static std::string_view movementIntro();
};
