#include "DebugMessageHandler.h"

#include <Libs/Utility/Exception.h>

#include <string>

namespace tactics {

DebugMessageHandler::DebugMessageHandler() {
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(DebugMessageHandler::_onGlDebugMessage, this);
}

void DebugMessageHandler::clearGlErrors() const {
	while (glGetError() != GL_NO_ERROR) {}
}

void DebugMessageHandler::checkGlErrors(const char* context) const {
	auto errorCode = glGetError();
	if (errorCode != GL_NO_ERROR) {
		std::string error;

		switch (errorCode) {
		case GL_INVALID_ENUM:
			error = "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
			break;
		case GL_INVALID_VALUE: error = "GL_INVALID_VALUE: A numeric argument is out of range."; break;
		case GL_INVALID_OPERATION:
			error =
				"GL_INVALID_OPERATION: The specified operation is not allowed in the current "
				"state.";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			error = "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
			break;
		case GL_OUT_OF_MEMORY:
			error = "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
			break;
		case GL_STACK_UNDERFLOW:
			error =
				"GL_STACK_UNDERFLOW: An attempt has been made to perform an operation that would "
				"cause an internal stack to underflow.";
			break;
		case GL_STACK_OVERFLOW:
			error =
				"GL_STACK_OVERFLOW: An attempt has been made to perform an operation that would "
				"cause an internal stack to overflow.";
			break;
		default: error = "UNKNOWN ERROR: An unknown OpenGL error occurred."; break;
		}

		throw TACTICS_EXCEPTION("OpenGL error in {}: {} - Code: {}", context, error, errorCode);
	}
}

void GLAD_API_PTR DebugMessageHandler::_onGlDebugMessage(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei /*length*/,
	const GLchar* message,
	const void* userParam) {
	const DebugMessageHandler* handler = static_cast<const DebugMessageHandler*>(userParam);
	handler->_handleMessage(source, type, id, severity, message);
}

void DebugMessageHandler::_handleMessage(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	const GLchar* message) const {
	// TODO(Gerark) Better to rely on a Log System here
	if (severity != GL_DEBUG_SEVERITY_NOTIFICATION) {
		printf("GL Debug Message:\n");
		printf("Source: %s\n", _getStringForSource(source));
		printf("Type: %s\n", _getStringForType(type));
		printf("ID: %u\n", id);
		printf("Severity: %s\n", _getStringForSeverity(severity));
		printf("Message: %s\n", message);
	}
}

const char* DebugMessageHandler::_getStringForSource(GLenum source) const {
	switch (source) {
	case GL_DEBUG_SOURCE_API			: return "API";
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM	: return "Window System";
	case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Shader Compiler";
	case GL_DEBUG_SOURCE_THIRD_PARTY	: return "Third Party";
	case GL_DEBUG_SOURCE_APPLICATION	: return "Application";
	case GL_DEBUG_SOURCE_OTHER			: return "Other";
	default								: return "Unknown";
	}
}

const char* DebugMessageHandler::_getStringForType(GLenum type) const {
	switch (type) {
	case GL_DEBUG_TYPE_ERROR			  : return "Error";
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated Behavior";
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR : return "Undefined Behavior";
	case GL_DEBUG_TYPE_PORTABILITY		  : return "Portability";
	case GL_DEBUG_TYPE_PERFORMANCE		  : return "Performance";
	case GL_DEBUG_TYPE_MARKER			  : return "Marker";
	case GL_DEBUG_TYPE_PUSH_GROUP		  : return "Push Group";
	case GL_DEBUG_TYPE_POP_GROUP		  : return "Pop Group";
	case GL_DEBUG_TYPE_OTHER			  : return "Other";
	default								  : return "Unknown";
	}
}

const char* DebugMessageHandler::_getStringForSeverity(GLenum severity) const {
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH		   : return "High";
	case GL_DEBUG_SEVERITY_MEDIUM	   : return "Medium";
	case GL_DEBUG_SEVERITY_LOW		   : return "Low";
	case GL_DEBUG_SEVERITY_NOTIFICATION: return "Notification";
	default							   : return "Unknown";
	}
}

} // namespace tactics
