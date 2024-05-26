#pragma once

#include <Libs/Resource/ResourceManager.h>
#include "Shader.h"

namespace tactics::resource {

class ShaderManager: public TResourceManager<Shader> {
public:
	using TResourceManager<Shader>::TResourceManager;
	ResourceId load(const nlohmann::json& descriptor) override;

private:
	std::string _loadFile(const std::string& filePath);
	unsigned int _loadProgram(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
	unsigned int _loadShader(unsigned int shaderType, const std::string& shaderFilePath);
};

}
