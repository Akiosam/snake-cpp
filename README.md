# Snake C++

Qt Widgets snake game with clean logic boundaries.

---

## For players

Users do not need CMake or a compiler.
Download the latest package from GitHub Releases, unzip it, and run the app binary (`snake.exe` on Windows, `snake` on Linux).

---

## Build it

Use CMake to configure and compile the app plus tests.

Linux/macOS:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

Windows (Visual Studio generator):

```powershell
cmake -S . -B build
cmake --build build --config Release
```

---

## Run it

Start the desktop app from the build output directory.

Linux/macOS:

```sh
./build/bin/snake
```

Windows:

```powershell
.\build\bin\snake.exe
```

---

## Test it

Run the headless test target to verify game logic.

Linux/macOS:

```sh
./build/bin/snake_tests
```

Windows:

```powershell
.\build\bin\snake_tests.exe
```

---

## Package it

Create a distributable archive that includes the executable.

Linux/macOS:

```sh
cpack --config build/CPackConfig.cmake
```

Windows:

```powershell
cpack --config build/CPackConfig.cmake -C Release
```

GitHub Actions also builds and publishes release packages automatically when you push a `v*` tag.

---

## Control it

Use `WASD` or arrow keys to set direction.
Press `P` to pause/resume and `Q` or `Esc` to quit.

---

## Track progress

Follow `PLAN.md` for scope, order, and acceptance criteria.
Read `docs/how-it-works.md` for architecture and flow details.
