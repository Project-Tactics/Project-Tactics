# Project-Tactics
An open-source game development framework inspired by Final Fantasy Tactics, empowering creators to craft their own unique tactical RPG experiences.

## Supported Platforms
We currently plan to support only Windows. Expanding and maintaining support for other platforms would require a dedicated person, which isn't feasible for us at this time.

## How to Build
### Generate solution/project
```bash
git clone https://github.com/Project-Tactics/Project-Tactics.git
cd Project-Tactics
cmake -G "Visual Studio 17 2022" -A x64 -S ./ -B ./build
```
### Build
```bash
cmake --build ./build --config Debug
```
### Run
```
./build/src/Apps/ProjectTactics/Debug/ProjectTactics.exe
```
