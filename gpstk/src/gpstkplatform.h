#pragma ident "$Id: gpstkplatform.h 3140 2012-06-18 15:03:02Z susancummins $"



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




/* This is where all platform specific includes, defines and crud should go.
   Unless, of course, it has to go somewhere else. :-)
*/

#ifndef GPSTK_GPSTKPLATFORM_H
#define GPSTK_GPSTKPLATFORM_H

#ifdef _MSC_VER

#include <cstdlib>

#define HAVE_STRING_H 1
#define STDC_HEADERS  1

// commented out - problems with doubly-defined types on Windows

//typedef __int8  int8_t;
//typedef __int16 int16_t;
//typedef __int32 int32_t;
//typedef __int64 int64_t;
//typedef unsigned __int8  uint8_t;
//typedef unsigned __int16 uint16_t;
//typedef unsigned __int32 uint32_t;
//typedef unsigned __int64 uint64_t;


//#include <sys/types.h>
#include <sys/timeb.h>
#include <stdint.h>


#elif defined __SUNPRO_CC

#include <sys/types.h>
#include <sys/timeb.h>

#else

#include <stdint.h>

#endif

typedef int8_t  Int8;
typedef int16_t Int16;
typedef int32_t Int32;
typedef int64_t Int64;

typedef uint8_t  UInt8;
typedef uint16_t UInt16;
typedef uint32_t UInt32;
typedef uint64_t UInt64;

#endif
