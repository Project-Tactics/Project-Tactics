#pragma once

#include "BaseBuffer.h"

namespace tactics {

class IndexBuffer : public BaseBuffer<unsigned int, render::pipeline::IndexBuffer::type> {
public:
	using BaseBuffer<unsigned int, render::pipeline::IndexBuffer::type>::BaseBuffer;
};

} // namespace tactics
