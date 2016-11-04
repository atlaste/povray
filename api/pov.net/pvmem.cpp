//******************************************************************************
///
/// @file windows/pvmem.cpp
///
/// This module implements windows-specific memory handling routines.
///
/// @author Christopher J. Cason
///
/// @copyright
/// @parblock
///
/// Persistence of Vision Ray Tracer ('POV-Ray') version 3.7.
/// Copyright 1991-2015 Persistence of Vision Raytracer Pty. Ltd.
///
/// POV-Ray is free software: you can redistribute it and/or modify
/// it under the terms of the GNU Affero General Public License as
/// published by the Free Software Foundation, either version 3 of the
/// License, or (at your option) any later version.
///
/// POV-Ray is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU Affero General Public License for more details.
///
/// You should have received a copy of the GNU Affero General Public License
/// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///
/// ----------------------------------------------------------------------------
///
/// POV-Ray is based on the popular DKB raytracer version 2.12.
/// DKBTrace was originally written by David K. Buck.
/// DKBTrace Ver 2.0-2.12 were written by David K. Buck & Aaron A. Collins.
///
/// @endparblock
///
//******************************************************************************

#include "stdafx.h"

#define POVWIN_FILE
#define _WIN32_IE COMMONCTRL_VERSION

#include "syspovconfig.h"
#undef POV_VIDCAP_IMPL

#include <windows.h>
#include <crtdbg.h>
#include <errno.h>
#include <assert.h>
#include <memory>
#include <cstdint>

// this must be the last file included
#include "syspovdebug.h"

#undef new
#undef delete

namespace povwin
{

	void *win_malloc(size_t size)
	{
		void *result = malloc(size);
		if (result == NULL)
			throw std::bad_alloc();
		return result;
	}

	void *win_malloc(size_t size, const void *data, int line)
	{
		void *result = malloc(size);
		if (result == NULL)
			throw std::bad_alloc();
		return result;
	}

	void *win_realloc(void *p, size_t size)
	{
		void *result = realloc(p, size);
		if (result == NULL)
			throw std::bad_alloc();
		return result;
	}

	void *win_realloc(void *p, size_t size, const void *data, int line)
	{
		void *result = realloc(p, size);
		if (result == NULL)
			throw std::bad_alloc();
		return result;
	}

	void win_free(void *p)
	{
		free(p);
	}

	void win_free(void *p, const void *data, int line)
	{
		free(p);
	}

	char *win_strdup(const char *s)
	{
		char *result = _strdup(s);
		if (result == NULL)
			throw std::bad_alloc();
		return result;
	}

	char *win_strdup(const char *s, const void *data, int line)
	{
		char *result = _strdup(s);
		if (result == NULL)
			throw std::bad_alloc();
		return result;
	}

	void WinMemStage(bool BeginRender, void *cookie)
	{
	}

	bool WinMemReport(bool global, uint64& allocs, uint64& frees, int64& current, uint64& peak, uint64 &smallest, uint64 &largest)
	{
		allocs = frees = current = smallest = largest = 0;
		peak = 0;
		return true;
	}

	void WinMemThreadStartup()
	{}

	void WinMemThreadCleanup()
	{}
} // end of namespace povwin

