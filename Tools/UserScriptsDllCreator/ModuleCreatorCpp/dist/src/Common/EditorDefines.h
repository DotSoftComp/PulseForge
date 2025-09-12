/**
 * @file EditorDefines.h
 * @brief Defines macros and helper functions used specifically in editor or game builds.
 *
 * This header provides a mechanism for conditionally including/excluding code
 * based on whether the engine is built in **editor** or **runtime** (game) mode.
 *
 * Use the following macros:
 * - `EDITOR_ONLY(code)`: Includes `code` only in editor builds.
 * - `IN_GAME_ONLY(code)`: Includes `code` only in game builds.
 * - `EDITOR_LOG(msg)`: Logs editor messages with timestamps to `std::cout`.
 * - `EDITOR_ERROR(msg)`: Logs error messages with timestamps to `std::cerr`.
 *
 * @note Define `ENGINE_EDITOR` at compile time to enable editor features.
 */

#ifndef EDITORDEFINES_H
#define EDITORDEFINES_H

#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>

// ----------------------------------------------------------------------------
// Editor Mode Definitions
// ----------------------------------------------------------------------------
#ifdef ENGINE_EDITOR

    /**
     * @brief Returns the current date and time as a formatted string.
     *
     * Used for timestamping editor logs.
     *
     * @return A string in the format "YYYY-MM-DD HH:MM:SS".
     */
    inline std::string GetCurrentDateTime()
    {
        auto now = std::chrono::system_clock::now();
        std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
        std::tm* nowTm = std::localtime(&nowTime);

        std::ostringstream oss;
        oss << std::put_time(nowTm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    /// Includes `code` only when compiling in editor mode.
    #define EDITOR_ONLY(code) code

    /// Strips `code` from editor builds; used to isolate game/runtime-only code.
    #define IN_GAME_ONLY(code)

    /// Logs a message to standard output with a timestamp and editor tag.
    #define EDITOR_LOG(msg) \
        std::cout << "[" << GetCurrentDateTime() << "] [Editor - LOG]: " << msg << std::endl;

    /// Logs an error message to standard error with a timestamp and editor tag.
    #define EDITOR_ERROR(msg) \
        std::cerr << "[" << GetCurrentDateTime() << "] [Editor - ERROR]: " << msg << std::endl;

#else

    // ----------------------------------------------------------------------------
    // Game Mode (Runtime Build) - Editor features are excluded
    // ----------------------------------------------------------------------------

    /// Strips `code` from game builds; used to isolate editor-only logic.
    #define EDITOR_ONLY(code)

    /// Includes `code` only when compiling in game mode.
    #define IN_GAME_ONLY(code) code

    /// Editor logs are disabled in game builds.
    #define EDITOR_LOG(msg)

    /// Editor error logs are disabled in game builds.
    #define EDITOR_ERROR(msg)

#endif // ENGINE_EDITOR

#endif // EDITORDEFINES_H
