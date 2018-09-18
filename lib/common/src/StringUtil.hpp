/*
 * StringUtil.hpp
 *
 *  Created on: Sep 17, 2018
 *      Author: suoalex
 */

#ifndef LIB_COMMON_SRC_STRINGUTIL_HPP_
#define LIB_COMMON_SRC_STRINGUTIL_HPP_

#include <algorithm>
#include <cctype>
#include <cstring>
#include <locale>
#include <string>

namespace mm
{
	//
	// The string utility functions.
	//
	struct StringUtil
	{
		//
		// trim the input string from start.
		//
		// s : The string to trim.
		//
		static inline void ltrim(std::string &s)
		{
		    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		        return !std::isspace(ch);
		    }));
		}

		//
		// trim the input string from end.
		//
		// s : The string to trim.
		//
		static inline void rtrim(std::string &s)
		{
		    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		        return !std::isspace(ch);
		    }).base(), s.end());
		}

		//
		// trim the input string from both ends.
		//
		// s : The string to trim.
		//
		static inline void trim(std::string &s)
		{
		    ltrim(s);
		    rtrim(s);
		}

		//
		// Copy at most count characters from src to dest. Append '\0' to end of dest regardlessly.
		//
		// dest : The destination.
		// src : The source string.
		// count : max count of chars to copy.
		//
		static inline void copy(char* dest, const std::string& src, std::size_t count)
		{
			std::strncpy(dest, src.c_str(), count);
			dest[count - 1] = '\0';
		}
	};
}



#endif /* LIB_COMMON_SRC_STRINGUTIL_HPP_ */
