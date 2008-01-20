/************************************************************************
               VMExecuteInterface.h - Copyright (c) 2008 The MWSE Project
                http://www.sourceforge.net/projects/mwse

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

**************************************************************************/

#pragma once

#include "mwseTypes.h"

using namespace mwse;

namespace mwse
{
	class VMExecuteInterface
	{
	public:
		//local variables
		virtual mwLong_t getLongVariable(int index) = 0;
		virtual mwLong_t getLongVariable(const char *id) = 0;
		virtual void setLongVariable(int index, mwLong_t value) = 0;
		virtual void setLongVariable(const char *id, mwLong_t value) = 0;

		virtual mwShort_t getShortVariable(int index) = 0;
		virtual mwShort_t getShortVariable(const char *id) = 0;
		virtual void setShortVariable(int index, mwShort_t value) = 0;
		virtual void setShortVariable(const char *id, mwShort_t value) = 0;

		virtual mwFloat_t getFloatVariable(int index) = 0;
		virtual mwFloat_t getFloatVariable(const char *id) = 0;
		virtual void setFloatVariable(int index, mwFloat_t value) = 0;
		virtual void setFloatVariable(const char *id, mwFloat_t value) = 0;

		//global variables
		virtual mwLong_t getLongGlobal(const char *id) = 0;
		virtual void setLongGlobal(const char *id, mwLong_t value) = 0;
		
		virtual mwShort_t getShortGlobal(const char *id) = 0;
		virtual void setShortGlobal(const char *id, mwShort_t value) = 0;

		virtual mwFloat_t getFloatGlobal(const char *id) = 0;
		virtual void setFloatGlobal(const char *id, mwFloat_t value) = 0;
	};
};