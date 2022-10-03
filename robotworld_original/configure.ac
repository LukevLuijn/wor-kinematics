# If given on the commandline, the following options overrule any
# options given in the Autotools files. They either replace the
# value (cc and cxx) or are added as last and additional/overruling
# argument (all others).
#
#	CC : the C-compiler to use
#	CXX: the C++-compiler to use. Be warned: AX_CXX_COMPILE_STDCXX sets CXX!
#   For the next variables see https://www.gnu.org/software/autoconf/manual/autoconf.html for the full explanation
#   CPPFLAGS: Preprocessor options for the C, C++, Objective C, and Objective C++ preprocessors and compilers. Defaults to empty.	
#	CFLAGS: debugging and optimization options for the C compiler. Defaults per AC_PROG_CC to "-g O2", see below though.  
#	CXXFLAGS: debugging and optimization options for the C++ compiler. Defaults per AC_PROG_CXX to "-g O2", see below though.
#	LDFLAGS: options for the linker. Defaults to empty.
#   LIBS: -l options to pass to the linker. Defaults to empty.

# If you change anything: tabs and spaces matter. Google it....

AC_INIT([robotworld],[0.2.0],[J.Kraaijeveld@askesis.nl],[robotworld-0.2.0])
AC_CONFIG_AUX_DIR(.)
AM_INIT_AUTOMAKE([subdir-objects])
AC_CONFIG_MACRO_DIR(./m4)

# Check for programs

LT_INIT

AC_PROG_MAKE_SET

# Set CFLAGS to an empty string, otherwise it will be set to "-g O2" by AC_PROG_CC
# See https://www.gnu.org/software/autoconf/manual/autoconf.html#C-Compiler
CFLAGS=""
AC_PROG_CC
AC_LANG([C])

# Set CXXFLAGS to an empty string, otherwise it will be set to "-g O2" by AC_PROG_CXX
# See https://www.gnu.org/software/autoconf/manual/autoconf.html#C_002b_002b-Compiler
CXXFLAGS=""
AC_PROG_CXX
AC_LANG([C++])

#########################################################################################################
#                     --with-cxx=11|14|17 (mandatory option, so check it first)
#########################################################################################################
AC_ARG_WITH(cxx,           [  --with-cxx=11|14|17|20     (mandatory) use the given C++ dialect], [WITH_CXX="$withval"])

if test -n "$WITH_CXX"; then
   case "$WITH_CXX" in
        11)
			ROBOTWORLD_STD_CXX="11"
            ;;
        14)
			ROBOTWORLD_STD_CXX="14"
            ;;
        17)
			ROBOTWORLD_STD_CXX="17"
			;;
        20)
			ROBOTWORLD_STD_CXX="2a"
            ;;
        *)
            AC_MSG_ERROR([Invalid --with-cxx="$WITH_CXX" option value, only 11, 14, 17 or 20 are supported])
    esac
	ROBOTWORLD_STD_CXX="-std=gnu++"$WITH_CXX
else
	AC_MSG_ERROR([Option --with-cxx= not given, read help for information])
fi
# The next macro is functional more or less equivalent to the stuff above but
# for didactical purposes we enforce a manual setting of the C++-version
# AX_CXX_COMPILE_STDCXX(2a, ext)

#########################################################################################################
#		CPPCheck
#########################################################################################################
AC_CHECK_PROGS([CPPCHECK], [cppcheck], [yes])
if test -z "$CPPCHECK"; then
  AC_MSG_WARN([cppcheck not found - continuing without cppcheck support])
fi
AM_CONDITIONAL([HAVE_CPPCHECK], [test -n "$CPPCHECK"])

#########################################################################################################
#		run-clang-tidy
#########################################################################################################
AC_CHECK_PROGS([CLANGTIDY], [run-clang-tidy], [yes])
if test -z "$CLANGTIDY"; then
  AC_MSG_WARN([run-clang-tidy not found - continuing without run-clang-tidy support])
fi
AM_CONDITIONAL([HAVE_CLANGTIDY], [test -n "$CLANGTIDY"])

#########################################################################################################
#		bear
#########################################################################################################
AC_CHECK_PROGS([BEAR], [bear], [yes])
if test -z "$BEAR"; then
  AC_MSG_WARN([bear not found - continuing without bear (needed for clang-tidy support)])
fi
AM_CONDITIONAL([HAVE_BEAR], [test -n "$BEAR"])

