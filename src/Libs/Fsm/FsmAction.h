#pragma once

#include <string_view>
#include <string>
#include <cassert>

namespace tactics {

class FsmAction {
public:
	static FsmAction none();
	static FsmAction transition(std::string_view transitionName);

	const std::string& transitionName() const;
	bool hasTransition() const;

private:
	std::string _transitionName;
};

class FsmEventAction {
public:
	static FsmEventAction none();
	static FsmEventAction capture();
	static FsmEventAction transition(std::string_view transitionName);

	const std::string& transitionName() const;
	bool wantsToCaptureInput() const;
	bool hasTransition() const;

private:
	std::string _transitionName;
	bool _captureInput{};
};

}
