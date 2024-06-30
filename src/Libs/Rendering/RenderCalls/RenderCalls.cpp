#include "RenderCalls.h"

#ifndef TACTICS_HEADLESS

#include <glad/gl.h>

namespace tactics::render::pipeline {

const unsigned int VertexBuffer::value = GL_ARRAY_BUFFER;
const unsigned int IndexBuffer::value = GL_ELEMENT_ARRAY_BUFFER;

const unsigned int StaticDraw::value = GL_STATIC_DRAW;
const unsigned int DynamicDraw::value = GL_DYNAMIC_DRAW;
const unsigned int StreamDraw::value = GL_STREAM_DRAW;

const unsigned int Texture2D::value = GL_TEXTURE_2D;

const unsigned int Triangles::value = GL_TRIANGLES;
const unsigned int TriangleStrip::value = GL_TRIANGLE_STRIP;
const unsigned int TriangleFan::value = GL_TRIANGLE_FAN;
const unsigned int Lines::value = GL_LINES;
const unsigned int LineStrip::value = GL_LINE_STRIP;
const unsigned int LineLoop::value = GL_LINE_LOOP;
const unsigned int Points::value = GL_POINTS;

unsigned int createShaderProgram() { return glCreateProgram(); }

void unbindShaderProgram() { glUseProgram(0); }

void deleteShaderProgram(unsigned int id) { glDeleteProgram(id); }

void activateShaderProgram(unsigned int id) { glUseProgram(id); }

int getShaderVarLocation(unsigned int id, const char* name) { return glGetUniformLocation(id, name); }

void setShaderVar(int location, int value) { glUniform1i(location, value); }

void setShaderVar(int location, float value) { glUniform1f(location, value); }

void setShaderVar(int location, const glm::vec2& value) { glUniform2f(location, value.x, value.y); }

void setShaderVar(int location, const glm::vec3& value) { glUniform3f(location, value.x, value.y, value.z); }

void setShaderVar(int location, const glm::vec4& value) { glUniform4f(location, value.x, value.y, value.z, value.w); }

void setShaderVar(int location, const glm::mat4& value) { glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]); }

void generateBuffers(unsigned int count, unsigned int* buffers) { glGenBuffers(count, buffers); }

void deleteBuffers(unsigned int count, unsigned int* buffers) { glDeleteBuffers(count, buffers); }

void bindBuffer(unsigned int target, unsigned int buffer) { glBindBuffer(target, buffer); }

void unbindBuffer(unsigned int target) { glBindBuffer(target, 0); }

void bufferData(unsigned int target, unsigned int size, const void* data, unsigned int usage) {
	glBufferData(target, size, data, usage);
}

void generateTextures(unsigned int count, unsigned int* textures) { glGenTextures(count, textures); }

void activeTexture(unsigned int slot) { glActiveTexture(GL_TEXTURE0 + slot); }

void deleteTextures(unsigned int count, unsigned int* textures) { glDeleteTextures(count, textures); }

void bindTexture(unsigned int target, unsigned int texture) { glBindTexture(target, texture); }

void unbindTexture(unsigned int target) { glBindTexture(target, 0); }

} // namespace tactics::render::pipeline

#else

namespace tactics::render::pipeline {

const unsigned int VertexBuffer::value = 0;
const unsigned int IndexBuffer::value = 0;
const unsigned int StaticDraw::value = 0;
const unsigned int DynamicDraw::value = 0;
const unsigned int StreamDraw::value = 0;
const unsigned int Texture2D::value = 0;
const unsigned int Triangles::value = 0;
const unsigned int TriangleStrip::value = 0;
const unsigned int TriangleFan::value = 0;
const unsigned int Lines::value = 0;
const unsigned int LineStrip::value = 0;
const unsigned int LineLoop::value = 0;
const unsigned int Points::value = 0;

unsigned int createShaderProgram() { return 0; }

void unbindShaderProgram() {}

void deleteShaderProgram(unsigned int /*id*/) {}

void activateShaderProgram(unsigned int /*id*/) {}

int getShaderVarLocation(unsigned int /*id*/, const char* /*name*/) { return 0; }

void setShaderVar(int /*location*/, int /*value*/) {}

void setShaderVar(int /*location*/, float /*value*/) {}

void setShaderVar(int /*location*/, const glm::vec2& /*value*/) {}

void setShaderVar(int /*location*/, const glm::vec3& /*value*/) {}

void setShaderVar(int /*location*/, const glm::vec4& /*value*/) {}

void setShaderVar(int /*location*/, const glm::mat4& /*value*/) {}

void generateBuffers(unsigned int /*count*/, unsigned int* /*buffers*/) {}

void deleteBuffers(unsigned int /*count*/, unsigned int* /*buffers*/) {}

void bindBuffer(unsigned int /*target*/, unsigned int /*buffer*/) {}

void unbindBuffer(unsigned int /*target*/) {}

void bufferData(unsigned int /*target*/, unsigned int /*size*/, const void* /*data*/, unsigned int /*usage*/) {}

void generateTextures(unsigned int /*count*/, unsigned int* /*textures*/) {}

void activeTexture(unsigned int /*slot*/) {}

void deleteTextures(unsigned int /*count*/, unsigned int* /*textures*/) {}

void bindTexture(unsigned int /*target*/, unsigned int /*texture*/) {}

void unbindTexture(unsigned int /*target*/) {}

} // namespace tactics::render::pipeline

#endif

