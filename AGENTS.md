# Agents Guide (Snake C++)

This repo uses `PLAN.md` as the source of truth for what to build and in what order.
If you are a human contributor or an AI agent, follow this file to stay consistent.

## What to read first
- `PLAN.md`: project scope, directory tree, modules, checklist, acceptance criteria

## How to use the plan
- Treat `PLAN.md` as a checklist
- Work top-to-bottom: scaffold -> core logic -> tests -> SDL wiring -> polish
- Keep logic and platform code separated (see “Boundaries”)
- Update `PLAN.md` only if scope/order changes, not for every tiny edit

## Working rules (keep it clean)
- Prefer small, reviewable changes
- Don’t mix refactors with new features
- Add tests for pure logic (grid/snake/game rules) as you go
- Avoid introducing new dependencies unless they unlock a clear feature

## Before modifying anything, explicitly answer:

Am I changing game rules?

Am I changing input mapping?

Am I changing rendering only?

Am I changing timing behavior?

Only one of those per session.

Let the human review the code before pushing

## Module boundaries (important)
`PLAN.md` splits the code into two layers:

- Logic layer (no SDL)
  - `include/snake/*.hpp` and `src/game.cpp`, `src/grid.cpp`
  - Owns: rules, state updates, collisions, food spawning, score
- Platform layer (SDL only)
  - `src/input_sdl.cpp`, `src/renderer_sdl.cpp`, `src/main.cpp`
  - Owns: window/events, key mapping, drawing, font/audio loading

Rule of thumb: if it can be unit-tested, it should not depend on SDL.

## Suggested work order (from `PLAN.md`)
1. Build scaffold (CMake targets, warnings, formatting)
2. Core types (`types.hpp`) and `Grid`
3. `Snake` model and `Game` fixed-tick update
4. Unit tests for rules (headless)
5. SDL window + main loop
6. SDL input mapping
7. SDL renderer + UI overlays
8. Optional polish (sound, config loading, packaging notes)

## Definition of done
Use `PLAN.md` “Acceptance criteria” as the bar. At minimum:
- Eating food grows the snake and increments score
- Food never spawns on the snake
- Game over on wall/self collision; restart works
- Stable fixed-tick speed
- `snake_tests` runs headless and passes

## Commands (expected)
These match `PLAN.md`:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
./build/snake
./build/snake_tests
```

## Notes for AI agents
- Make the smallest change that moves the checklist forward
- When unsure about library choices (SDL2 vs raylib vs ncurses), ask once and keep going with the default in `PLAN.md`
- Avoid “helpful” rewrites of unrelated files
- If you add a new file, place it according to the tree in `PLAN.md`
