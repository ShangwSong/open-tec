#pragma ident "$Id: xBinUtils.hpp 3140 2012-06-18 15:03:02Z susancummins $"
// file: xBinUtils.hpp

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

#ifndef XBINUTILS_HPP
#define XBINUTILS_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "BinUtils.hpp"

using namespace std;

 
class xBinUtils: public CPPUNIT_NS :: TestFixture
{
	//Test Fixture information, e.g. what tests will be run
	CPPUNIT_TEST_SUITE (xBinUtils);
	CPPUNIT_TEST (firstTest);
	CPPUNIT_TEST_SUITE_END ();

	public:
		void setUp (void); //Sets up test enviornment
		
	protected:
		
		void firstTest (void); // Test constructors
		
	private:
};		
		
#endif
