#ifdef TACTICS_HEADLESS

#include "RenderCalls.h"

namespace tactics::render::pipeline {

const unsigned int VertexBuffer::value = 0;
const unsigned int IndexBuffer::value = 0;
const unsigned int StaticDraw::value = 0;
const unsigned int DynamicDraw::value = 0;
const unsigned int StreamDraw::value = 0;
const unsigned int Texture2D::value = 0;

unsigned int createShaderProgram() {
	return 0;
}

void deleteShaderProgram(unsigned int /*id*/) {
}

void activateShaderProgram(unsigned int /*id*/) {
}

int getShaderVarLocation(unsigned int /*id*/, const char* /*name*/) {
	return 0;
}

void setShaderVar(int /*location*/, int /*value*/) {
}

void setShaderVar(int /*location*/, float /*value*/) {
}

void setShaderVar(int /*location*/, const glm::vec2& /*value*/) {
}

void setShaderVar(int /*location*/, const glm::vec3& /*value*/) {
}

void setShaderVar(int /*location*/, const glm::vec4& /*value*/) {
}

void setShaderVar(int /*location*/, const glm::mat4& /*value*/) {
}

void generateBuffers(unsigned int /*count*/, unsigned int* /*buffers*/) {
}

void deleteBuffers(unsigned int /*count*/, unsigned int* /*buffers*/) {
}

void bindBuffer(unsigned int /*target*/, unsigned int /*buffer*/) {
}

void unbindBuffer(unsigned int /*target*/) {
}

void bufferData(unsigned int /*target*/, unsigned int /*size*/, const void* /*data*/, unsigned int /*usage*/) {
}

void generateTextures(unsigned int /*count*/, unsigned int* /*textures*/) {
}

void activeTexture(unsigned int /*slot*/) {
}

void deleteTextures(unsigned int /*count*/, unsigned int* /*textures*/) {
}

void bindTexture(unsigned int /*target*/, unsigned int /*texture*/) {
}

void unbindTexture(unsigned int /*target*/) {
}

}

#endif
