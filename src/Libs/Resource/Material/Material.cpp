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

[[nodiscard]] std::shared_ptr<Material::Instance> Material::Instance::clone() {
	auto instance = std::make_shared<Material::Instance>(parent);
	instance->set(getInts());
	instance->set(getFloats());
	instance->set(getVectors2());
	instance->set(getVectors3());
	instance->set(getVectors4());
	instance->set(getMatrices());
	instance->set(getTextures());
	return instance;
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

	Material::updateUniforms(parent->shader, parent->getInts(), getInts());
	Material::updateUniforms(parent->shader, parent->getFloats(), getFloats());
	Material::updateUniforms(parent->shader, parent->getVectors2(), getVectors2());
	Material::updateUniforms(parent->shader, parent->getVectors3(), getVectors3());
	Material::updateUniforms(parent->shader, parent->getVectors4(), getVectors4());
	Material::updateUniforms(parent->shader, parent->getMatrices(), getMatrices());
}

}
