#pragma ident "$Id: xRinexEphemerisStore.hpp 3140 2012-06-18 15:03:02Z susancummins $"
// file: xRinexEphemerisStore.hpp

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

#ifndef XRINEXEPHEMERISSTORE_HPP
#define XRINEXEPHEMERISSTORE_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "Rinex3EphemerisStore.hpp"
#include <fstream>


using namespace std;

 
class xRinexEphemerisStore: public CPPUNIT_NS :: TestFixture
{
	//Test Fixture information, e.g. what tests will be run
	CPPUNIT_TEST_SUITE (xRinexEphemerisStore);
	CPPUNIT_TEST (RESTest);
	CPPUNIT_TEST (BCESfindEphTest);
	CPPUNIT_TEST (BCESgetXvtTest);
	CPPUNIT_TEST (BCESgetXvt2Test);
	CPPUNIT_TEST (BCESgetSatHealthTest);
	CPPUNIT_TEST (BCESdumpTest);
	CPPUNIT_TEST (BCESaddEphemerisTest);
	CPPUNIT_TEST (BCESeditTest);
	CPPUNIT_TEST (BCESwiperTest);
	CPPUNIT_TEST (BCESclearTest);
	CPPUNIT_TEST (BCESfindUserTest);
	CPPUNIT_TEST (BCESfindNearTest);
	CPPUNIT_TEST (BCESaddToListTest);
	CPPUNIT_TEST_SUITE_END ();

	public:
		void setUp (void); //Sets up test environment
		
	protected:
		
		void RESTest (void); 
		void BCESfindEphTest (void);
		void BCESgetXvtTest (void);
		void BCESgetXvt2Test (void);
		void BCESgetSatHealthTest (void);
		void BCESdumpTest (void);
		void BCESaddEphemerisTest (void);
		void BCESeditTest (void);
		void BCESwiperTest (void);
		void BCESclearTest (void);
		void BCESfindUserTest (void);
		void BCESfindNearTest (void);
		void BCESaddToListTest (void);
		bool fileEqualTest (char[], char[]);

	private:		

};	
#endif
