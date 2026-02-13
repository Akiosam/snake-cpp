# Snake plan (Qt Widgets)

## Session guard
- Am I changing game rules? **No (plan-only update)**
- Am I changing input mapping? **No (plan-only update)**
- Am I changing rendering only? **No (plan-only update)**
- Am I changing timing behavior? **No (plan-only update)**

---

## Direction
Qt Widgets is the only platform target for this project.
No SDL and no terminal UI work is planned.

---

## Current status
- [x] CMake project builds with Qt Widgets
- [x] `snake` target exists (`src/main.cpp`, linked to Qt Widgets)
- [x] Runtime binary is produced at repo root as `./snake`
- [x] `snake_tests` target exists with `src/tests_main.cpp`
- [x] Prototype `QWidget` game loop is running in `src/main.cpp`
- [x] Key mapping works: WASD/arrows, `P` pause, `Q`/`Esc` quit
- [x] Timer-driven tick is in place (`180ms`)
- [x] Grid draw is in place (`20x15`, `QPainter`)
- [x] Snake body moves using `std::deque<QPoint>`
- [x] Food marker is rendered (currently static)
- [x] Wrap-around movement exists
- [x] Core shared types exist in `src/types.hpp` (`Vec2i`, `Dir`, `Cell`, `GameState`, `Config`)
- [x] Input action enum exists in `src/input.hpp`

---

## Target tree (next step)
```text
.
|- CMakeLists.txt
|- src/
|  |- main.cpp
|  |- types.hpp            # keep or move to include/snake/types.hpp
|  |- input.hpp
|  |- game.hpp
|  |- game.cpp             # pure rules + state updates (no Qt)
|  |- grid.hpp
|  |- grid.cpp
|  |- snake_model.hpp
|  |- snake_model.cpp
|  |- qt/
|  |  |- game_widget.hpp
|  |  |- game_widget.cpp   # QWidget, painting, key events, timer wiring
|  |  \- qt_input.cpp      # Qt key -> Action mapping
|  \- tests_main.cpp
\- tests/
   |- game_tests.cpp
   |- grid_tests.cpp
   \- snake_tests.cpp
```

---

## Module boundaries
- **Pure logic (no Qt includes):**
  Owns rules, collisions, growth, food spawn, score, game state transitions.
  Uses project types (`Vec2i`, `Dir`, `Cell`, `GameState`, `Config`).
  Fully unit-testable in `snake_tests`.
- **Qt UI layer (Qt-only code):**
  Owns window, widget lifecycle, key events, timer hookup, painting.
  Converts Qt input events to `Action`.
  Reads logic state and draws it, but does not implement core rules.
- **Rule of thumb:**
  If it can run headless, keep it out of Qt files.

---

## Implementation checklist (from here)
- [ ] Extract game state + update step from `src/main.cpp` into `game.*`
- [ ] Extract grid helpers into `grid.*`
- [ ] Extract snake body/rule helpers into `snake_model.*`
- [ ] Keep `main.cpp` minimal: create app, create widget, start event loop
- [ ] Move QWidget code into `src/qt/game_widget.*`
- [ ] Keep key mapping in one Qt adapter (`qt_input.cpp`)
- [ ] Replace static food marker with logic-driven spawn
- [ ] Prevent food spawn on snake cells
- [ ] Add wall/self collision mode choice in config (wrap stays default for now)
- [ ] Add restart flow after game over
- [ ] Add score + paused/game-over overlays in painter
- [ ] Expand `snake_tests` for logic-only coverage
- [ ] Keep Qt UI smoke checks lightweight (manual run)

---

## Qt loop + fixed tick wiring
- [ ] Keep Qt event loop as the app driver (`QApplication::exec()`)
- [ ] Use `QTimer` only to schedule updates, not to hold game rules
- [ ] Route each timer pulse to `game.step()` with one fixed delta
- [ ] Keep fixed tick value in config (current default `180ms`)
- [ ] Paint from current state in `paintEvent`, never mutate rules there
- [ ] Pause should stop rule updates but allow repaint/UI updates

---

## Acceptance criteria
- [ ] Snake moves consistently on fixed tick
- [ ] Input controls work (WASD/arrows, pause, quit)
- [ ] Eating food increases score and grows snake
- [ ] Food never appears on snake
- [ ] Game-over state triggers correctly on configured collision rules
- [ ] Restart works from game-over
- [ ] UI always reflects logic state (no rule logic in painter)
- [ ] `snake_tests` pass headless

---

## Test cases
- [ ] `game.step()` moves head one cell in current direction
- [ ] Opposite-direction reversal is rejected while moving
- [ ] Food eaten -> score +1 and snake length +1
- [ ] Food respawn avoids all occupied snake cells
- [ ] Self collision sets game over (when wrap mode still enabled)
- [ ] Wall collision behavior matches config (wrap or fail)
- [ ] Pause blocks state updates
- [ ] Restart resets state cleanly (score, snake, direction, food)

---

## Commands
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
./snake
./build/snake_tests
```
