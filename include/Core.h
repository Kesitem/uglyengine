#pragma once

//STL
#include <string>
#include <map>
#include <set>
#include <optional>
#include <fstream>

// Include configuration header
#include "config.h"

// Include plog
#include <plog/Log.h>
#include <plog/Appenders/ConsoleAppender.h>

// Include GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// Include glm
#include <glm/glm.hpp>
#include <glm/ext.hpp>

// Constants declaration
namespace ugly
{
	static const std::string ENGINE_NAME = "UglyEngine";
	static const std::string LOG_FILENAME = "UglyEngine.log";

}//namespace ugly