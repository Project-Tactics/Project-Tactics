#pragma once

#include <vector>
#include <functional>

namespace tactics {

class VertexDefinition {
public:
	class Builder {
	public:
		void attributef(int count);
		VertexDefinition* create();
	private:
		int _stride{};
		unsigned int _attributeIndex{};
		std::vector<std::function<void()>> _attributes;
	};

	VertexDefinition();
	~VertexDefinition();
	void bind();
	void unbind();

private:
	unsigned int vao;
};

}