#########################################################################################################
#		compiledb
#########################################################################################################
AC_CHECK_PROGS([COMPILEDB], [compiledb], [yes])
if test -z "$COMPILEDB"; then
  AC_MSG_WARN([compiledb not found - continuing without compiledb (needed for clang-tidy support)])
fi
AM_CONDITIONAL([HAVE_COMPILEDB], [test -n "$COMPILEDB"])


#########################################################################################################
#		Doxygen
#########################################################################################################
AC_CHECK_PROGS([DOXYGEN], [doxygen], [yes])
if test -z "$DOXYGEN"; then
   AC_MSG_WARN([Doxygen not found - continue without Doxygen support])
fi
AM_CONDITIONAL([HAVE_DOXYGEN], [test -n "$DOXYGEN"])

AC_CANONICAL_HOST

#########################################################################################################
#		Compilation target
#########################################################################################################
case $host in
*linux*)
		TARGET=linux
	;;
*mingw* | *i686-pc-mingw64* | *i686-pc-mingw32* | *i686-pc-msys* | *x86_64-unknown-msys*)
	if [ "$host" -eq "$build" ]
	then
		TARGET=mingw
	else #if [ "$host" != "$build" ]
		TARGET=crossmingw
	fi
	;;
*darwin*)
		TARGET=darwin
	;;
esac

#########################################################################################################
#		Generic headers
#########################################################################################################
# Check for headers

# Check for typedefs, structures and compiler chracteristics
# Empty for now

# Check for library functions
# Empty for now


# Check for libraries

#########################################################################################################
#                     Boost
#########################################################################################################

# boost.m4 sets the following macros: 
#
# BOOST_CPPFLAGS: the include directory (-I)
# BOOST_LDFLAGS: the library path (-L) with -rpath/-R, if --with-boost=/path/to/boost is given 
#
# Each additional ax_boost_LIBRARY.m4 sets the appropriate LIBRARY name

BOOST_REQUIRE([1.60])

#################################### Boost naming transition 1.68/69 hack #################################### 
# From 1.69 Boost seems to encode the memory model into the library's file name
# Whether this is true for upcoming Debian (derived distribution) system installed libraries, 
# i.e. with --layout-system is unknown.

# After BOOST_REQUIRE we have the Boost (installation) directory via BOOST_CPPFLAGS so we can determine the version
# which is contained in <boost/version.hpp>

# If Boost is installed as system library, BOOST_CPPFLAGS is empty
if test -z $BOOST_CPPFLAGS; then 
    VERSION_HPP_PATH=/usr/local/include
else
	VERSION_HPP_PATH=${BOOST_CPPFLAGS:2} # strip the -I    
fi
boost_version=$(cat $VERSION_HPP_PATH/boost/version.hpp | grep "#define BOOST_VERSION ")
boost_version=$(echo ${boost_version: -6})
major_boost_version=$(echo ${boost_version:0:2})
minor_boost_version=$(echo ${boost_version:2:2})
patch_boost_version=$(echo ${boost_version:4:2})
# Boost 1.69 (and higher?) encode the memory model in the library's file name 
if test $minor_boost_version -gt "68"; then 
	MEMORY_MODEL=`uname -m`
	if test ${MEMORY_MODEL} = "x86_64" ; then
		MEMORY_MODEL=64
	else
		MEMORY_MODEL=32
	fi
    requested_version="mt-x$MEMORY_MODEL"
else
	requested_version="mt"   
fi
#################################### Boost naming transition 1.68/69 hack ####################################    

# These libraries are actually all needed for the program
#BOOST_SYSTEM([$requested_version])
#BOOST_THREAD([$requested_version])
#BOOST_FILESYSTEM([$requested_version])
#BOOST_CHRONO([$requested_version])

BOOST_SYSTEM([$requested_version])
BOOST_THREAD([$requested_version])
BOOST_DATE_TIME([$requested_version])
BOOST_REGEX([$requested_version])
BOOST_SERIALIZATION([$requested_version])
BOOST_FILESYSTEM([$requested_version])
BOOST_CHRONO([$requested_version])
BOOST_TEST([$requested_version])

#################################### boost.m4 missing BOOST_TIMER macro hack ####################################
# If we get here we can assume that Boost is installed somewhere: otherwise the previous macros would have failed.
# Be warned: this piece depends on the "Boost naming transition 1.68/69 hack" because of $minor_boost_version
# and should therefor be after the "Boost naming transition 1.68/69 hack"!  

# According to the Boost src-distribution, the Jamroot file, system installed libaries do not encode anything in 
# the file name

