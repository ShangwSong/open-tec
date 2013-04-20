#pragma ident "$Id: DebugUtils.cpp 3149 2012-06-20 16:22:19Z prestonherrmann $"

/**
 * @file DebugUtils.cpp
 * 
 */
 
//============================================================================
//
//  This file is part of GPSTk, the GPS Toolkit.
//
//  The GPSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 2.1 of the License, or
//  any later version.
//
//  The GPSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GPSTk; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
//
//  Wei Yan - Chinese Academy of Sciences . 2011
//
//============================================================================

#include "DebugUtils.hpp"
#include <sstream>

namespace gpstk
{
   void DebugUtils::assertion(const char* cond, const char* file, int line)
   {
      throw AssertFailedException(what(cond, file, line));
   }

   void DebugUtils::nullPointer(const char* ptr, const char* file, int line)
   {
      throw NullPointerException(what(ptr, file, line));
   }

   std::string DebugUtils::what(const char* msg, const char* file, int line)
   {
      std::ostringstream str;
      if (msg) str << msg << " ";
      str << "in file \"" << file << "\", line " << line;
      return str.str();
   }
   
}   // End of namespace gpstk

