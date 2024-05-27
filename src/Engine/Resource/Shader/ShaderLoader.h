#pragma once

#include <Libs/Resource/ResourceLoader.h>
#include "Shader.h"

namespace tactics::resource {

struct ShaderDescriptor {
	std::string name;
	std::string vertexShaderPath;
	std::string fragmentShaderPath;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(ShaderDescriptor, name, vertexShaderPath, fragmentShaderPath);
};

class ShaderLoader: public ResourceLoader {
public:
	std::shared_ptr<Shader> load(const ShaderDescriptor& descriptor);

private:
	std::string _loadFile(const std::string& filePath);
	unsigned int _loadProgram(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
	unsigned int _loadShader(unsigned int shaderType, const std::string& shaderFilePath);
};

}