# If Boost is installed as system library, BOOST_CPPFLAGS is empty
# See https://lists.gnu.org/archive/html/autoconf/2003-04/msg00103.html for the AC_DEFINE solution
# and why "AC_DEFINE(BOOST_TIMER_LIBS" does not work...  
AC_DEFINE(BOOST_TIMER_LIBS,"","")
if test -z "$BOOST_CPPFLAGS"; then 
    BOOST_TIMER_LIBS="-lboost_timer" 
else	
	if test $minor_boost_version -gt "68"; then 
		MEMORY_MODEL=`uname -m`
		if test ${MEMORY_MODEL} = "x86_64" ; then
			MEMORY_MODEL=64
		else
			MEMORY_MODEL=32
		fi
	    BOOST_TIMER_LIBS="-lboost_timer-mt-x$MEMORY_MODEL" 
	else
	    BOOST_TIMER_LIBS="-lboost_timer-mt"       
	fi   
fi
#################################### boost.m4 missing BOOST_TIMER macro hack ####################################

# Replace -I by -isystem to prevent warning generation from the Boost headers (which we can't/won't fix)
# This seems to needed only (?) for a non-system installed Boost?
AC_ARG_WITH(boost,
[  --with-boost=path_to_installed_boost     set the path to an installed Boost *not* in the path],
[BOOST_CPPFLAGS=`echo ${BOOST_CPPFLAGS} | sed 's/-I/-isystem /'`],[BOOST_CPPFLAGS=${BOOST_CPPFLAGS}])
AC_SUBST([BOOST_CPPFLAGS])

# Put a space after every link flag (except the last one)!
BOOST_LDFLAGS=""
BOOST_LDFLAGS+="$BOOST_SYSTEM_LDFLAGS "
BOOST_LDFLAGS+="$BOOST_THREAD_LDFLAGS "
BOOST_LDFLAGS+="$BOOST_FILESYSTEM_LDFLAGS "
BOOST_LDFLAGS+="$BOOST_CHRONO_LDFLAGS"
# using tr, split string on ' ', sort unique (because multiple libraries are mentioned multiple times) and, 
# using tr, resplit (i.e. concatinate) again on '\n'
# the grep is due to a boost.M4 problem that generates spurious stuff
BOOST_LDFLAGS=$(echo $BOOST_LDFLAGS | tr ' ' '\n' | sort -u | grep -oh "\w*-[[[:print:]]]*\w*" | tr '\n' ' ')
AC_SUBST([BOOST_LDFLAGS])

AC_CANONICAL_HOST
case $host in
*linux*)
	BOOST_LIBS="$BOOST_SYSTEM_LIBS $BOOST_THREAD_LIBS $BOOST_FILESYSTEM_LIBS $BOOST_CHRONO_LIBS"
	;;
*mingw*)
	# handle any cross compiling
	case $build in
	*linux*)
		BOOST_LIBS="$BOOST_SYSTEM_LIBS $BOOST_THREAD_LIBS $BOOST_FILESYSTEM_LIBS $BOOST_CHRONO_LIBS"
		;;
	*mingw*)
		BOOST_LIBS="$BOOST_SYSTEM_LIBS $BOOST_THREAD_LIBS $BOOST_FILESYSTEM_LIBS $BOOST_CHRONO_LIBS"
		;;
	esac
	;;
*darwin*)
	BOOST_LIBS="$BOOST_SYSTEM_LIBS $BOOST_THREAD_LIBS $BOOST_FILESYSTEM_LIBS $BOOST_CHRONO_LIBS"
	;;
esac
AC_SUBST([BOOST_LIBS])
# unset LDFLAGS because it is used in detecting the Boost libraries and it may hold old values
LDFLAGS=""

#########################################################################################################
#                     wxWidgets
#########################################################################################################

AM_OPTIONS_WXCONFIG
AM_PATH_WXCONFIG(3.1.2, wxWin=1, , ,[--debug=yes])
if test "$wxWin" != 1; then
	AC_MSG_ERROR([
				wxWidgets must be installed on your system
				but no wx-config script could be found.

				Please check that wx-config is in path, the directory
				where wxWidgets libraries are installed as returned by 
				'wx-config --libs' is in the linker path (LD_LIBRARY_PATH 
				or equivalent variable) and wxWidgets version is 2.9.4 or 
				above.
		])
	fi
if test "$wxWin" != 0; then
	AC_DEFINE(HAVE_WXWIDGETS,1,[define if the wxWidgets 3.0.2. or higher is available])
