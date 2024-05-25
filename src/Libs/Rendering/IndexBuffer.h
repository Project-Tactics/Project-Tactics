#pragma once

#include "BaseBuffer.h"

namespace tactics {

class IndexBuffer: public BaseBuffer<unsigned int, GL_ELEMENT_ARRAY_BUFFER> {
public:
	using BaseBuffer<unsigned int, GL_ELEMENT_ARRAY_BUFFER>::BaseBuffer;
};

}
