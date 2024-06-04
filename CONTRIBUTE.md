# Project Structure
The project is organized into three main layers.
From top to bottom:
### **ENGINE**
The engine folder contains libraries that are more opinionated on how to use the internal libraries from libs. For example, it exposes a SceneSystem responsible for creating game characters, maps, and spawn effects. The engine libraries depend on one or more libs.
### **LIBS**
All the internal libraries intended for use by the engine. These libraries provide core functionalities and are the foundational layer of the project.
### **THIRDPARTY**
The thirdparty folder contains libraries developed by others. These external libraries can be used by both the libs and engine folders. We aim to ensure all third-party libraries included are at least MIT compliant. Please check and adhere to the respective licenses of these libraries.

# Coding Style

### C++
#### EditorConfig
- You can inspect the `.editorconfig` file to know more about the formatting rules.
- Enable the `format on save` option on your IDE. Just be sure you're not overwriting the default rules.
#### Spacing
- Use 4 spaces for indentation.
- Place the opening brace `{` on the same line as the function or control statement.
- Place the closing brace `}` on a new line.
#### Naming
- Use `camelCase` for variable names.
- Use `camelCase` for function names.
- Namespace names should be all `lowercase`.
- Use `PascalCase` for class and type names.
- use `_camelCase` for `private` member variables and functions (prefixed with `_`)
- When using acronyms in names, capitalize only the first letter ( `FSMState` -> `FsmState`, `ECSComponent` -> `EcsComponent` )
#### Classes
- Use only one section per access modifier. One `public:`, `private:` and `protected` accessor.
- Use default implementation for empty virtual destructor: `virtual ~Destructor() = default;`
- When overriding a virtual function just use the override keyword: `void method() override;`
#### Functions
- Force `[[nodiscard]]` attribute whenever possible.
- Do not use `const` by-value parameters.
#### Auto Keyword
- Use `auto` whenever is possible.
#### Namespace
- Use `using namespace` for relevant namespaces at the beginning of functions to avoid repetitive namespace qualifications.
#### Comments
- Use comments only to explain why we're doing something that might not be clear enough just by looking at the code.
- Use comments to give indication of something that could be improved. Provide a github issue link so we can properly keep track of it.
- Use comments to specify `TODO` in the form of `TODO(GithubUsername) MESSAGE`. If you can, provide a github issue link.
- Do not keep commented code around, just get rid of those lines.
#### Control Structures
- Place the else statement on the same line as the closing brace of the if block.
### CMake
emember to treat CMake files as code; they should be reviewed and properly commented.


### WIP ...