#include "Log.h"

#include <filesystem>
#include <memory>
#include <source_location>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace tactics {

std::array<int, 6> Log::_logCountsByLevel = {};
bool Log::_isLogEnabled = false;
std::unique_ptr<LogInstance> Log::_logInstance = std::make_unique<LogInstance>();

static fmt::text_style categoryTextStyle(uint32_t rgb) {
	return fmt::bg(fmt::rgb(rgb)) | fmt::emphasis::bold | fmt::emphasis::italic;
}

const LogCategory Log::General("General", 0x000000);
const LogCategory Log::Game("Game", 0x000000);
const LogCategory Log::Engine("Engine", 0x004000);
const LogCategory Log::Resource("Resource", 0x000040);
const LogCategory Log::Rendering("Rendering", 0x400000);
const LogCategory Log::Overlay("Overlay", 0x404000);
const LogCategory Log::Fsm("Fsm", 0x400040);
const LogCategory Log::Ecs("Ecs", 0x000000);

static std::vector<std::shared_ptr<spdlog::logger>>& getLoggers() {
	static std::vector<std::shared_ptr<spdlog::logger>> loggers;
	return loggers;
}

static std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> getSink() {
	static auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	return sink;
}

LogCategory::LogCategory(std::string_view name, const fmt::text_style& style) : _name(name), _style(style) {
	auto& loggers = getLoggers();
	auto& newLogger = loggers.emplace_back(std::make_shared<spdlog::logger>(_name, getSink()));
	newLogger->set_level(spdlog::level::trace);
	spdlog::register_logger(newLogger);
}

LogCategory::LogCategory(std::string_view name, uint32_t rgb) : LogCategory(name, categoryTextStyle(rgb)) {}

const fmt::text_style& LogCategory::getStyle() const { return _style; }

const std::string& LogCategory::getName() const { return _name; }

[[nodiscard]] static const char* toString(LogLevel level) {
	switch (level) {
	case LogLevel::Trace   : return "Trace";
	case LogLevel::Debug   : return "Debug";
	case LogLevel::Info	   : return "Info";
	case LogLevel::Warning : return "Warning";
	case LogLevel::Error   : return "Error";
	case LogLevel::Critical: return "Critical";
	}
	return "Unknown";
}

static spdlog::level::level_enum convertLogLevelToSpdLog(LogLevel level) {
	switch (level) {
	case LogLevel::Trace   : return spdlog::level::trace;
	case LogLevel::Debug   : return spdlog::level::debug;
	case LogLevel::Info	   : return spdlog::level::info;
	case LogLevel::Warning : return spdlog::level::warn;
	case LogLevel::Error   : return spdlog::level::err;
	case LogLevel::Critical: return spdlog::level::critical;
	}
	return spdlog::level::off;
}

void Log::setLogInstance(std::unique_ptr<LogInstance> logInstance) { _logInstance = std::move(logInstance); }

void LogInstance::log(const LogCategory& category, LogLevel level, const std::string& message) {
	auto logger = spdlog::get(category.getName());
	logger->log(convertLogLevelToSpdLog(level),
				fmt::format("[{}] {}", fmt::styled(category.getName(), category.getStyle()), message));
}

void LogInstance::init(LogLevel minimumLogLevel) {
	spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
	spdlog::set_level(convertLogLevelToSpdLog(minimumLogLevel));
}

void Log::init(LogLevel minimumLogLevel) {
	_isLogEnabled = true;
	_logInstance->init(minimumLogLevel);
}

void Log::log(const LogCategory& category, LogLevel level, fmt::string_view fmt, fmt::format_args args) {
	if (!_isLogEnabled) { return; }
	_logInstance->log(category, level, fmt::vformat(fmt, args));
	++_logCountsByLevel[static_cast<int>(level)];
}

void Log::exception(const std::exception& exception) { critical(Log::Engine, "Exception: {}", exception.what()); }

void Log::exception(const Exception& exception) {
	std::string message = fmt::format("{}\nCallstack:", exception.message());

	std::filesystem::path rootPath = (*std::stacktrace::current().begin()).source_file();
	for (auto i = 0; i < 4; ++i, rootPath = rootPath.parent_path()) {}

	for (auto&& entry : exception.trace()) {
		auto entryPath = std::filesystem::path(entry.filename);
		auto relativePath = entryPath.lexically_relative(rootPath);
		message += fmt::format("\n{}", fmt::styled(".\\" + relativePath.string(), fmt::fg(fmt::color::aquamarine)));
		message += fmt::format(":{}", entry.line.value_or(0));
		message += fmt::format(" {}\n", fmt::styled(entry.symbol, fmt::fg(fmt::color::bisque)));
		message += cpptrace::get_snippet(entry.filename, entry.line.value_or(0), 1, false);
	}

	critical(Log::Engine, "{}", message);
}

bool Log::hasBeenLoggedOverLevel(LogLevel minimumLogLevel) {
	auto totalCount = 0;
	for (auto logLevelIndex = 0; auto count : _logCountsByLevel) {
		if (logLevelIndex >= static_cast<int>(minimumLogLevel)) { totalCount += count; }
		++logLevelIndex;
	}
	return totalCount > 0;
}

std::string Log::getLogCountRecapMessage() {
	std::string message;
	for (auto logLevelIndex = 0; auto count : _logCountsByLevel) {
		message += fmt::format("{}: {}\n", toString(static_cast<LogLevel>(logLevelIndex)), count);
		++logLevelIndex;
	}
	return message;
}

} // namespace tactics
