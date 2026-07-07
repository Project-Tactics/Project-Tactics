# AGENTS.md

## Operating Rules

Do not change code, resources, build files, scripts, or project metadata unless the user explicitly grants permission for
that task.

Default to read-only analysis. When a change is requested, first understand the affected subsystem and call out risky
assumptions before editing. Be suspicious of lifecycle, resource ownership, global state, and generated files.

Never revert user changes unless the user explicitly asks. Do not edit `build/`, generated Visual Studio/CMake files, or
third-party code unless the request specifically targets them.

## Project Summary

Project-Tactics is a C++23 tactical RPG/game framework inspired by Final Fantasy Tactics. It is currently structured as
an engine/framework plus sample and prototype applications.

Main areas:
- `src/Engine`: runtime orchestration, application lifecycle, scene helpers, engine/debug overlays.
- `src/Libs`: reusable engine subsystems: ECS, resources, rendering, input, physics, FSM, filesystem, events, overlays,
  utility, UI.
- `src/Apps/Samples`: broad demo harness for engine features.
- `src/Apps/OpenTacticaPrototype`: early tactical-RPG gameplay prototype.
- `src/Apps/OpenTactica`: minimal app shell.
- `resources`: per-app data copied beside executable output.
- `thirdparty`: vendored/submodule/prebuilt dependencies.
- `build`: generated local build output. Ignore it.

## Build And Validation

The project is Windows/MSVC-oriented and uses CMake with Visual Studio 2022.

Fresh configure/build via presets:
```powershell
cmake --preset Debug
cmake --build --preset Debug
```

CI-style build commands may also use:
```powershell
cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Debug -S ./ -B ./build/Debug
cmake --build ./build/Debug --config Debug
```

Unit tests are aggregated in `TacticsEngine_UnitTests` through GTest. Run tests only after the test executable has been
built:
```powershell
ctest --output-on-failure --test-dir ./build/Debug/src/Apps/TacticsEngine_UnitTests
```

Do not run configure/build/test commands that download dependencies or write substantial build output unless the user has
approved that work.

## Formatting

C++ formatting is defined by `src/.clang-format`.

Use the repository formatter for C++ changes:
```powershell
python tools/clang-format.py --dir=src/
```

Important style points:
- C++23 project.
- Tabs are intentional.
- Column limit is 120.
- Include ordering is configured for local includes, `Engine/*`, `Libs/*`, then external includes.

## Architecture Notes

The engine uses a fixed-tick simulation loop and renders every frame. The runtime registers systems in a `ServiceLocator`;
app states retrieve systems from it.

The resource system is pack-based. Resource definitions live in JSON files such as `engine_data.json` and
`common/resources.json`. App data is copied from `resources/<AppName>/data` to the executable directory after build.

Resource lifetime is strict:
- Duplicate resource names/ids are errors.
- Unloading a resource still referenced by a `shared_ptr` is an error.
- Engine shutdown fails if resources remain loaded.
- Engine shutdown also fails if warning-or-higher logs occurred.

Treat resource pack loading/unloading, shared resource references, and component reflection registration as high-risk
areas.

## Risk Areas

Be especially careful with:
- `src/Libs/Resource`: pack lifetime, resource manager registration, shared ownership.
- `src/Libs/Ecs`: prefab/scene registry behavior and component reflection.
- `src/Engine/Core/Engine.cpp`: initialization order, update order, shutdown order.
- `src/Libs/Rendering`: SDL/OpenGL/ImGui lifecycle.
- `src/Libs/Input`: SDL event processing and input-map resources.
- `src/Apps/OpenTacticaPrototype`: prototype gameplay assumptions may not be scale-safe.
- Global/static systems such as `Log`, `Random`, `EngineTime`, and resource serialization provider.

Known suspicious prototype issue: `BattleSystem::advanceTick` currently uses a fixed 64-entity array for ready entities.
Do not expand gameplay scale around that assumption without addressing capacity.

## Tests

Existing tests cover FSM, resource packs, material loading, ECS basics, service locator, strings, and frame timer
behavior.

Some tests use death tests and global state. Avoid adding hidden dependencies between tests. Reset or isolate global
state when practical.

For resource-related changes, prefer focused tests using headless resource/rendering targets where available.

## Dependencies

Some dependencies are submodules or vendored under `thirdparty`; others are fetched by CMake through `FetchContent` or
`ExternalProject`.

Do not modify third-party source, submodule contents, or prebuilt dependency files unless explicitly asked.

Network may be required for fresh CMake configuration because JSON, Jolt, GTest, cpptrace, and Assimp may be downloaded.

## Scripts

`tools/app/create-app.py` and `tools/sample/create-sample.py` mutate source, resource, and CMake files using exact text
markers. Treat them as convenience scaffolding, not safe refactoring tools. Ask before running them.

## Git Notes

If Git reports dubious ownership for this repository, use a scoped safe-directory option for read-only commands:
```powershell
git -c safe.directory=D:/Work/Dev/Games/Project-Tactics status --short --branch
```

Do not change global Git config unless the user explicitly approves.
