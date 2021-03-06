/* vim:ts=4
 *
 * Copyleft 2012…2015  Michał Gawron
 * Marduk Unix Labs, http://mulabs.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Visit http://www.gnu.org/licenses/gpl-3.0.html for more information on licensing.
 */

#ifndef XEFIS__CONFIG__TYPES_H__INCLUDED
#define XEFIS__CONFIG__TYPES_H__INCLUDED

// Standard:
#include <stdint.h>
#include <string>
#include <vector>

// Boost:
#include <boost/optional.hpp>

// Qt:
#include <QtCore/QString>

// Lib:
#include <lib/si/all.h>
#include <lib/half/half.hpp>


using namespace SI;


inline std::string
operator"" _str (const char* string, size_t)
{
    return std::string (string);
}


inline QString
operator"" _qstr (const char* string, size_t)
{
    return QString (string);
}


typedef half_float::half		float16_t;
typedef float					float32_t;
typedef double					float64_t;

// C-compatibility:
typedef bool					_Bool;

typedef std::vector<uint8_t>	Blob;

template<class Value>
	using Optional = boost::optional<Value>;

template<class Type>
	using Shared = std::shared_ptr<Type>;

template<class Type>
	using Unique = std::unique_ptr<Type>;

template<class Type>
	using Weak = std::weak_ptr<Type>;

#endif

