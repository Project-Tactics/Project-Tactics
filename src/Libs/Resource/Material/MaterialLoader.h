#pragma once

#include "Material.h"

#include <Libs/Resource/ResourceLoader.h>
#include <Libs/Utility/Uniforms/UniformsDescriptor.h>

namespace tactics::resource {

struct MaterialDescriptor {
    std::string shader;
    bool alphaBlended{};
    UniformsDescriptor uniforms;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(MaterialDescriptor, shader, alphaBlended, uniforms);
};

class MaterialLoader : public ResourceLoader {
    public:
    using ResourceLoader::ResourceLoader;
    [[nodiscard]] std::shared_ptr<Material> load(const MaterialDescriptor &descriptor);
};

}
