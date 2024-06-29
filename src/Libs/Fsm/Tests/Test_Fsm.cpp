#include <utility>
#include <memory>
#include <string>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <Libs/Fsm/FsmBuilder.h>
#include <Libs/Utility/Exception.h>
#include <Libs/Utility/Log/Log.h>

using namespace tactics;

using ::testing::AtLeast;
using ::testing::Exactly;
using ::testing::Return;
using ::testing::Invoke;
using ::testing::AnyNumber;
using ::testing::_;

class MockLogger: public LogInstance {
public:
	MOCK_METHOD(void, log, (const LogCategory& category, LogLevel level, const std::string& message), (override));
	MOCK_METHOD(void, init, (LogLevel minimumLevel), (override));
};

class FsmTest: public testing::Test {
protected:
	FsmTest() {
		EXPECT_CALL(*_loggerPtr, init(_));
		EXPECT_CALL(*_loggerPtr, log(Log::Fsm, LogLevel::Trace, _))
			.Times(AnyNumber());
		Log::setLogInstance(std::move(_logger));
		Log::init();
	}

	void _addState(std::string_view name, std::unique_ptr<FsmState> state, FsmTransitions transitions = {}) {
		auto entry = std::make_unique<FsmStateEntry>();
		entry->name = HashId(name);
		entry->state = std::move(state);
		entry->transitions = std::move(transitions);
		_states.insert({HashId(name), std::move(entry)});
	}

	std::unique_ptr<Fsm> _createFsm(std::string_view startingStateName) {
		return std::make_unique<Fsm>(std::move(_states), HashId(startingStateName), nullptr);
	}

	FsmStateEntries _states;
	std::unique_ptr<MockLogger> _logger = std::make_unique<MockLogger>();
	MockLogger* _loggerPtr = _logger.get();
};

class MockFsmState: public FsmState {
public:
	MOCK_METHOD(FsmAction, enter, (), (override));
	MOCK_METHOD(FsmAction, update, (), (override));
	MOCK_METHOD(void, exit, (), (override));
};

TEST_F(FsmTest, WrongStartingStateWithEmptyFsm) {
	EXPECT_CALL(*_loggerPtr, log(Log::Fsm, LogLevel::Error, _))
		.Times(Exactly(1));
	_createFsm("InvalidState");
}

TEST_F(FsmTest, WrongStartingState) {
	EXPECT_CALL(*_loggerPtr, log(Log::Fsm, LogLevel::Error, _))
		.Times(Exactly(1));
	_addState("FirstState", std::make_unique<MockFsmState>());
	_addState("SecondState", std::make_unique<MockFsmState>());
	_addState("ThirdState", std::make_unique<MockFsmState>());
	_createFsm("FourthState");
}

TEST_F(FsmTest, CallEnterOnStartingState) {
	auto state = std::make_unique<MockFsmState>();
	EXPECT_CALL(*state.get(), enter())
		.Times(Exactly(1));
	EXPECT_CALL(*state.get(), update())
		.Times(Exactly(1));

	_addState("TestState", std::move(state));

	auto fsm = _createFsm("TestState");
	fsm->update();
}

TEST_F(FsmTest, PassThroughTest) {
	auto state1 = std::make_unique<MockFsmState>();
	auto state2 = std::make_unique<MockFsmState>();
	auto state3 = std::make_unique<MockFsmState>();

	EXPECT_CALL(*state1.get(), enter())
		.Times(Exactly(1))
		.WillRepeatedly(Return(FsmAction::transition("next"_id)));

	EXPECT_CALL(*state2.get(), enter())
		.Times(Exactly(1))
		.WillRepeatedly(Return(FsmAction::transition("next"_id)));

	EXPECT_CALL(*state3.get(), enter())
		.Times(Exactly(1))
		.WillRepeatedly(Return(FsmAction::transition("next"_id)));

	EXPECT_CALL(*state1.get(), exit()).Times(Exactly(1));
	EXPECT_CALL(*state2.get(), exit()).Times(Exactly(1));
	EXPECT_CALL(*state3.get(), exit()).Times(Exactly(1));

	_addState("FirstState", std::move(state1), {
		{"next"_id, {{"SecondState"_id}}}
	});
	_addState("SecondState", std::move(state2), {
		{"next"_id, {{"ThirdState"_id}}}
	});
	_addState("ThirdState", std::move(state3), {
		{"next"_id, {{Fsm::exitState}}}
	});

	auto fsm = _createFsm("FirstState");
	fsm->update();
	EXPECT_TRUE(fsm->hasReachedExitState());
}

TEST_F(FsmTest, TransitionAfterTwoUpdateCalls) {
	auto state1 = std::make_unique<MockFsmState>();
	auto state2 = std::make_unique<MockFsmState>();

	int count = 0;

	EXPECT_CALL(*state1.get(), enter()).Times(Exactly(1));
	EXPECT_CALL(*state1.get(), exit()).Times(Exactly(1));
	EXPECT_CALL(*state1.get(), update())
		.Times(Exactly(2))
		.WillRepeatedly([&count] {
		++count;
		return count <= 1 ? FsmAction::none() : FsmAction::transition("next"_id);
	}
	);

	EXPECT_CALL(*state2.get(), enter())
		.Times(Exactly(1))
		.WillRepeatedly(Return(FsmAction::transition("next"_id)));
	EXPECT_CALL(*state2.get(), exit()).Times(Exactly(1));
	EXPECT_CALL(*state2.get(), update()).Times(0);

	_addState("FirstState", std::move(state1), {
		{"next"_id, {{"SecondState"_id}}}
		});
	_addState("SecondState", std::move(state2), {
		{"next"_id, {{Fsm::exitState}}}
		});

	auto fsm = _createFsm("FirstState");
	EXPECT_FALSE(fsm->hasReachedExitState());
	fsm->update();
	EXPECT_FALSE(fsm->hasReachedExitState());
	fsm->update();
	EXPECT_TRUE(fsm->hasReachedExitState());
}

TEST_F(FsmTest, WrongTransitionName) {
	EXPECT_CALL(*_loggerPtr, log(Log::Fsm, LogLevel::Error, _))
		.Times(Exactly(1));

	auto state1 = std::make_unique<MockFsmState>();

	EXPECT_CALL(*state1.get(), enter());
	EXPECT_CALL(*state1.get(), update())
		.Times(Exactly(1))
		.WillRepeatedly([] { return FsmAction::transition("wrongTransition"_id); }
	);

	_addState("FirstState", std::move(state1), {
		{"next"_id, {{Fsm::exitState}}}
	});

	auto fsm = _createFsm("FirstState");
	fsm->update();
}

TEST_F(FsmTest, WrongTargetStateTransition) {
	EXPECT_CALL(*_loggerPtr, log(Log::Fsm, LogLevel::Error, _))
		.Times(Exactly(1));

	auto state1 = std::make_unique<MockFsmState>();

	EXPECT_CALL(*state1.get(), enter());
	EXPECT_CALL(*state1.get(), update())
		.Times(Exactly(1))
		.WillRepeatedly([] { return FsmAction::transition("next"_id); }
	);

	_addState("FirstState", std::move(state1), {
		{"next"_id, {{"WrongState"_id}}}
	});

	auto fsm = _createFsm("FirstState");
	fsm->update();
}
