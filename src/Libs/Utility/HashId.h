#pragma once

#include <cstdint>
#include <fmt/format.h>
#include <string>
#include <string_view>

namespace tactics {

struct HashId {
public:
	HashId() = default;
	explicit HashId(const char* str);
	explicit HashId(const std::string& str);
	explicit HashId(std::string_view str);

	[[nodiscard]] std::uint32_t id() const;
	[[nodiscard]] const char* str() const;
	[[nodiscard]] bool operator==(const HashId& other) const;
	[[nodiscard]] bool operator>(const HashId& other) const;
	[[nodiscard]] operator std::uint32_t() const;
	[[nodiscard]] bool isEmpty() const;

	static const HashId none;

private:
	std::uint32_t _id{};

	// We keep a std::string to improve the debugging experience
#ifdef _DEBUG
	std::string _debugName;
#endif
};

[[nodiscard]] HashId operator"" _id(const char* value, size_t);
} // namespace tactics

template<> struct fmt::formatter<tactics::HashId> {
public:
	constexpr auto parse(format_parse_context& ctx) {
		return ctx.begin();
	}

	template<typename Context> constexpr auto format(tactics::HashId const& obj, Context& ctx) const {
		return format_to(ctx.out(), "{}", obj.str());
	}
};

template<> struct std::hash<tactics::HashId> {
	std::size_t operator()(const tactics::HashId& obj) const noexcept {
		return obj.id();
	}
};
