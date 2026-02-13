# Qt playfield

Current GUI prototype with timed snake movement.

---

## Run it

Configure and build with CMake, then launch the root binary.

```sh
cmake -S . -B build
cmake --build build -j
./snake
```

---

## Use controls

Press WASD or Arrow keys to set direction.
Press `P` to pause/resume and `Q` or `Esc` to quit.

---

## Understand movement

A `QTimer` ticks every 180 ms and advances the snake one cell.
Movement uses the current direction and wraps at grid edges.

---

## See rendering

The board is a centered `20x15` grid drawn in `src/main.cpp`.
Snake segments come from a `std::deque<QPoint>` and are painted each refresh.

---

## Note limits

Food is currently static and does not affect length.
Collision, scoring, and restart flow are not implemented yet.
