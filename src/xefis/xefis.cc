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

// Standards:
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <memory>

// System:
#include <signal.h>
#include <locale.h>
#include <string.h>

// Qt:
#include <QtCore/QTextCodec>

// Xefis:
#include <xefis/config/all.h>
#include <xefis/core/application.h>
#include <xefis/core/fail.h>
#include <xefis/core/services.h>
#include <xefis/utility/backtrace.h>


int main (int argc, char** argv, char**)
{
	signal (SIGILL, xf::fail);
	signal (SIGFPE, xf::fail);
	signal (SIGSEGV, xf::fail);

	setenv ("LC_ALL", "POSIX", 1);
	setlocale (LC_ALL, "POSIX");

	try {
		if (argc == 2 && (strcmp (argv[1], "-v") == 0 || strcmp (argv[1], "--version") == 0))
		{
			std::cout << "Xefis" << std::endl;
			std::cout << "Commit: " << xf::Version::commit << std::endl;
			std::cout << "Branch: " << xf::Version::branch << std::endl;
			std::clog << "Features: ";
			std::vector<const char*> features = xf::Services::features();
			std::copy (features.begin(), features.end(), std::ostream_iterator<const char*> (std::clog, " "));
			std::clog << std::endl;
		}
		else
		{
			auto app = std::make_unique<xf::Application> (argc, argv);
			app->exec();
		}
	}
	catch (xf::Application::QuitInstruction)
	{
		return EXIT_SUCCESS;
	}
	catch (xf::Exception& e)
	{
		std::cerr << e << std::endl;
	}
	catch (...)
	{
		xf::Backtrace::clog();
		throw;
	}

	return EXIT_SUCCESS;
}

