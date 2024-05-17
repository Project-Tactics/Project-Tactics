#pragma once

#include <glad/gl.h>

namespace tactics {

class DebugMessageHandler {
public:
	DebugMessageHandler();

	void checkGlErrors(const char* context) const;
	void clearGlErrors() const;

private:
	// Callback function for OpenGL debug messages
	static void _onGlDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

	// Instance method to handle the debug message
	void _handleMessage(GLenum source, GLenum type, GLuint id, GLenum severity, const GLchar* message) const;

	// Convert GLenum to string for better readability
	const char* _getStringForSource(GLenum source) const;
	const char* _getStringForType(GLenum type) const;
	const char* _getStringForSeverity(GLenum severity) const;
};

}
