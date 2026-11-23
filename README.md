# Gravity Balls

A C++ physics playground built with SFML 3. A ball falls under gravity and bounces against rotating circular arcs. The project explores collision detection, vector math, and real-time rendering.

## Features

- Gravity-based motion with frame-time updates and ten physics substeps per frame.
- Collision response against the inner and outer sides of arcs, including their endpoints.
- Ball-to-ball collision handling when multiple balls are enabled in the source.
- Three rotating arcs and a borderless window sized to the desktop.
- Keyboard control to randomize ball velocities.

The current configuration starts directly in the simulation with **one ball**. Menu and end-screen code exists, but those branches are disabled in the main loop.

## Controls

| Input | Action |
| --- | --- |
| **B** | Give each ball a new random velocity |
| **Esc** | Close the application |

## Build

Requirements: a C++17 compiler, CMake **3.28+**, Git, and Ninja. On Windows, use a terminal configured for your C++ compiler, such as the Visual Studio Developer Command Prompt.

CMake downloads and builds **SFML 3.0.2** through `FetchContent`; the first configuration needs internet access. The executable target is named `main`.

```sh
git clone https://github.com/Kxen0X/gravityBalls.git
cd gravityBalls
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

On Debian/Ubuntu, SFML also needs its native development dependencies. The repository's [CI workflow](.github/workflows/ci.yml) lists the packages used for Linux builds.

## Run

Start the executable **with `build/bin` as the working directory**. The application uses paths such as `../resources/sword2.png`; CMake copies the resource directory into `build/resources`.

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

If using an IDE or another CMake generator, set the working directory explicitly. Running from a different directory can prevent textures and fonts from loading.

## Explore the code

| File | Responsibility |
| --- | --- |
| [src/main.cpp](src/main.cpp) | Application entry point |
| [src/project.cpp](src/project.cpp) | Rendering loop, input, motion, and collision response |
| [headers/project.h](headers/project.h) | `Project`, `Ball`, and `Arc` definitions |
| [CMakeLists.txt](CMakeLists.txt) | SFML dependency, executable, and resource copying |
| [resources](resources) | Textures, font, and other assets |

To experiment, edit `maxCountOfCircles`, `gravity`, and the `createArc(...)` calls in `Project::Project()`. Scene coordinates and UI positions are currently hard-coded, so smaller displays may not show the entire scene.

## Project status

This is an educational simulation with custom collision handling. It does not include a general-purpose physics engine or a numerical accuracy test suite. Runtime settings, resolution-independent layout, and resource-load checks are useful next improvements.

## License and credits

The CMake setup is based on the [SFML CMake project template](https://github.com/SFML/cmake-sfml-project). See [LICENSE.md](LICENSE.md) for the repository's existing license text and attribution. The repository does not provide separate licensing information for every bundled media asset.
