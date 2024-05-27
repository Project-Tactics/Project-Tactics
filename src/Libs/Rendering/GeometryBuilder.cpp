#include "GeometryBuilder.h"

namespace tactics {

GeometryBuilder::GeometryBuilder(std::vector<GeometryBuilder::Attributef> attributes): _attributes(attributes) {}

void GeometryBuilder::addVertex(std::vector<float> vertex) {
	_vertices.insert(_vertices.end(), vertex.begin(), vertex.end());
}

void GeometryBuilder::addIndex(unsigned int index) {
	_indices.push_back(index);
}

void GeometryBuilder::addIndices(std::vector<unsigned int> indices) {
	_indices.insert(_indices.end(), indices.begin(), indices.end());
}

}
