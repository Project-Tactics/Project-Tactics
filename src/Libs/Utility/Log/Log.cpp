#include "Log.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <source_location>
#include <filesystem>
#include <memory>

namespace tactics {

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

LogCategory::LogCategory(std::string_view name, const fmt::text_style& style): _name(name), _style(style) {
	auto& loggers = getLoggers();
	auto& newLogger = loggers.emplace_back(
		std::make_shared<spdlog::logger>(
		_name,
		getSink()
	));
	newLogger->set_level(spdlog::level::trace);
	spdlog::register_logger(newLogger);
}

LogCategory::LogCategory(std::string_view name, uint32_t rgb):
	LogCategory(name, categoryTextStyle(rgb)) {
}

const fmt::text_style& LogCategory::getStyle() const {
	return _style;
}

const std::string& LogCategory::getName() const {
	return _name;
}

static spdlog::level::level_enum convertLogLevelToSpdLog(LogLevel level) {
	switch (level) {
	case LogLevel::Trace:
		return spdlog::level::trace;
	case LogLevel::Debug:
		return spdlog::level::debug;
	case LogLevel::Info:
		return spdlog::level::info;
	case LogLevel::Warning:
		return spdlog::level::warn;
	case LogLevel::Error:
		return spdlog::level::err;
	case LogLevel::Critical:
		return spdlog::level::critical;
	}
	return spdlog::level::off;
}

void Log::init(LogLevel minimumLogLevel) {
	spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
	spdlog::set_level(convertLogLevelToSpdLog(minimumLogLevel));
}

void Log::log(const LogCategory& category, LogLevel level, fmt::string_view fmt, fmt::format_args args) {
	auto message = fmt::vformat(fmt, args);
	message = fmt::format("[{}] {}", fmt::styled(category.getName(), category.getStyle()), message);
	spdlog::get(category.getName())->log(
		convertLogLevelToSpdLog(level),
		message);
}

void Log::exception(const std::exception& exception) {
	critical(Log::Engine, "Exception: {}", exception.what());
}

void Log::exception(const Exception& exception) {
	std::string message = fmt::format("{}\nCallstack:", exception.what());

	auto currentStackTrace = std::stacktrace::current();

	// TODO(Gerark) Very hacky way to retrieve the src root folder,
	// can be improved by defining a constant in CMake
	std::filesystem::path currentSourceFilePath((*currentStackTrace.begin()).source_file());
	auto rootPath = currentSourceFilePath.parent_path().parent_path().parent_path().parent_path();

	for (auto&& entry : exception.stackTrace()) {
		auto entryPath = std::filesystem::path(entry.source_file());
		auto relativePath = entryPath.lexically_relative(rootPath);
		message += fmt::format("\n.\\{}:{} {}", relativePath.string(), entry.source_line(), entry.description());
	}

	log(Log::Engine, LogLevel::Critical, message, {});
}

}
