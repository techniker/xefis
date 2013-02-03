/* vim:ts=4
 *
 * Copyleft 2012…2013  Michał Gawron
 * Marduk Unix Labs, http://mulabs.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Visit http://www.gnu.org/licenses/gpl-3.0.html for more information on licensing.
 */

#ifndef SI__LENGTH_H__INCLUDED
#define SI__LENGTH_H__INCLUDED

// Standard:
#include <cstddef>

// Local:
#include "value.h"


namespace SI {

class Length: public Value<double, Length>
{
	friend class Value<double, Length>;
	friend constexpr Length operator"" _m (long double);
	friend constexpr Length operator"" _m (unsigned long long);
	friend constexpr Length operator"" _km (long double);
	friend constexpr Length operator"" _km (unsigned long long);
	friend constexpr Length operator"" _ft (long double);
	friend constexpr Length operator"" _ft (unsigned long long);
	friend constexpr Length operator"" _nm (long double);
	friend constexpr Length operator"" _nm (unsigned long long);
	friend constexpr Length operator"" _mil (long double);
	friend constexpr Length operator"" _mil (unsigned long long);

  protected:
	/**
	 * Used by the _inhg, _hpa, _psi and similar suffix operators.
	 * To create an Length use these operators directly.
	 */
	constexpr
	Length (ValueType psi);

  public:
	constexpr
	Length() = default;

	constexpr
	Length (Length const&) = default;

	constexpr ValueType
	m() const noexcept;

	constexpr ValueType
	km() const noexcept;

	constexpr ValueType
	ft() const noexcept;

	constexpr ValueType
	nm() const noexcept;

	constexpr ValueType
	mil() const noexcept;
};


inline constexpr
Length::Length (ValueType m):
	Value (m)
{ }


inline constexpr Length::ValueType
Length::m() const noexcept
{
	return value();
}


inline constexpr Length::ValueType
Length::km() const noexcept
{
	return value() * 0.001;
}


inline constexpr Length::ValueType
Length::ft() const noexcept
{
	return value() * 3.280839895;
}


inline constexpr Length::ValueType
Length::nm() const noexcept
{
	return value() * 0.0005399568;
}


inline constexpr Length::ValueType
Length::mil() const noexcept
{
	return value() * 0.0006213711;
}


/*
 * Global functions
 */


inline constexpr Length
operator"" _m (long double m)
{
	return Length (static_cast<Length::ValueType> (m));
}


inline constexpr Length
operator"" _m (unsigned long long m)
{
	return Length (static_cast<Length::ValueType> (m));
}


inline constexpr Length
operator"" _km (long double km)
{
	return Length (static_cast<Length::ValueType> (km) * 1000.0);
}


inline constexpr Length
operator"" _km (unsigned long long km)
{
	return Length (static_cast<Length::ValueType> (km) * 1000.0);
}


inline constexpr Length
operator"" _ft (long double ft)
{
	return Length (static_cast<Length::ValueType> (ft) * 0.3048);
}


inline constexpr Length
operator"" _ft (unsigned long long ft)
{
	return Length (static_cast<Length::ValueType> (ft) * 0.3048);
}


inline constexpr Length
operator"" _nm (long double nm)
{
	return Length (static_cast<Length::ValueType> (nm) * 1852.0);
}


inline constexpr Length
operator"" _nm (unsigned long long nm)
{
	return Length (static_cast<Length::ValueType> (nm) * 1852.0);
}


inline constexpr Length
operator"" _mil (long double mil)
{
	return Length (static_cast<Length::ValueType> (mil) * 1609.344);
}


inline constexpr Length
operator"" _mil (unsigned long long mil)
{
	return Length (static_cast<Length::ValueType> (mil) * 1609.344);
}

} // namespace SI

#endif
