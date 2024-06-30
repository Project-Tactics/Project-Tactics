#pragma once

namespace tactics {

/**
 * @brief A utility class to generate unique IDs for classes.
 * The ID is generated at compile time.
 *
 * Example:
 *	ClassID<MyClass>::ID()
 */
class ClassIdInternalValue {
public:
	static inline unsigned int nextID = 0;
};

template<typename T> class ClassId {
public:
	constexpr static unsigned int ID() {
		static unsigned int id = ++ClassIdInternalValue::nextID;
		return id;
	}
};
} // namespace tactics
