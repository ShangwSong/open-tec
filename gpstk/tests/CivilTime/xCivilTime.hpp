#pragma ident "$Id: xCivilTime.hpp 3140 2012-06-18 15:03:02Z susancummins $"

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

#ifndef XCIVILTIME_HPP
#define XCIVILTIME_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "CivilTime.hpp"

using namespace std;

class xCivilTime: public CPPUNIT_NS :: TestFixture
{
	CPPUNIT_TEST_SUITE (xCivilTime);
	CPPUNIT_TEST (setFromInfoTest);
	CPPUNIT_TEST (operatorTest);
    CPPUNIT_TEST (resetTest);
    CPPUNIT_TEST (timeSystemTest);
	CPPUNIT_TEST (printfTest);
	CPPUNIT_TEST_SUITE_END ();

	public:
		void setUp (void);

	protected:
		void operatorTest (void);
		void setFromInfoTest (void);
        void resetTest (void);
		void timeSystemTest (void);
		void printfTest (void);
	private:

};

#endif
