#pragma ident "$Id: xGPSWeekZcount.hpp 3140 2012-06-18 15:03:02Z susancummins $"

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
//  Copyright 2009, The University of Texas at Austin
//
//============================================================================

#ifndef XGPSWEEKZCOUNT_HPP
#define XGPSWEEKZCOUNT_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "GPSWeekZcount.hpp"

using namespace std;

class xGPSWeekZcount: public CPPUNIT_NS :: TestFixture
{
	CPPUNIT_TEST_SUITE (xGPSWeekZcount);
	CPPUNIT_TEST (setFromInfoTest);
	CPPUNIT_TEST (operatorTest);
	CPPUNIT_TEST_SUITE_END ();

	public:
		void setUp (void);

	protected:
		void operatorTest (void);
		void setFromInfoTest (void);

	private:

};

#endif
