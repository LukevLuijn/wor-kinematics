#ifndef TYPEINFO_HPP_
#define TYPEINFO_HPP_

#include "Config.hpp"

#include <cstdlib>
#include <cxxabi.h>
#include <string>
#include <typeinfo>

namespace Utils
{
	/**
	 * @return The demangled (human readable) version of the name() string for std::type_info
	 */
	inline std::string demangleTypeInfo(const std::type_info& aTypeInfo)
	{
		int status;
		char* realname;
		realname = abi::__cxa_demangle(aTypeInfo.name(), 0, 0, &status);
		std::string result(realname);
		std::free(realname);
		return result;
	}
	/**
	 * @return The demangled (human readable) version of aTypeInfoString which should contain the name() string for std::type_info
	 */
	inline std::string demangleTypeInfo(const std::string& aTypeInfoString)
	{
		int status;
		char* realname;
		realname = abi::__cxa_demangle(aTypeInfoString.c_str(), 0, 0, &status);
		std::string result(realname);
		std::free(realname);
		return result;
	}
	/**
	 * @return The demangled (human readable) version of the name() string for type T
	 */
	template<typename T>
	inline std::string typeinfoFor(const T& x)
	{
		return demangleTypeInfo(typeid(x));
	}
	/**
	 * @return The demangled (human readable) version of the name() string for type T
	 */
	template<typename T>
	inline std::string typeinfoFor(T const* & x)
	{
		return demangleTypeInfo(typeid(x));
	}
} // namespace Utils

#endif // DANU_TYPEINFO_HPP_
