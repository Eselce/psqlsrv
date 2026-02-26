
#pragma once

// General C++ type for type parameter in DBparameter::bindany()...
typedef unsigned int DBparameterType;

// Possible values for format parameter in DBparameter::bindany()...
typedef enum
{
	FORMAT_TEXT	= 0,	// 0: FORMAT_TEXT   - Text format
	FORMAT_BINARY		// 1: FORMAT_BINARY - Binary format
} DBparameterFormat;

class DBconnection;  // forward declaration to avoid circular dependency

extern const char *cmdErrorMsg;  // = "Command failed";

extern const char *cmdLogMsg;  // = "Execute command (status)";

