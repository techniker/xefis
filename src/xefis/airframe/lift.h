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

#ifndef XEFIS__CORE__AIRFRAME__LIFT_H__INCLUDED
#define XEFIS__CORE__AIRFRAME__LIFT_H__INCLUDED

// Standard:
#include <cstddef>

// Qt:
#include <QtXml/QDomElement>

// Xefis:
#include <xefis/config/all.h>
#include <xefis/airframe/types.h>


namespace Xefis {

class Lift
{
	typedef std::map<Angle, LiftCoefficient> Coefficients;

  public:
	// Ctor
	Lift (QDomElement const& config);

	/**
	 * Return lift coefficient (C_L) for given angle of attack.
	 *
	 * Uses linear interpolation.
	 */
	LiftCoefficient
	get_cl (Angle const& aoa) const;

	/**
	 * Return maximum possible lift.
	 */
	LiftCoefficient
	max_cl() const noexcept;

	/**
	 * Return angle for which C_L is maximum.
	 */
	Angle
	critical_aoa() const noexcept;

	/**
	 * Return AOA in normal regime for given C_L.
	 */
	Angle
	get_aoa_in_normal_regime (LiftCoefficient const& cl) const noexcept;

  private:
	Coefficients	_coeffs;
	LiftCoefficient	_max_cl;
	Angle			_critical_aoa;
};

} // namespace Xefis

#endif

