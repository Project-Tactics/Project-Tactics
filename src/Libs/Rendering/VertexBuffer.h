#pragma once

#include "BaseBuffer.h"

namespace tactics {

class VertexBuffer: public BaseBuffer<float, GL_ARRAY_BUFFER> {
public:
	using BaseBuffer<float, GL_ARRAY_BUFFER>::BaseBuffer;
};
}