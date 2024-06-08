#include "Material.h"

#include <Libs/Utility/Exception.h>

#include <ranges>

namespace tactics::resource {

std::shared_ptr<Material::Instance> Material::createInstance(std::shared_ptr<Material> parentMaterial) {
	auto instance = std::make_shared<Instance>(parentMaterial);
	return instance;
}

Material::Instance::Instance(std::shared_ptr<Material> parent): parent(parent) {
	if (!parent) {
		throw TACTICS_EXCEPTION("Cannot create a Material Instance. The provided parent material is null.");
	}
}

template<typename TContainer, typename TShader>
void _updateUniforms(TShader&& shader, TContainer&& parentContainer, TContainer&& instanceContainer) {
	for (auto&& [uniformName, value] : parentContainer) {
		if (auto itr = instanceContainer.find(uniformName); itr != instanceContainer.end()) {
			shader->setUniform(uniformName, itr->second);
		} else {
			shader->setUniform(uniformName, value);
		}
	}
}

void Material::Instance::updateShaderUniforms() {
	int textureIndex = 0;
	for (auto&& [uniformName, texture] : parent->getTextures()) {
		if (auto itr = getTextures().find(uniformName); itr != getTextures().end()) {
			itr->second->bind(textureIndex);
		} else {
			texture->bind(textureIndex);
		}

		parent->shader->setUniform(uniformName, textureIndex);
		++textureIndex;
	}

	_updateUniforms(parent->shader, parent->getInts(), getInts());
	_updateUniforms(parent->shader, parent->getFloats(), getFloats());
	_updateUniforms(parent->shader, parent->getVectors2(), getVectors2());
	_updateUniforms(parent->shader, parent->getVectors3(), getVectors3());
	_updateUniforms(parent->shader, parent->getVectors4(), getVectors4());
	_updateUniforms(parent->shader, parent->getMatrices(), getMatrices());
}

void Material::Instance::updateTextureTransparencyInfo() {
	useTransparency = false;

	for (auto&& [uniformName, texture] : parent->getTextures()) {
		if (auto itr = getTextures().find(uniformName); itr != getTextures().end()) {
			useTransparency |= itr->second->info.useTransparency;
		} else {
			useTransparency |= texture->info.useTransparency;
		}
	}
}

}
