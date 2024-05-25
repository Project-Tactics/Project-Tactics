#pragma once

#include <Libs/Utilities/Exception.h>
#include <Libs/Utilities/ClassId.h>

#include <unordered_map>

namespace tactics {

/**
* @brief A service locator is a way to provide services to objects without having to pass them in the constructor.
* The class does not become the owner of the services, it just holds pointers to them.
* There's a risk that we might abuse this class by registering any thing just to have some sort of global repository for objects.
* However, I believe that the risk is low, as the class is not meant to be used as a general purpose repository anyway.
*/
class ServiceLocator {
public:
	template<typename TService>
	void addService(TService* service) {
		auto id = ClassId<TService>::ID();
		if (_services.contains(id)) {
			throw Exception("Service is already registered. Class ID: {}", id);
		}
		_services[id] = service;
	}

	template<typename TService>
	void removeService(TService* service) {
		auto id = ClassId<TService>::ID();
		if (auto itr = _services.find(id); itr != _services.end()) {
			_services.erase(itr);
		} else {
			throw Exception("Can't remove Service. Service not found. Class ID: {}", id);
		}
	}

	template<typename TService>
	[[nodiscard]] TService& getService() {
		auto id = ClassId<TService>::ID();
		if (auto itr = _services.find(id); itr != _services.end()) {
			return static_cast<TService&>(*reinterpret_cast<TService*>(itr->second));
		} else {
			throw Exception("Can't find Service. Class ID: {}", id);
		}
	}

private:
	// TODO(Gerark) Using void* might be perceived as a suboptimal choice. One alternative could be creating a base class for services
	// and mandating all services to inherit from it.
	// However, I propose delaying this decision until we encounter a situation where adding additional functionality to the base class becomes necessary.
	std::unordered_map<unsigned int, void*> _services;
};

}
