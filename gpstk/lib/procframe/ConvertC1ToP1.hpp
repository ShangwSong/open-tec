#pragma ident "$Id: ConvertC1ToP1.hpp 3140 2012-06-18 15:03:02Z susancummins $"

/**
* @file GPSTK_CONVERTC1TOP1_HPP.hpp
* Class to convert C1 to P1.
*/

#ifndef GPSTK_CONVERTC1TOP1_HPP
#define GPSTK_CONVERTC1TOP1_HPP

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
//  Wei Yan - Chinese Academy of Sciences . 2009, 2010, 2011
//
//============================================================================

#include "ProcessingClass.hpp"
#include "DCBDataReader.hpp"
#include <string>

namespace gpstk
{
      /** @addtogroup DataStructures */
      //@{

      /** This class convert C1 observable to P1 observable with add (P1-C1)
       *  Differences of Code Biases(P1-C1).
       *
       * This class is meant to be used with the GNSS data structures objects
       * found in "DataStructures" class.
       *
       * A typical way to use this class follows:
       *
       * @code
       *
       *
       *   gnssRinex gRin;
       *   ConvertC1ToP1 c1ToP1;
       *   coor.setDCBFile("P1C11001.DCB");
       *
       *   while(rin >> gRin)
       *   {
       *      gRin >> c1ToP1;
       *   }
       *
       * @endcode
       *
       * The "CorrectObservables" object will visit every satellite in the
       * GNSS data structure that is "gRin", it will do nothing if P1 is 
       * available, and it will insert new P1 as C1+Bp1-c1 if P1 is unavailable.
       * Be warned that if there no a DCB file be inputed, it will take C1 as 
       * P1. 
       * 
       * When used with the ">>" operator, this class returns the same
       * incoming data structure with the observables corrected.
       *
       */
   class ConvertC1ToP1 : public ProcessingClass
   {
   public:
         /// Default constructor
      ConvertC1ToP1(){}

         // Default deconstructor
      ~ConvertC1ToP1(){}
   
         /** Sets name of file containing DCBs data.
          * @param name      Name of the file containing DCB(P1-C1)
          */
      virtual ConvertC1ToP1& setDCBFile(const std::string& fileP1C1);

         /** Returns a satTypeValueMap object, adding the new data generated
          *  when calling this object.
          *
          * @param time      Epoch corresponding to the data.
          * @param gData     Data object holding the data.
          */
      virtual satTypeValueMap& Process( const CommonTime& time,
                                        satTypeValueMap& gData )
         throw(ProcessingException);


         /** Returns a gnnsSatTypeValue object, adding the new data
          *  generated when calling this object.
          *
          * @param gData    Data object holding the data.
          */
      virtual gnssSatTypeValue& Process(gnssSatTypeValue& gData)
         throw(ProcessingException)
      { Process(gData.header.epoch, gData.body); return gData; };


         /** Returns a gnnsRinex object, adding the new data generated
          *  when calling this object.
          *
          * @param gData    Data object holding the data.
          */
      virtual gnssRinex& Process(gnssRinex& gData)
         throw(ProcessingException)
      { Process(gData.header.epoch, gData.body); return gData; };

   
         /// Returns a string identifying this object.
      virtual std::string getClassName() const;


   protected:

         /// Object to access DCB data from CODE
      DCBDataReader dcbP1C1;


   }; // End of class 'ConvertC1ToP1'
   
      //@}

}  // End of namespace gpstk

#endif   // GPSTK_CONVERTC1TOP1_HPP



