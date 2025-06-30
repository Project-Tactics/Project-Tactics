#pragma once

#include "BaseBuffer.h"

namespace tactics {

class VertexBuffer : public BaseBuffer<float, render::pipeline::VertexBuffer::type> {
public:
	using BaseBuffer<float, render::pipeline::VertexBuffer::type>::BaseBuffer;
};

template<typename TVertex> class TVertexBuffer : public BaseBuffer<TVertex, render::pipeline::VertexBuffer::type> {
public:
	using BaseBuffer<TVertex, render::pipeline::VertexBuffer::type>::BaseBuffer;
};

} // namespace tactics
