#pragma ident "$Id: RinexObsID.cpp 3140 2012-06-18 15:03:02Z susancummins $"
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

/**
 * @file RinexObsID.cpp
 * gpstk::RinexObsID - Identifies types of observations
 */

#include "RinexObsID.hpp"

namespace gpstk
{
   /// Construct this object from the string specifier
   RinexObsID::RinexObsID(const std::string& strID) throw(InvalidParameter)
   {
      if(!isValidRinexObsID(strID)) {
         InvalidParameter ip(strID + " is not a valid RinexObsID");
         GPSTK_THROW(ip);
      }
      try {
         ObsID obsid(strID);
         *this = RinexObsID(obsid);
      }
      catch(InvalidParameter& ip) { GPSTK_RETHROW(ip); }
   }

   RinexObsID::RinexObsID(const RinexObsHeader::RinexObsType& rot)
      : ObsID()
   {
      // Note that the choice of tracking code for L1, L2, S1, S2 are arbitrary 
      // since they are ambiguous in the rinex 2 specifications
      // L1 -> L1P; P1 -> C1P; C1 -> C1C; S1 -> S1P; D1 -> D1P
      if      (rot == RinexObsHeader::L1) {type=otPhase;   band=cbL1; code=tcP;}
      else if (rot == RinexObsHeader::P1) {type=otRange;   band=cbL1; code=tcP;}
      else if (rot == RinexObsHeader::C1) {type=otRange;   band=cbL1; code=tcCA;}
      else if (rot == RinexObsHeader::S1) {type=otSNR;     band=cbL1; code=tcP;}
      else if (rot == RinexObsHeader::D1) {type=otDoppler; band=cbL1; code=tcP;}
      // L2 -> L2P; P2 -> C2P; C2 -> C2X; S2 -> S2P; D2 -> D2P
      else if (rot == RinexObsHeader::L2) {type=otPhase;   band=cbL2; code=tcP;}
      else if (rot == RinexObsHeader::P2) {type=otRange;   band=cbL2; code=tcP;}
      else if (rot == RinexObsHeader::C2) {type=otRange;   band=cbL2; code=tcC2LM;}
      else if (rot == RinexObsHeader::S2) {type=otSNR;     band=cbL2; code=tcP;}
      else if (rot == RinexObsHeader::D2) {type=otDoppler; band=cbL2; code=tcP;}
   }

   // Represent this object using the Rinex3 notation
   std::string RinexObsID::asString() const
   {
      char buff[4];

      buff[0] = ot2char[type];
      buff[1] = cb2char[band];
      buff[2] = tc2char[code];
      buff[3] = 0;
      return std::string(buff);
   }

   namespace StringUtils
   {
      // convert this object to a string representation
      std::string asString(const RinexObsID& p)
      {
         return p.asString();
      }
   }

   // RINEX 3.01 document, section 5.1
   // GPS
   //       L1   C,S,L,X,P,W,Y,M,N(but not C1N)       G 1 CSLXPWYMN (but not C1N)
   //       L2   C,D,S,L,X,P,W,Y,M,N(but not C2N)     G 2 CDSLXPWYMN (but not C1N)
   //       L5   I,Q,X                                G 5 IQX
   // GLO
   //       G1   C,P                                  R 1 CP
   //       G2   C,P                                  R 2 CP
   // GAL
   //       E1   A,B,C,X,Z                            E 1 ABCXZ
   //       E5a  I,Q,X                                E 5 IQX
   //       E5b  I,Q,X                                E 7 IQX
   //       E5ab I,Q,X                                E 8 IQX
   //       E6   A,B,C,X,Z                            E 6 ABCXZ
   // SBAS
   //       L1   C                                    S 1 C
   //       L5   I,Q,X                                S 5 IQX
   // COM
   //       E1   -                                    C 1 -
   //       E2   I,Q,X                                C 2 IQX
   //       E5b  I,Q,X                                C 7 IQX
   //       E6   I,Q,X                                C 6 IQX

   // Determine if the given ObsID is valid. If the input string is 3 
   // characters long, the system is assumed to be GPS. If this string is 4
   // characters long, the first character is the system designator as
   // described in the Rinex 3 specification.
   bool isValidRinexObsID(const std::string& strID)
   {
      int i(strID.length()-3);
      if(i < 0 || i > 1)
         return false;

      char sys;
      std::string id;

      if(i == 1) {
         sys = strID[0];
         id = strID.substr(1);
         return isValidRinexObsID(id,sys);
      }

      // test all RINEX systems
      std::string syss(ObsID::validRinexSystems);
      for(i=0; i<syss.size(); i++)
         if(isValidRinexObsID(strID,syss[i])) return true;

      return false;
   }

   // Determine if the given ObsID is valid, for the given system
   bool isValidRinexObsID(const std::string& strID, const char sys)
   {
      if(strID.length() != 3)
         return false;
      char ot(strID[0]);
      char cb(strID[1]);
      char tc(strID[2]);
      std::string codes(ObsID::validRinexTrackingCodes[sys][cb]);
      if(codes.find(std::string(1,tc)) == std::string::npos)
         return false;
      if(sys == 'G' && ot == 'C' && tc == 'N')           // the one exception
         return false;

      return true;
   }

}
