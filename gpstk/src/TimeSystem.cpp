#pragma ident "$Id: TimeSystem.cpp 3140 2012-06-18 15:03:02Z susancummins $"


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
//  Copyright 2004, The University of Texas at Austin
//
//============================================================================

//============================================================================
//
//This software developed by Applied Research Laboratories at the University of
//Texas at Austin, under contract to an agency or agencies within the U.S. 
//Department of Defense. The U.S. Government retains all rights to use,
//duplicate, distribute, disclose, or release this software. 
//
//Pursuant to DoD Directive 523024 
//
// DISTRIBUTION STATEMENT A: This software has been approved for public 
//                           release, distribution is unlimited.
//
//=============================================================================

#include "TimeSystem.hpp"

using namespace std;

namespace gpstk
{
   // Static initialization of const std::strings for asString().
   // Must parallel enum Systems in TimeSystem.hpp.
   // NB: DO NOT use std::map here; on some systems initialization fails.
   const string TimeSystem::Strings[count] =
     {
       string("Unknown"),
       string("Any"),
       string("GPS"),
       string("GLO"),
       string("GAL"),
       string("UTC"),
       string("TAI")
     };

   void TimeSystem::setTimeSystem(const Systems& sys)
      throw()
   {
      if(sys < 0 || sys >= count)
         system = Unknown;
      else
         system = sys;
   }

   void TimeSystem::fromString(const string str)
      throw()
   {
      system = Unknown;
      for(int i=0; i<count; i++) {
         if(Strings[i] == str) {
            system = static_cast<Systems>(i);
            break;
         }
      }
   }

   ostream& operator<<(ostream os, const TimeSystem& ts)
   {
      return os << ts.asString();
   }
}   // end namespace
