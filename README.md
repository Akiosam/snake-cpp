# Snake C++

Qt Widgets snake game with clean logic boundaries.

---

## Build it

Use CMake to configure and compile the app plus tests.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

---

## Run it

Start the desktop app from the project root.

```sh
./snake
```

---

## Test it

Run the headless test target to verify game logic.

```sh
./build/snake_tests
```

---

## Control it

Use `WASD` or arrow keys to set direction.
Press `P` to pause/resume and `Q` or `Esc` to quit.

---

## Track progress

Follow `PLAN.md` for scope, order, and acceptance criteria.
Read `docs/how-it-works.md` for architecture and flow details.
