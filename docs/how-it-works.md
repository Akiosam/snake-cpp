# How it works

Quick map of flow, layers, and behavior.

---

## Follow flow

`src/main.cpp` starts `QApplication`, creates the widget, and enters the Qt event loop.
Game updates run on a fixed timer pulse and drawing happens in widget paint events.

---

## Keep boundaries

Core rules live in `src/game.cpp` and related non-UI files.
Qt code in `src/qt/*` handles input, timer wiring, and rendering only.

---

## Read state

The board uses a `20x15` cell grid and snake segments are stored in a deque.
Direction, movement, and game status come from shared types in `src/types.hpp`.

---

## Trigger input

Controls map keys to actions in the Qt input adapter.
Current mapping supports `WASD`, arrows, `P` for pause, and `Q`/`Esc` for quit.

---

## Step timing

The default tick is `180ms` for stable movement pacing.
Pause blocks rule updates while leaving UI refresh active.

---

## Verify behavior

Build with CMake, run `./snake`, then run `./build/snake_tests`.
Use acceptance checks in `PLAN.md` to confirm expected gameplay behavior.
