#pragma ident "$Id: NavSatOrbit.hpp 3140 2012-06-18 15:03:02Z susancummins $"

/**
 * @file NavSatOrbit.hpp
 * This class handle equation of motion for navigation satellites.
 */

#ifndef GPSTK_NAV_SAT_ORBIT_HPP
#define GPSTK_NAV_SAT_ORBIT_HPP


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
//  Wei Yan - Chinese Academy of Sciences . 2009, 2010
//
//============================================================================

#include "SatOrbit.hpp"

namespace gpstk
{

      /** @addtogroup GeoDynamics */
      //@{


      /** This class handle equation of motion for navigation satellites.
       */

   class NavSatOrbit : public SatOrbit
   {
   public:

         /// Default constructor
      NavSatOrbit();

         /// Default destructor
      virtual ~NavSatOrbit(){};


      virtual Vector<double> getDerivatives(const double&         t,
                                            const Vector<double>& y );


   protected:


   
   }; // End of class 'NavSatOrbit'

      // @}

}  // End of namespace 'gpstk'


#endif   // GPSTK_NAV_SAT_ORBIT_HPP

