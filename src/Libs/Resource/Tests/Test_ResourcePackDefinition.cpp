#include "Test_Fixture.h"
#include <Libs/Utility/Exception.h>

TEST_F(ResourceTest, LoadInvalidPackDefinition) {
	givenJsonFileCreation();
	givenPackDefinitionExists(false);
	expectJsonFileLoad(0);

	EXPECT_THROW(_resourceSystem->loadPackDefinition("TestPackDefinition.json"), Exception);
}

TEST_F(ResourceTest, LoadEmptyPackDefinition) {
	givenJsonFileCreation();
	givenPackDefinitionExists(true);
	expectJsonFileLoad(1);

	EXPECT_THROW(_resourceSystem->loadPackDefinition("TestPackDefinition.json"), Exception);
}

TEST_F(ResourceTest, LoadValidPackDefinition) {
	givenJsonFileCreation();
	givenDefaultPackDefinition();
	givenPackDefinitionExists(true);
	expectJsonFileLoad(1);

	_resourceSystem->loadPackDefinition("TestPackDefinition.json");

	auto numberOfPacks = 0;
	_resourceSystem->forEachPack([&numberOfPacks] (auto&) {
		++numberOfPacks;
	});

	EXPECT_EQ(numberOfPacks, 1);
}

TEST_F(ResourceTest, LoadSamePackDefinitionTwice) {
	givenJsonFileCreation();
	givenDefaultPackDefinition();
	givenPackDefinitionExists(true);
	expectJsonFileLoad(1);

	_resourceSystem->loadPackDefinition("TestPackDefinition.json");

	givenJsonFileCreation();
	givenDefaultPackDefinition();
	givenPackDefinitionExists(true);
	expectJsonFileLoad(1);
	EXPECT_THROW(_resourceSystem->loadPackDefinition("TestPackDefinition.json"), Exception);
}
