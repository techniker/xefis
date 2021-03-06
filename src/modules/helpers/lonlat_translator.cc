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

// Standard:
#include <cstddef>

// Xefis:
#include <xefis/config/all.h>

// Local:
#include "lonlat_translator.h"


XEFIS_REGISTER_MODULE_CLASS ("helpers/lonlat-translator", LonLatTranslator);


LonLatTranslator::LonLatTranslator (xf::ModuleManager* module_manager, QDomElement const& config):
	Module (module_manager, config)
{
	parse_properties (config, {
		{ "input.longitude", _input_longitude, true },
		{ "input.latitude", _input_latitude, true },
		{ "output.longitude", _output_longitude, true },
		{ "output.latitude", _output_latitude, true },
	});
}


void
LonLatTranslator::data_updated()
{
	if (_input_longitude.valid_and_fresh())
		_output_longitude.write (_input_longitude->to_longitude_dms());

	if (_input_latitude.valid_and_fresh())
		_output_latitude.write (_input_latitude->to_latitude_dms());
}

