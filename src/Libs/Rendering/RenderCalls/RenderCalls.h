#pragma once

#include <glm/glm.hpp>

namespace tactics {
namespace render::pipeline {

enum class BufferType {
	VertexBuffer,
	IndexBuffer
};

enum class BufferUsage {
	StaticDraw,
	DynamicDraw,
	StreamDraw
};

enum class TextureType {
	Texture2D
};

enum class PrimitiveType {
	Triangles,
	TriangleStrip,
	TriangleFan,
	Lines,
	LineStrip,
	LineLoop,
	Points
};

template<BufferType bufferType> class BufferTypeValue {};
template<> class BufferTypeValue<BufferType::VertexBuffer> {
public: static const unsigned int value; static const BufferType type = BufferType::VertexBuffer;
};
template<> class BufferTypeValue<BufferType::IndexBuffer> {
public: static const unsigned int value; static const BufferType type = BufferType::IndexBuffer;
};

template<BufferUsage bufferUsage> class BufferUsageValue {};
template<> class BufferUsageValue<BufferUsage::StaticDraw> {
public: static const unsigned int value; static const BufferUsage type = BufferUsage::StaticDraw;
};
template<> class BufferUsageValue<BufferUsage::DynamicDraw> {
public: static const unsigned int value; static const BufferUsage type = BufferUsage::DynamicDraw;
};
template<> class BufferUsageValue<BufferUsage::StreamDraw> {
public: static const unsigned int value; static const BufferUsage type = BufferUsage::StreamDraw;
};

template<TextureType textureType> class TextureTypeValue {};
template<> class TextureTypeValue<TextureType::Texture2D> {
public: static const unsigned int value; static const TextureType type = TextureType::Texture2D;
};

template<PrimitiveType primitiveType> class PrimitiveTypeValue {};
template<> class PrimitiveTypeValue<PrimitiveType::Triangles> {
public: static const unsigned int value; static const PrimitiveType type = PrimitiveType::Triangles;
};
template<> class PrimitiveTypeValue<PrimitiveType::TriangleStrip> {
public: static const unsigned int value; static const PrimitiveType type = PrimitiveType::TriangleStrip;
};
template<> class PrimitiveTypeValue<PrimitiveType::TriangleFan> {
public: static const unsigned int value; static const PrimitiveType type = PrimitiveType::TriangleFan;
};
template<> class PrimitiveTypeValue<PrimitiveType::Lines> {
public: static const unsigned int value; static const PrimitiveType type = PrimitiveType::Lines;
};
template<> class PrimitiveTypeValue<PrimitiveType::LineStrip> {
public: static const unsigned int value; static const PrimitiveType type = PrimitiveType::LineStrip;
};
template<> class PrimitiveTypeValue<PrimitiveType::LineLoop> {
public: static const unsigned int value; static const PrimitiveType type = PrimitiveType::LineLoop;
};
template<> class PrimitiveTypeValue<PrimitiveType::Points> {
public: static const unsigned int value; static const PrimitiveType type = PrimitiveType::Points;
};

using VertexBuffer = BufferTypeValue<BufferType::VertexBuffer>;
using IndexBuffer = BufferTypeValue<BufferType::IndexBuffer>;

using StaticDraw = BufferUsageValue<BufferUsage::StaticDraw>;
using DynamicDraw = BufferUsageValue<BufferUsage::DynamicDraw>;
using StreamDraw = BufferUsageValue<BufferUsage::StreamDraw>;

using Texture2D = TextureTypeValue<TextureType::Texture2D>;

using Triangles = PrimitiveTypeValue<PrimitiveType::Triangles>;
using TriangleStrip = PrimitiveTypeValue<PrimitiveType::TriangleStrip>;
using TriangleFan = PrimitiveTypeValue<PrimitiveType::TriangleFan>;
using Lines = PrimitiveTypeValue<PrimitiveType::Lines>;
using LineStrip = PrimitiveTypeValue<PrimitiveType::LineStrip>;
using LineLoop = PrimitiveTypeValue<PrimitiveType::LineLoop>;
using Points = PrimitiveTypeValue<PrimitiveType::Points>;

/*
* SHADERS
*/
unsigned int createShaderProgram();
void unbindShaderProgram();
void deleteShaderProgram(unsigned int id);
void activateShaderProgram(unsigned int id);
int getShaderVarLocation(unsigned int id, const char* name);
void setShaderVar(int location, int value);
void setShaderVar(int location, float value);
void setShaderVar(int location, const glm::vec2& value);
void setShaderVar(int location, const glm::vec3& value);
void setShaderVar(int location, const glm::vec4& value);
void setShaderVar(int location, const glm::mat4& value);

/*
* BUFFERS
*/

void generateBuffers(unsigned int count, unsigned int* buffers);
void deleteBuffers(unsigned int count, unsigned int* buffers);
void bindBuffer(unsigned int target, unsigned int buffer);
void unbindBuffer(unsigned int target);
void bufferData(unsigned int target, unsigned int size, const void* data, unsigned int usage);

template<BufferType bufferType>
void bindBuffer(unsigned int buffer) {
	bindBuffer(BufferTypeValue<bufferType>::value, buffer);
}

template<BufferType bufferType>
void unbindBuffer() {
	unbindBuffer(BufferTypeValue<bufferType>::value);
}

template<BufferType bufferType, BufferUsage usage>
void bufferData(unsigned int size, const void* data) {
	bufferData(BufferTypeValue<bufferType>::value, size, data, BufferUsageValue<usage>::value);
}

/*
* DRAWING
*/

void drawArrays(unsigned int mode, int first, int count);
void drawElements(unsigned int mode, int count, unsigned int type, const void* indices);

/*
* TEXTURES
*/
void generateTextures(unsigned int count, unsigned int* textures);
void activeTexture(unsigned int slot);
void deleteTextures(unsigned int count, unsigned int* textures);
void bindTexture(unsigned int target, unsigned int texture);
void unbindTexture(unsigned int target);

template<TextureType textureType>
void bindTexture(unsigned int texture) {
	bindTexture(TextureTypeValue<textureType>::value, texture);
}

template<TextureType textureType>
void unbindTexture() {
	unbindTexture(TextureTypeValue<textureType>::value);
}

}

namespace rp = render::pipeline;

}
