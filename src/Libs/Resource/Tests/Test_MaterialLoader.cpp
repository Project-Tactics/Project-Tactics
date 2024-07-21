#include "../Material/MaterialLoader.h"
#include "../Shader/Shader.h"
#include "../Texture/Texture.h"
#include "Test_Fixture.h"

#include <Libs/Utility/Exception.h>

TEST_F(ResourceTest, CreateMaterialLoader) {
	[[maybe_unused]] MaterialLoader loader(*_fileSystem, *_resourceSystem);
}

TEST_F(ResourceTest, LoadEmptyMaterialDescriptor) {
	MockResourceProvider mockProvider;
	EXPECT_CALL(mockProvider, getResource(ResourceType::Shader, ""_id)).Times(0);

	MaterialLoader loader(*_fileSystem, mockProvider);

	MaterialDescriptor desc;
	EXPECT_THROW([[maybe_unused]] auto material = loader.load(desc), Exception);
}

TEST_F(ResourceTest, LoadMaterialWithValidShader) {
	MockResourceProvider mockProvider;
	EXPECT_CALL(mockProvider, getResource(ResourceType::Shader, "testShader"_id))
		.WillOnce(Return(std::make_shared<Shader>("testShader"_id)));

	[[maybe_unused]] MaterialLoader loader(*_fileSystem, mockProvider);

	MaterialDescriptor desc{.shader = "testShader"};
	auto material = loader.load(desc);
	EXPECT_NE(material->shader, nullptr);
	EXPECT_TRUE(material->getInts().empty());
	EXPECT_TRUE(material->getFloats().empty());
	EXPECT_TRUE(material->getMatrices().empty());
	EXPECT_TRUE(material->getVectors2().empty());
	EXPECT_TRUE(material->getVectors3().empty());
	EXPECT_TRUE(material->getVectors4().empty());
}

TEST_F(ResourceTest, LoadMaterialWithAlphaBlend) {
	MockResourceProvider mockProvider;
	EXPECT_CALL(mockProvider, getResource(ResourceType::Shader, "testShader"_id))
		.WillOnce(Return(std::make_shared<Shader>("testShader"_id)));

	[[maybe_unused]] MaterialLoader loader(*_fileSystem, mockProvider);

	MaterialDescriptor desc{.shader = "testShader", .alphaBlended = true};
	auto material = loader.load(desc);
	EXPECT_TRUE(material->hasAlphaBlend);
}

TEST_F(ResourceTest, LoadMaterialWithCustomBuiltinUniforms) {
	MockResourceProvider mockProvider;
	EXPECT_CALL(mockProvider, getResource(ResourceType::Shader, "testShader"_id))
		.WillOnce(Return(std::make_shared<Shader>("testShader"_id)));

	[[maybe_unused]] MaterialLoader loader(*_fileSystem, mockProvider);

	MaterialDescriptor desc{
		.shader = "testShader",
		.uniforms = {.ints = {{"int1", 1}, {"int2", 2}},
					 .floats = {{"float1", 1.0f}, {"float2", 2.0f}},
					 .matrices = {{"matrix1", glm::mat4(1.0f)}, {"matrix2", glm::mat4(2.0f)}},
					 .vectors2 = {{"vec2_1", glm::vec2(0.0f, 1.0f)},
								  {"vec2_2", glm::vec2(2.0f, 3.0f)},
								  {"vec2_3", glm::vec2(2.0f, 3.0f)}},
					 .vectors3 = {{"vec3_1", glm::vec3(0.0f, 1.0f, 2.0f)}, {"vec3_2", glm::vec3(2.0f, 3.0f, 3.0f)}},
					 .vectors4 = {{"vec4_1", glm::vec4(0.0f, 1.0f, 2.0f, 5.f)}}}};
	auto material = loader.load(desc);
	EXPECT_EQ(material->getInts().size(), 2);
	EXPECT_EQ(material->getFloats().size(), 2);
	EXPECT_EQ(material->getMatrices().size(), 2);
	EXPECT_EQ(material->getVectors2().size(), 3);
	EXPECT_EQ(material->getVectors3().size(), 2);
	EXPECT_EQ(material->getVectors4().size(), 1);
}

TEST_F(ResourceTest, LoadMaterialWithTextureUniforms) {
	MockResourceProvider mockProvider;
	EXPECT_CALL(mockProvider, getResource(ResourceType::Shader, "testShader"_id))
		.Times(Exactly(1))
		.WillOnce(Return(std::make_shared<Shader>("testShader"_id)));

	EXPECT_CALL(mockProvider, getResource(ResourceType::Texture, "myTexture"_id))
		.Times(Exactly(2))
		.WillRepeatedly(Return(std::make_shared<Texture>("myTexture"_id)));

	[[maybe_unused]] MaterialLoader loader(*_fileSystem, mockProvider);

	MaterialDescriptor desc{.shader = "testShader",
							.uniforms = {.textures = {{"texture1", "myTexture"}, {"texture2", "myTexture"}}}};

	auto material = loader.load(desc);
	EXPECT_EQ(material->getTextures().size(), 2);
}
