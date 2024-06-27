#pragma once

#include <Libs/Utility/Exception.h>
#include <Libs/Utility/ClassId.h>
#include <Libs/Utility/Log/Log.h>

#include <unordered_map>
#include <memory>

namespace tactics {

struct Service {
public:
	virtual ~Service() = default;
};

template<typename TService>
struct WrappedService: public Service {
public:
	WrappedService(TService* service): _service(service) {
	}

	[[nodiscard]] TService& get() {
		return *_service;
	}

private:
	TService* _service;
};

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
			LOG_WARNING(Log::Engine, "Service is already registered. Class ID: {}", id);
			return;
		}
		_services[id] = std::make_unique<WrappedService<TService>>(service);
	}

	template<typename TService>
	void removeService(TService* service) {
		auto id = ClassId<TService>::ID();
		if (auto itr = _services.find(id); itr != _services.end()) {
			_services.erase(itr);
		} else {
			LOG_WARNING(Log::Engine, "Can't remove Service. Service not found. Class ID: {}", id);
			return;
		}
	}

	template<typename TService>
	[[nodiscard]] TService& getService() {
		auto id = ClassId<TService>::ID();
		if (auto itr = _services.find(id); itr != _services.end()) {
			return static_cast<WrappedService<TService>*>(itr->second.get())->get();
		} else {
			throw TACTICS_EXCEPTION("Can't find Service. Class ID: {}", id);
		}
	}

private:
	std::unordered_map<unsigned int, std::unique_ptr<Service>> _services;
};

}
