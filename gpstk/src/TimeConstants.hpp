#pragma ident "$Id: TimeConstants.hpp 3140 2012-06-18 15:03:02Z susancummins $"



#ifndef GPSTK_TIMECONSTANTS_HPP
#define GPSTK_TIMECONSTANTS_HPP

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

namespace gpstk
{
      /// Add this offset to convert Modified Julian Date to Julian Date.
   const double MJD_TO_JD = 2400000.5;
      /// 'Julian day' offset from MJD
   const long MJD_JDAY = 2400001L;
      /// 'Julian day' of GPS epoch (Jan. 6, 1980).
   const long GPS_EPOCH_JDAY = 2444245L;
      /// Modified Julian Date of GPS epoch (Jan. 6, 1980).
   const long GPS_EPOCH_MJD = 44244L;
      /// Modified Julian Date of UNIX epoch (Jan. 1, 1970).
   const long UNIX_MJD = 40587L;

      /// Seconds per half a GPS week.
   const long HALFWEEK = 302400L;
      /// Seconds per whole GPS week.
   const long FULLWEEK = 604800L;

      /// Weeks per GPS Epoch
   const long WEEK_PER_EPOCH = 1024L;

      /// Seconds per day.
   const long SEC_PER_DAY = 86400L;
      /// Days per second.
   const double DAY_PER_SEC = 1.0/SEC_PER_DAY;

      /// Milliseconds in a second.
   const long MS_PER_SEC = 1000L;
      /// Seconds per millisecond.
   const double SEC_PER_MS = 1.0/MS_PER_SEC;
   
      /// Milliseconds in a day.
   const long MS_PER_DAY = MS_PER_SEC * SEC_PER_DAY;
      /// Days per milliseconds.
   const double DAY_PER_MS = 1.0/MS_PER_DAY;

      /// Zcounts in a  day.
   const long ZCOUNT_PER_DAY = 57600L;
      /// Days in a Zcount
   const double DAY_PER_ZCOUNT = 1.0/ZCOUNT_PER_DAY;

      /// Zcounts in a week.
   const long ZCOUNT_PER_WEEK = 403200L;
      /// Weeks in a Zcount.
   const double WEEK_PER_ZCOUNT = 1.0/ZCOUNT_PER_WEEK;

} // namespace

#endif // GPSTK_TIMECONSTANTS_HPP
