#include "CustomOverlayColors.h"

#include <Libs/Resource/IniFile/IniFile.h>
#include <Libs/Utility/Ini/IniMathConverter.h>

namespace tactics {

CustomOverlayColors::Colors CustomOverlayColors::_colors;
ImVec4 CustomOverlayColors::Colors::TitleTextColor;
ImVec4 CustomOverlayColors::Colors::SelectedButtonColor;
ImVec4 CustomOverlayColors::Colors::XComponentColor;
ImVec4 CustomOverlayColors::Colors::YComponentColor;
ImVec4 CustomOverlayColors::Colors::ZComponentColor;
ImVec4 CustomOverlayColors::Colors::TextureColor;
ImVec4 CustomOverlayColors::Colors::MeshColor;
ImVec4 CustomOverlayColors::Colors::MaterialColor;
ImVec4 CustomOverlayColors::Colors::ShaderColor;
ImVec4 CustomOverlayColors::Colors::IniFileColor;
ImVec4 CustomOverlayColors::Colors::ResourceColor;

void CustomOverlayColors::initialize(resource::IniFile& configFile) {
	auto category = "imguiCustomColors";
	_colors.TitleTextColor = configFile.get(category, "titleTextColor", ImVec4{1, 0.5f, 0.5f, 1.f});
	_colors.SelectedButtonColor = configFile.get(category, "buttonSelectedBkgColor", ImVec4{1, 0.5f, 0.5f, 1.f});
	_colors.XComponentColor = configFile.get(category, "xComponentColor", ImVec4{1, 0.5f, 0.5f, 1.f});
	_colors.YComponentColor = configFile.get(category, "yComponentColor", ImVec4{1, 0.5f, 0.5f, 1.f});
	_colors.ZComponentColor = configFile.get(category, "zComponentColor", ImVec4{1, 0.5f, 0.5f, 1.f});
	_colors.TextureColor = configFile.get(category, "textureColor", ImVec4{1, 0.5f, 0.5f, 1.f});
	_colors.MeshColor = configFile.get(category, "meshColor", ImVec4{1, 0.5f, 0.5f, 1.f});
	_colors.MaterialColor = configFile.get(category, "materialColor", ImVec4{1, 0.5f, 0.5f, 1.f});
	_colors.ShaderColor = configFile.get(category, "shaderColor", ImVec4{1, 0.5f, 0.5f, 1.f});
	_colors.IniFileColor = configFile.get(category, "iniFileColor", ImVec4{1, 0.5f, 0.5f, 1.f});
	_colors.ResourceColor = configFile.get(category, "resourceColor", ImVec4{1, 0.5f, 0.5f, 1.f});
}

const CustomOverlayColors::Colors& CustomOverlayColors::getColors() {
	return _colors;
}

}
