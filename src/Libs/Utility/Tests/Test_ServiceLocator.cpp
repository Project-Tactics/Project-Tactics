#include <Libs/Utility/Exception.h>
#include <Libs/Utility/Service/ServiceLocator.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <utility>

using namespace tactics;

class ServiceLocatorTest : public testing::Test {
protected:
	ServiceLocator _serviceLocator;
};

using ::testing::AtLeast;
using ::testing::Exactly;
using ::testing::Invoke;
using ::testing::Return;

class ExampleService {
public:
	void foo() {
		_value = 1;
	}

private:
	int _value;
};

TEST_F(ServiceLocatorTest, AddSameServiceClassTwice) {
	_serviceLocator.addService(new ExampleService());
	_serviceLocator.addService(new ExampleService());
}

TEST_F(ServiceLocatorTest, AddManyDifferentServices) {
	_serviceLocator.addService(new int());
	_serviceLocator.addService(new float());
	_serviceLocator.addService(new std::string());
	_serviceLocator.addService(new double());
}

TEST_F(ServiceLocatorTest, RetrieveService) {
	_serviceLocator.addService(new int());
	[[maybe_unused]] auto& service = _serviceLocator.getService<int>();
}

TEST_F(ServiceLocatorTest, RemoveService) {
	_serviceLocator.addService(new int());
	_serviceLocator.removeService(new int());
	EXPECT_THROW([[maybe_unused]] auto& service = _serviceLocator.getService<int>(), Exception);
}

TEST_F(ServiceLocatorTest, RemoveServiceWhichIsNotRegistered) {
	_serviceLocator.removeService(new int());
}
