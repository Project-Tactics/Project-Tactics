#pragma once

#include "../Exception.h"

#include <fmt/core.h>
#include <fmt/color.h>
#include <string_view>

namespace tactics {

enum LogLevel {
	Trace,
	Debug,
	Info,
	Warning,
	Error,
	Critical
};

#define LOG_TRACE(...) Log::trace(__VA_ARGS__)
#define LOG_DEBUG(...) Log::debug(__VA_ARGS__)
#define LOG_INFO(...) Log::info(__VA_ARGS__)
#define LOG_WARNING(...) Log::warning(__VA_ARGS__)
#define LOG_ERROR(...) Log::error(__VA_ARGS__)
#define LOG_CRITICAL(...) Log::critical(__VA_ARGS__)
#define LOG_EXCEPTION(exception) Log::exception(exception)

class LogCategory {
public:
	LogCategory(std::string_view name, const fmt::text_style& style);
	LogCategory(std::string_view name, uint32_t rgb);
	const std::string& getName() const;
	const fmt::text_style& getStyle() const;

private:
	std::string _name;
	fmt::text_style _style;
};

class Log {
public:
	static const LogCategory Game;
	static const LogCategory General;
	static const LogCategory Engine;
	static const LogCategory Resource;
	static const LogCategory Rendering;
	static const LogCategory Overlay;
	static const LogCategory Fsm;
	static const LogCategory Ecs;

	static void init(LogLevel minimumLogLevel);

	template<typename... Args>
	static void trace(const LogCategory& category, fmt::format_string<Args...> fmt, Args &&... args) {
		log(category, LogLevel::Trace, fmt, fmt::make_format_args(args...));
	}

	template<typename... Args>
	static void debug(const LogCategory& category, fmt::format_string<Args...> fmt, Args &&... args) {
		log(category, LogLevel::Debug, fmt, fmt::make_format_args(args...));
	}

	template<typename... Args>
	static void info(const LogCategory& category, fmt::format_string<Args...> fmt, Args &&... args) {
		log(category, LogLevel::Info, fmt, fmt::make_format_args(args...));
	}

	template<typename... Args>
	static void warning(const LogCategory& category, fmt::format_string<Args...> fmt, Args &&... args) {
		log(category, LogLevel::Warning, fmt, fmt::make_format_args(args...));
	}

	template<typename... Args>
	static void error(const LogCategory& category, fmt::format_string<Args...> fmt, Args &&... args) {
		log(category, LogLevel::Error, fmt, fmt::make_format_args(args...));
	}

	template<typename... Args>
	static void critical(const LogCategory& category, fmt::format_string<Args...> fmt, Args &&... args) {
		log(category, LogLevel::Critical, fmt, fmt::make_format_args(args...));
	}

	static void exception(const std::exception& exception);
	static void exception(const Exception& exception);

	static void log(const LogCategory& category, LogLevel level, fmt::string_view fmt, fmt::format_args args);
};

}
