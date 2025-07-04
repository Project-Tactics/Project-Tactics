#pragma once

#include <Libs/Resource/Material/Material.h>
#include <Libs/Resource/Resource.h>
#include <Libs/Resource/ResourceProvider.h>
#include <Libs/Utility/Exception.h>
#include <Libs/Utility/Json/JsonSerialization.h>

namespace tactics::resource {

/*
 * We're going to rely on this global function to be able to seriaize resources.
 * We do serialize a resource by its hash id, and we need to be able to get the resource
 */
void setResourceProviderForSerialization(tactics::resource::ResourceProvider* provider);

} // namespace tactics::resource

namespace nlohmann {

struct resource_serializer_base {
protected:
	static tactics::resource::ResourceProvider* _getResourceProvider();
};

template<tactics::resource::IsResource T> struct adl_serializer<std::shared_ptr<T>> : public resource_serializer_base {
	static void to_json(json&, const std::shared_ptr<T>&) {
		TACTICS_EXCEPTION("Serializing resources is not yet implemented");
	}

	static void from_json(const json& json, std::shared_ptr<T>& resource) {
		resource = _getResourceProvider()->getResource<T>(json.get<tactics::HashId>());
	}
};

template<> struct adl_serializer<tactics::resource::MaterialInstances> {
	static void to_json(json&, const tactics::resource::MaterialInstances&);
	static void from_json(const json& json, tactics::resource::MaterialInstances& materialInstances);
};

} // namespace nlohmann
