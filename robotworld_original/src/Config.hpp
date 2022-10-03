#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#ifdef __MINGW32__ // This is also defined for mingw64...

// Disable some warnings in Boost
#pragma GCC diagnostic ignored "-Wattributes"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

// Needed for double Winsock inclusion problems with Boost/ASIO
// See http://msdn.microsoft.com/en-us/library/ms737629%28VS.85%29.aspx
#define WIN32_LEAN_AND_MEAN

# include <_mingw.h>

#endif


// Additional macros that suppress un-used argument compiler warnings
#define UNUSEDPARAM(parameter) /* parameter */
#define UNUSEDCAST(argument) (void)argument

// The defaults for wxWidgets, Boost and the C/C++-runtime differ w.r.t Unicode,
// so we need to define Unicode manually.
#if !defined (UNICODE)
#define UNICODE
#endif
#if !defined (_UNICODE)
#define _UNICODE
#endif

#endif // CONFIG_HPP_
