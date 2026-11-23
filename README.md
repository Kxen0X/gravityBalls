# Gravity Balls

A C++ physics playground built with SFML 3. A ball falls under gravity, bounces against rotating circular arcs, and breaks through their openings. The project explores collision detection, vector math, and real-time rendering.

## Features

- Gravity-based motion with frame-time updates and ten physics substeps per frame.
- Collision response against the inner and outer sides of arcs, including their endpoints.
- Ball-to-ball collision handling when multiple balls are enabled in the source.
- Twenty generated arcs, rainbow color animation, and a borderless window sized to the desktop.
- Collision and arc-break sound effects; escaped arcs are removed from the scene.
- Keyboard control to randomize ball velocities.

The current configuration starts with **one ball** and a three-second pause before physics begins. The final development version is available in `main`.

## Controls

| Input | Action |
| --- | --- |
| **B** | Give each ball a new random velocity |
| **Esc** | Close the application |

## Build

Requirements: a C++17 compiler, CMake **3.28+**, Git, and Ninja. On Windows, use a terminal configured for your C++ compiler, such as the Visual Studio Developer Command Prompt.

CMake downloads and builds **SFML 3.0.2** through `FetchContent` and links the Graphics and Audio modules; the first configuration needs internet access. The executable target is named `main`.

```sh
git clone https://github.com/Kxen0X/gravityBalls.git
cd gravityBalls
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

On Debian/Ubuntu, SFML also needs its native development dependencies. The repository's [CI workflow](.github/workflows/ci.yml) lists the packages used for Linux builds.

## Run

Start the executable **with `build/bin` as the working directory**. The application uses paths such as `../resources/crack.mp3`; CMake copies the resource directory into `build/resources`.

Linux/macOS:

```sh
cd build/bin
./main
```

Windows PowerShell, from the repository root:

```powershell
Set-Location build/bin
.\main.exe
```

If using an IDE or another CMake generator, set the working directory explicitly. Running from a different directory can prevent the sound files from loading. Keep both `crack.mp3` and `touch.mp3` available: the current code does not safely handle failed audio loading.

## Explore the code

| File | Responsibility |
| --- | --- |
| [src/main.cpp](src/main.cpp) | Application entry point |
| [src/project.cpp](src/project.cpp) | Rendering loop, input, motion, and collision response |
| [headers/project.h](headers/project.h) | `Project`, `Ball`, and `Arc` definitions |
| [CMakeLists.txt](CMakeLists.txt) | SFML dependency, executable, and resource copying |
| [resources](resources) | Collision and arc-break audio |

To experiment, edit `maxCountOfCircles`, `gravity`, and the `arcsGeneration(...)` call in `updateInGame()`. Scene coordinates are currently hard-coded, so smaller displays may not show the entire scene.

## Project status

This is an educational simulation with custom collision handling. It does not include a general-purpose physics engine or a numerical accuracy test suite. Runtime settings, resolution-independent layout, and resource-load checks are useful next improvements.

## License and credits

The CMake setup is based on the [SFML CMake project template](https://github.com/SFML/cmake-sfml-project). See [LICENSE.md](LICENSE.md) for the repository's existing license text and attribution. The repository does not provide separate licensing information for every bundled media asset.
