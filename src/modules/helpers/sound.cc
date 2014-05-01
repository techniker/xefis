/* vim:ts=4
 *
 * Copyleft 2012…2014  Michał Gawron
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
#include <memory>
#include <random>

// Xefis:
#include <xefis/config/all.h>
#include <xefis/utility/qdom.h>
#include <xefis/core/stdexcept.h>
#include <xefis/core/module_manager.h>
#include <xefis/core/sound_manager.h>

// Local:
#include "sound.h"


XEFIS_REGISTER_MODULE_CLASS ("helpers/sound", Sound);


Sound::Alarm::Alarm (QDomElement const& alarm_element, Xefis::SoundManager* sound_manager):
	_sound_manager (sound_manager)
{
	_sound_file_path = alarm_element.attribute ("sound");
	_property.set_path (alarm_element.attribute ("property-path").toStdString());
	_priority = alarm_element.attribute ("priority").toInt();
	_repeat = alarm_element.hasAttribute ("repeat-period");
	if (_repeat)
		_repeat_period.parse (alarm_element.attribute ("repeat-period").toStdString());
	if (_repeat_period < 0_s)
		_repeat_period = 0_s;
}


bool
Sound::Alarm::operator< (Alarm const& other) const noexcept
{
	return _priority > other._priority;
}


bool
Sound::Alarm::check()
{
	bool test = *_property && (!_finished_timestamp || (_repeat && *_finished_timestamp + _repeat_period <= Time::now()));
	auto sptr = _sound.lock();

	if (_was_started && (!sptr || sptr->finished()))
	{
		_finished_timestamp = Time::now();
		_was_started = false;
	}

	if (test)
	{
		if (!sptr || sptr->finished())
		{
			_sound = _sound_manager->play (_sound_file_path);
			_was_started = true;
		}
	}
	else
		stop();

	return test;
}


void
Sound::Alarm::stop()
{
	_was_started = false;

	auto sptr = _sound.lock();

	if (sptr)
		sptr->stop();

	if (!*_property)
		_finished_timestamp.reset();
}


Sound::Group::Group (QDomElement const& group_element, Xefis::SoundManager* sound_manager)
{
	for (QDomElement e: group_element)
	{
		if (e == "alarm")
			_alarms.insert (std::make_unique<Alarm> (e, sound_manager));
		else
			throw Xefis::UnsupportedElementException (e);
	}
}


void
Sound::Group::check()
{
	bool mute_other = false;

	for (auto& alarm: _alarms)
	{
		if (mute_other)
			alarm->stop();
		else if (alarm->check())
			mute_other = true;
	}
}


Sound::Sound (Xefis::ModuleManager* module_manager, QDomElement const& config):
	Module (module_manager, config)
{
	for (QDomElement e: config)
	{
		if (e == "alarms")
		{
			for (QDomElement e2: e)
			{
				if (e2 == "group")
					_groups.insert (std::make_unique<Group> (e2, this->module_manager()->application()->sound_manager()));
				else
					throw Xefis::UnsupportedElementException (e2);
			}
		}
		else
			throw Xefis::UnsupportedElementException (e);
	}
}


void
Sound::data_updated()
{
	for (auto& group: _groups)
		group->check();
}
