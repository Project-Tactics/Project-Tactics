#pragma once

#include <string_view>
#include <string>
#include <cassert>

namespace tactics {

class FsmAction {
public:
	static FsmAction none();
	static FsmAction transition(std::string_view transitionName);

	operator bool() const;
	bool operator!() const;
	const std::string& transitionName() const;

private:
	std::string _transitionName;
};

class FsmInputAction {
public:
	static FsmInputAction none();
	static FsmInputAction transition(std::string_view transitionName);
	static FsmInputAction consume();

	bool isConsumed() const;
};

}
