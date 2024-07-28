#pragma once

#include "../Exception.h"

#include <Libs/Utility/Json/Json.h>

#include <array>
#include <fmt/color.h>
#include <fmt/core.h>
#include <memory>
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

#ifdef COMPILING_TESTS
#define LOG_TRACE(...)			 (void)0
#define LOG_DEBUG(...)			 (void)0
#define LOG_INFO(...)			 (void)0
#define LOG_WARNING(...)		 (void)0
#define LOG_ERROR(...)			 (void)0
#define LOG_CRITICAL(...)		 (void)0
#define LOG_EXCEPTION(exception) (void)0
#else
#define LOG_TRACE(...)			 tactics::Log::trace(__VA_ARGS__)
#define LOG_DEBUG(...)			 tactics::Log::debug(__VA_ARGS__)
#define LOG_INFO(...)			 tactics::Log::info(__VA_ARGS__)
#define LOG_WARNING(...)		 tactics::Log::warning(__VA_ARGS__)
#define LOG_ERROR(...)			 tactics::Log::error(__VA_ARGS__)
#define LOG_CRITICAL(...)		 tactics::Log::critical(__VA_ARGS__)
#define LOG_EXCEPTION(exception) tactics::Log::exception(exception)
#endif

class LogCategory {
public:
	LogCategory(std::string_view name, const fmt::text_style& style);
	LogCategory(std::string_view name, uint32_t rgb);
	const std::string& getName() const;
	const fmt::text_style& getStyle() const;

	bool operator==(const LogCategory& other) const {
		return _name == other._name;
	}

private:
	std::string _name;
	fmt::text_style _style;
};

class LogInstance {
public:
	virtual ~LogInstance() = default;
	virtual void log(const LogCategory& category, LogLevel level, const std::string& message);
	virtual void init(LogLevel minimumLogLevel);
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
	static const LogCategory Input;
	static const LogCategory Ecs;

	static void init(LogLevel minimumLogLevel = LogLevel::Trace);
	static void setLogInstance(std::unique_ptr<LogInstance> logInstance);

	template<typename... Args>
	static void trace(const LogCategory& category, fmt::format_string<Args...> fmt, Args&&... args) {
		log(category, LogLevel::Trace, fmt, fmt::make_format_args(args...));
	}

	template<typename... Args>
	static void debug(const LogCategory& category, fmt::format_string<Args...> fmt, Args&&... args) {
		log(category, LogLevel::Debug, fmt, fmt::make_format_args(args...));
	}

	template<typename... Args>
	static void info(const LogCategory& category, fmt::format_string<Args...> fmt, Args&&... args) {
		log(category, LogLevel::Info, fmt, fmt::make_format_args(args...));
	}

	template<typename... Args>
	static void warning(const LogCategory& category, fmt::format_string<Args...> fmt, Args&&... args) {
		log(category, LogLevel::Warning, fmt, fmt::make_format_args(args...));
	}

	template<typename... Args>
	static void error(const LogCategory& category, fmt::format_string<Args...> fmt, Args&&... args) {
		log(category, LogLevel::Error, fmt, fmt::make_format_args(args...));
	}

	template<typename... Args>
	static void critical(const LogCategory& category, fmt::format_string<Args...> fmt, Args&&... args) {
		log(category, LogLevel::Critical, fmt, fmt::make_format_args(args...));
	}

	static void exception(const std::exception& exception);
	static void exception(const json_exception& exception);
	static void exception(const Exception& exception);

	static void log(const LogCategory& category, LogLevel level, fmt::string_view fmt, fmt::format_args args);

	static bool hasBeenLoggedOverLevel(LogLevel minimumLogLevel);
	static std::string getLogCountRecapMessage();

private:
	static std::array<int, 6> _logCountsByLevel;
	static bool _isLogEnabled;
	static std::unique_ptr<LogInstance> _logInstance;
};

} // namespace tactics
