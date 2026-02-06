#ifndef ID3_EXPORT_H
#define ID3_EXPORT_H

/**
 * @file ID3Export.h
 *
 * @author Evandro L. Vieira
 *
 * @brief Platform-specific export/import macros for building shared libraries.
 */

#if defined(_WIN32) || defined(_WIN64)
    #ifdef ID3_BUILD_SHARED
        #define ID3_API __declspec(dllexport)
    #elif defined(ID3_USE_SHARED)
        #define ID3_API __declspec(dllimport)
    #else
        #define ID3_API
    #endif
#elif defined(__GNUC__) && __GNUC__ >= 4
    #ifdef ID3_BUILD_SHARED
        #define ID3_API __attribute__((visibility("default")))
    #else
        #define ID3_API
    #endif
#else
    #define ID3_API
#endif

#endif
