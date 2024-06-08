#pragma once

#include <imgui.h>

namespace tactics {
namespace resource {
class IniFile;
}

class CustomOverlayColors {
public:
	struct Colors {
		static ImVec4 TitleTextColor;
		static ImVec4 SelectedButtonColor;
		static ImVec4 XComponentColor;
		static ImVec4 YComponentColor;
		static ImVec4 ZComponentColor;
		static ImVec4 IniFileColor;
		static ImVec4 MaterialColor;
		static ImVec4 MeshColor;
		static ImVec4 PrefabColor;
		static ImVec4 ResourceColor;
		static ImVec4 ShaderColor;
		static ImVec4 TextureColor;
	};

	static void initialize(resource::IniFile& configFile);
	static const Colors& getColors();

private:
	static Colors _colors;
};

}
