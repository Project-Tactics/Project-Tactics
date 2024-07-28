#pragma once

#include "SamplesUtils.h"

#include <Engine/Core/Application.h>

#include <Libs/Utility/String/String.h>

namespace tactics {

class SamplesApplication : public tactics::Application {
public:
	SamplesApplication();
	void setupComponentReflections() override;
	HashId initialize(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) override;

private:
	template<typename TSampleState>
	void _addSampleFlow(std::string_view key,
						std::string_view sampleName,
						std::string_view resourceFileExtension = ".json") {
		_sampleFlows[std::string(key)] = std::make_unique<TSampleFlow<TSampleState>>(sampleName, resourceFileExtension);
	}

	SampleFlows _sampleFlows;
};

} // namespace tactics