fi

# Replace -I by -isystem to prevent warning generation from the wxWidgets headers (which we can't/won't fix)
# This seems to needed only (?) for a non-system installed wxWidgets?
AC_ARG_WITH(wx-config,
[  --with-wx-config=path_to_wx-config     use wx-config in path_to_wx-config],
[WX_CPPFLAGS=`echo ${WX_CPPFLAGS} | sed 's/-I/-isystem /g'`],[WX_CPPFLAGS=${WX_CPPFLAGS}])
AC_SUBST([WX_CPPFLAGS])
AC_ARG_WITH(wx-config,
[  --with-wx-config=path_to_wx-config     use wx-config in path_to_wx-config],
[WX_CXXFLAGS=`echo ${WX_CXXFLAGS} | sed 's/-I/-isystem /g'`],[WX_CXXFLAGS=${WX_CXXFLAGS}])
AC_SUBST([WX_CXXFLAGS])

# This is needed in case the lib directory of wxWidgets is not in the linker path
AC_ARG_WITH(wx-config,
[  --with-wx-config=path_to_wx-config     use wx-config in path_to_wx-config],
[WX_LDFLAGS="-Wl,-rpath,`$withval --exec-prefix`/lib"],[WX_LDFLAGS=""])
AC_SUBST([WX_LDFLAGS])

#########################################################################################################
#                     Windows Sockets, Unix works out of the box.....
#########################################################################################################
case $host in
*mingw* | *i686-pc-mingw64* | *i686-pc-mingw32* | *i686-pc-msys* | *x86_64-unknown-msys*)
	SOCKET_LIBS="-lwsock32 -lws2_32"
	;;
esac
AC_SUBST([SOCKET_LIBS])


#########################################################################################################
#                     Set the preprocessor, c-compiler, c++-compiler and linker option
#########################################################################################################

# set the CPP options
case $host in
*linux*)
	ROBOTWORLD_CPPFLAGS=""
	;;
*mingw* | *i686-pc-mingw64* | *i686-pc-mingw32* | *i686-pc-msys* | *x86_64-unknown-msys*)
	ROBOTWORLD_CPPFLAGS="-D_WIN32_WINNT=0x0501"
	;;
*darwin*)
	ROBOTWORLD_CPPFLAGS=""
	;;
esac
AC_SUBST([ROBOTWORLD_CPPFLAGS])

# set the C options
case $host in
*linux*)
	ROBOTWORLD_CFLAGS="-pipe -std=gnu11"
	;;
*mingw* | *i686-pc-mingw64* | *i686-pc-mingw32* | *i686-pc-msys* | *x86_64-unknown-msys*)
	ROBOTWORLD_CFLAGS="-pipe -std=gnu11"
	;;
*darwin*)
	ROBOTWORLD_CFLAGS="-pipe -std=gnu11"
	;;
esac
AC_SUBST([ROBOTWORLD_CFLAGS])

# set the C++ options
case $host in
*linux*)
	ROBOTWORLD_CXXFLAGS="-pipe "$ROBOTWORLD_STD_CXX
	;;
*mingw* | *i686-pc-mingw64* | *i686-pc-mingw32* | *i686-pc-msys* | *x86_64-unknown-msys*)
	ROBOTWORLD_CXXFLAGS="-pipe "$ROBOTWORLD_STD_CXX
	;;
*darwin*)
	ROBOTWORLD_CXXFLAGS="-pipe "$ROBOTWORLD_STD_CXX
	;;
esac
AC_SUBST([ROBOTWORLD_CXXFLAGS])

# set the Linker options
case $host in
*linux*)
	ROBOTWORLD_LDFLAGS=""
	;;
*mingw* | *i686-pc-mingw64* | *i686-pc-mingw32* | *i686-pc-msys* | *x86_64-unknown-msys*)
	ROBOTWORLD_LDFLAGS="-Wl,--subsystem,windows -mwindows"
	;;
*darwin*)
	ROBOTWORLD_LDFLAGS=""
	;;
	
esac
AC_SUBST([ROBOTWORLD_LDFLAGS])

#########################################################################################################
#		General AM stuff
#########################################################################################################
AM_CPPFLAGS=
AM_CFLAGS=
AM_CXXFLAGS=
AM_LDFLAGS=

case $host in
*linux*)
	AM_CPPFLAGS="$AM_CPPFLAGS"
	AM_CFLAGS="$AM_CFLAGS"
	AM_CXXFLAGS="$AM_CXXFLAGS"
	AM_LDFLAGS="-no-undefined"
	;;
