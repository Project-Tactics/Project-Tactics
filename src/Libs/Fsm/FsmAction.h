#pragma once

#include <Libs/Utility/HashId.h>

namespace tactics {

class FsmAction {
public:
	static FsmAction none();
	static FsmAction transition(const HashId& transitionName);

	const HashId& transitionName() const;
	bool hasTransition() const;

private:
	HashId _transitionName;
};

class FsmEventAction {
public:
	static FsmEventAction none();
	static FsmEventAction capture();
	static FsmEventAction transition(const HashId& transitionName);

	const HashId& transitionName() const;
	bool wantsToCaptureInput() const;
	bool hasTransition() const;

private:
	HashId _transitionName;
	bool _captureInput{};
};

} // namespace tactics
