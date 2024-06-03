# Project-Tactics
An open-source game development framework inspired by Final Fantasy Tactics, letting you create your own unique tactical RPG adventures. The project goal is to allow users to create a set of battlegrounds to share with other players.

Please checkout the [Design References](https://github.com/Project-Tactics/Project-Tactics/blob/main/DESIGNREFERENCE.md)!

## Supported Platforms
We currently plan to support only Windows. Expanding and maintaining support for other platforms would require a dedicated person, which isn't feasible for us at this time.

## How to Build
### Generate solution/project
```bash
git clone https://github.com/Project-Tactics/Project-Tactics.git
cd Project-Tactics
git submodule update --init --recursive
cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CONFIGURATION_TYPES=Debug -S ./ -B ./build
```
### Build
```bash
cmake --build ./build --config Debug
```
### Run
```
cd .\build\src\Apps\OpenTactica\Debug
.\OpenTactica.exe
```
