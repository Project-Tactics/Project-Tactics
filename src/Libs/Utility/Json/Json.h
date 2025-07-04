#pragma once

#ifdef _DEBUG
#define JSON_DIAGNOSTICS 1
#else
#define JSON_DIAGNOSTICS 0
#endif

#include <nlohmann/json.hpp>

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;
using json_exception = nlohmann::detail::exception;