*mingw* | *i686-pc-mingw64* | *i686-pc-mingw32* | *i686-pc-msys* | *x86_64-unknown-msys*)
	AM_CPPFLAGS="$AM_CPPFLAGS"
	AM_CFLAGS="$AM_CFLAGS"
	AM_CXXFLAGS="$AM_CXXFLAGS"
	AM_LDFLAGS="-no-undefined -Wl,--enable-runtime-pseudo-reloc,--enable-auto-import"
	;;
*darwin*)
	AM_CPPFLAGS="$AM_CPPFLAGS"
	AM_CFLAGS="$AM_CFLAGS"
	AM_CXXFLAGS="$AM_CXXFLAGS"
	AM_LDFLAGS="-no-undefined"
	;;
esac

#########################################################################################################
#                               Set debug related compiler options
#########################################################################################################

development_default="yes"
AC_ARG_ENABLE(development,
AS_HELP_STRING([--enable-development= no/yes],[if enabled all development features will be on]),
enable_development=$enableval,
enable_development=$development_default)
if test "$enable_development" = "yes"; then
	AM_CPPFLAGS="$AM_CPPFLAGS -DDEVELOPMENT"
	AM_CFLAGS="$AM_CFLAGS -Og -g3 -Wall -Wextra -Wconversion"
	AM_CXXFLAGS="$AM_CXXFLAGS -Og -g3 -Wall -Wextra -Wconversion"
	AC_MSG_RESULT(Enabling development tracing)
else
	AM_CPPFLAGS="$AM_CPPFLAGS -DPRODUCTION"
	AM_CFLAGS="$AM_CFLAGS -O3"
	AM_CXXFLAGS="$AM_CXXFLAGS -O3"
	AC_MSG_RESULT(Enabling production tracing)
fi

AC_SUBST([AM_CPPFLAGS])
AC_SUBST([AM_CFLAGS])
AC_SUBST([AM_CXXFLAGS])
AC_SUBST([AM_LDFLAGS])
#AC_SUBST([AM_LDADD])

AM_LIBTOOLFLAGS="--silent"
AC_SUBST([AM_LIBTOOLFLAGS])

AC_PROG_INSTALL

# Output files
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_FILES(Makefile src/Makefile)
AC_OUTPUT

# Echo some variables for configure.ac debug purpose
AC_MSG_NOTICE([
	CXX                     : ${CXX}
	C Preprocessor flags    : ${CPPFLAGS}
	C Compile flags         : ${CFLAGS}
	C++ Compile flags       : ${CXXFLAGS}
	Link flags              : ${LDFLAGS}
	Link libraries          : ${LIBS}
	"AM_CPPFLAGS"           : ${AM_CPPFLAGS}
	"AM_CFLAGS"             : ${AM_CFLAGS}
	"AM_CXXFLAGS"           : ${AM_CXXFLAGS}
	"AM_LDFLAGS"            : ${AM_LDFLAGS}
#	"AM_LDADD"              : ${AM_LDADD}
	ROBOTWORLD_CPPFLAGS     : ${ROBOTWORLD_CPPFLAGS}
	ROBOTWORLD_CFLAGS       : ${ROBOTWORLD_CFLAGS}
	ROBOTWORLD_CXXFLAGS     : ${ROBOTWORLD_CXXFLAGS}
	ROBOTWORLD_LDFLAGS      : ${ROBOTWORLD_LDFLAGS}
	BOOST_CPPFLAGS          : ${BOOST_CPPFLAGS}
	BOOST_LDFLAGS           : ${BOOST_LDFLAGS}
	BOOST_LIBS              : ${BOOST_LIBS}	
	BOOST_SYSTEM_LIBS       : ${BOOST_SYSTEM_LIBS}        
	BOOST_THREAD_LIBS       : ${BOOST_THREAD_LIBS}
	BOOST_CHRONO_LIBS       : ${BOOST_CHRONO_LIBS}
	BOOST_TIMER_LIBS        : ${BOOST_TIMER_LIBS}
	BOOST_FILESYSTEM_LIBS   : ${BOOST_FILESYSTEM_LIBS}
	WX_CPPFLAGS             : ${WX_CPPFLAGS}
	WX_CFLAGS               : ${WX_CFLAGS}
	WX_CXXFLAGS             : ${WX_CXXFLAGS}
	WX_LDFLAGS              : ${WX_LDFLAGS}
])
