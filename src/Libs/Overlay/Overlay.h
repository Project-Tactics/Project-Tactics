#pragma once

namespace tactics {

class Overlay {
public:
	virtual ~Overlay() = default;
	virtual void update() = 0;
};

}
