#pragma ident "$Id: ObsID.cpp 3140 2012-06-18 15:03:02Z susancummins $"

/**
 * @file ObsID.cpp
 * gpstk::ObsID - Identifies types of observations
 */

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



#include "ObsID.hpp"


namespace gpstk
{
   // descriptions (strings) of each code, carrier and type
   std::map< ObsID::TrackingCode,    std::string > ObsID::tcDesc;
   std::map< ObsID::CarrierBand,     std::string > ObsID::cbDesc;
   std::map< ObsID::ObservationType, std::string > ObsID::otDesc;
   // mappings between code, carrier, type and characters
   std::map< char, ObsID::ObservationType> ObsID::char2ot;
   std::map< char, ObsID::CarrierBand> ObsID::char2cb;
   std::map< char, ObsID::TrackingCode> ObsID::char2tc;
   std::map< ObsID::ObservationType, char > ObsID::ot2char;
   std::map< ObsID::CarrierBand, char > ObsID::cb2char;
   std::map< ObsID::TrackingCode, char> ObsID::tc2char;

   // map of valid RINEX tracking codes, systems and frequency
   std::map<char, std::map<char, std::string> > ObsID::validRinexTrackingCodes;

   // string containing the system characters for all valid RINEX systems.
   std::string ObsID::validRinexSystems;

   // object that forces initialization of the maps
   ObsID::Initializer singleton;

   // initialize the maps
   ObsID::Initializer::Initializer()
   {
      otDesc[otUnknown]   = "UnknownType";   //Rinex (sp)
      otDesc[otAny]       = "AnyType";       //Rinex *
      otDesc[otRange]     = "pseudorange";   //Rinex C
      otDesc[otPhase]     = "phase";         //Rinex L
      otDesc[otDoppler]   = "doppler";       //Rinex D
      otDesc[otSNR]       = "snr";           //Rinex S
      otDesc[otChannel]   = "channel";       //Rinex  
      otDesc[otIono]      = "iono";          //Rinex  
      otDesc[otSSI]       = "ssi";           //Rinex  
      otDesc[otLLI]       = "lli";           //Rinex  
      otDesc[otTrackLen]  = "tlen";          //Rinex  
      otDesc[otNavMsg]    = "navmsg";        //Rinex
      otDesc[otUndefined] = "undefined";     //Rinex -

      cbDesc[cbUnknown]   = "UnknownBand";   //Rinex (sp)
      cbDesc[cbAny]       = "AnyBand";       //Rinex *
      cbDesc[cbZero]      = "";              //Rinex  
      cbDesc[cbL1]        = "L1";            //Rinex 1
      cbDesc[cbL2]        = "L2";            //Rinex 2
      cbDesc[cbL5]        = "L5";            //Rinex 5
      cbDesc[cbL1L2]      = "L1+L2";         //Rinex  
      cbDesc[cbG1]        = "G1";            //Rinex 1
      cbDesc[cbG2]        = "G2";            //Rinex 2
      cbDesc[cbE5b]       = "E5b";           //Rinex 7
      cbDesc[cbE5ab]      = "L5a+b";         //Rinex 8
      cbDesc[cbE6]        = "E6";            //Rinex 6
      cbDesc[cbE1]        = "E1";            //Rinex 1
      cbDesc[cbE2]        = "E2";            //Rinex 2
      cbDesc[cbC6]        = "C6";            //Rinex 6
      cbDesc[cbUndefined] = "undefined";     //Rinex -

      tcDesc[tcUnknown]   = "UnknownCode";   //Rinex (sp)
      tcDesc[tcAny]       = "AnyCode";       //Rinex *
      tcDesc[tcCA]        = "GPSC/A";        //Rinex C    // GPScivil
      tcDesc[tcP]         = "GPSP";          //Rinex P    // GPSprecise
      tcDesc[tcY]         = "GPSY";          //Rinex Y    // GPSprecise_encrypted
      tcDesc[tcW]         = "GPScodelessZ";  //Rinex W    // GPSprecise_encrypted_codeless_Z
      tcDesc[tcN]         = "GPSsquare";     //Rinex N    // GPSprecise_encrypted_codeless_squaring
      tcDesc[tcD]         = "GPScodeless";   //Rinex D    // GPSprecise_encrypted_codeless_other
      tcDesc[tcM]         = "GPSM";          //Rinex M    // GPSmilitary
      tcDesc[tcC2M]       = "GPSC2M";        //Rinex S    // GPScivil_M
      tcDesc[tcC2L]       = "GPSC2L";        //Rinex L    // GPScivil_L
      tcDesc[tcC2LM]      = "GPSC2L+M";      //Rinex X    // GPScivil_L+M
      tcDesc[tcI5]        = "GPSI5";         //Rinex I    // GPScivil_I
      tcDesc[tcQ5]        = "GPSQ5";         //Rinex Q    // GPScivil_Q
      tcDesc[tcIQ5]       = "GPSI+Q5";       //Rinex X    // GPScivil_I+Q
      tcDesc[tcGCA]       = "GLOC/A";        //Rinex C    // GLOcivil
      tcDesc[tcGP]        = "GLOP";          //Rinex P    // GLOprecise
      tcDesc[tcA]         = "GALA";          //Rinex A    // GAL
      tcDesc[tcB]         = "GALB";          //Rinex B    // GAL
      tcDesc[tcC]         = "GALC";          //Rinex C    // GAL
      tcDesc[tcBC]        = "GALB+C";        //Rinex X    // GAL
      tcDesc[tcABC]       = "GALA+B+C";      //Rinex Z    // GAL
      tcDesc[tcIE5]       = "GALIE5";        //Rinex I    // GAL
      tcDesc[tcQE5]       = "GALQE5";        //Rinex Q    // GAL
      tcDesc[tcIQE5]      = "GALI+QE5";      //Rinex X    // GAL
      tcDesc[tcSCA]       = "SBASC/A";       //Rinex C    // SBAS civil code
      tcDesc[tcSI5]       = "SBASI5";        //Rinex I    // SBAS L5 I code
      tcDesc[tcSQ5]       = "SBASQ5";        //Rinex Q    // SBAS L5 Q code
      tcDesc[tcSIQ5]      = "SBASI+Q5";      //Rinex X    // SBAS L5 I+Q code
      tcDesc[tcCI2]       = "COMIC2";        //Rinex I    // Compass E2 I code
      tcDesc[tcCQ2]       = "COMQC2";        //Rinex Q    // Compass E2 Q code
      tcDesc[tcCIQ2]      = "COMI+QC2";      //Rinex X    // Compass E2 I code
      tcDesc[tcCI5]       = "COMIC5";        //Rinex I    // Compass E5 I+Q code
      tcDesc[tcCQ5]       = "COMQC5";        //Rinex Q    // Compass E5 Q code
      tcDesc[tcCIQ5]      = "COMI+QC5";      //Rinex X    // Compass E5 I+Q code
      tcDesc[tcCI6]       = "COMIC6";        //Rinex I    // Compass E6 I code
      tcDesc[tcCQ6]       = "COMQC6";        //Rinex Q    // Compass E6 Q code
      tcDesc[tcCIQ6]      = "COMI+QC6";      //Rinex X    // Compass E6 I+Q code
      tcDesc[tcUndefined] = "undefined";

      if (otDesc.size() != (int)otLast)
         std::cerr << "Error in otDesc" << std::endl;
      if (cbDesc.size() != (int)cbLast)
         std::cerr << "Error in cbDesc" << std::endl;
      if (tcDesc.size() != (int)tcLast)
         std::cerr << "Error in tcDesc" << std::endl;

      // The following definitions really should only describe the items that are
      // in the Rinex 3 specification. If an application needs additional ObsID
      // types to be able to be translated to/from Rinex3, the additional types
      // must be added by the application.
      char2ot[' '] = otUnknown;
      char2ot['*'] = otAny;
      char2ot['C'] = otRange;
      char2ot['L'] = otPhase;
      char2ot['D'] = otDoppler;
      char2ot['S'] = otSNR;
      char2ot['-'] = otUndefined;

      char2cb[' '] = cbUnknown;
      char2cb['*'] = cbAny;
      char2cb['1'] = cbL1;
      char2cb['2'] = cbL2;
      char2cb['5'] = cbL5;
      char2cb['6'] = cbE6;
      char2cb['7'] = cbE5b;
      char2cb['8'] = cbE5ab;
      char2cb['-'] = cbUndefined;

      char2tc[' '] = tcUnknown;
      char2tc['*'] = tcAny;
      char2tc['C'] = tcCA;
      char2tc['P'] = tcP;  
      char2tc['W'] = tcW;
      char2tc['Y'] = tcY;
      char2tc['M'] = tcM;
      char2tc['N'] = tcN;
      char2tc['D'] = tcD;
      char2tc['S'] = tcC2M;
      char2tc['L'] = tcC2L;
      char2tc['X'] = tcC2LM;
      char2tc['I'] = tcI5;
      char2tc['Q'] = tcQ5;
      char2tc['A'] = tcA;
      char2tc['B'] = tcB;
      char2tc['Z'] = tcABC;
      char2tc['-'] = tcUndefined;

      // Since some of the items in the enums don't have corresponding RINEX
      // definitions, make sure there is an entry for all values
      for (int i=otUnknown; i<otLast; i++) ot2char[(ObservationType)i] = ' ';
      for (int i=cbUnknown; i<cbLast; i++) cb2char[(CarrierBand)i] = ' ';
      for (int i=tcUnknown; i<tcLast; i++) tc2char[(TrackingCode)i] = ' ';

      // Here the above three maps are reversed to speed up the runtime
      for (std::map< char, ObservationType>::const_iterator i=char2ot.begin();
           i != char2ot.end(); i++)
         ot2char[i->second] = i->first;

      for (std::map< char, CarrierBand>::const_iterator i=char2cb.begin();
           i != char2cb.end(); i++)
         cb2char[i->second] = i->first;

      for (std::map< char, TrackingCode>::const_iterator i=char2tc.begin();
           i != char2tc.end(); i++)
         tc2char[i->second] = i->first;

      // And add the couple 'special' cases
      cb2char[cbG1] = '1';
      cb2char[cbG2] = '2';
      cb2char[cbE1] = '1';
      cb2char[cbE2] = '2';

      tc2char[tcC]    = 'C';
      tc2char[tcGCA]  = 'C';
      tc2char[tcGP]   = 'P';
      tc2char[tcIE5]  = 'I';
      tc2char[tcQE5]  = 'Q';
      tc2char[tcIQE5] = 'X';
      tc2char[tcIQ5]  = 'X';
      tc2char[tcBC]   = 'X';
      tc2char[tcSCA]  = 'C';
      tc2char[tcSI5]  = 'I';
      tc2char[tcSQ5]  = 'Q';
      tc2char[tcSIQ5] = 'X';
      tc2char[tcCI2]  = 'I';
      tc2char[tcCQ2]  = 'Q';
      tc2char[tcCIQ2] = 'X';
      tc2char[tcCI5]  = 'I';
      tc2char[tcCQ5]  = 'Q';
      tc2char[tcCIQ5] = 'X';
      tc2char[tcCI6]  = 'I';
      tc2char[tcCQ6]  = 'Q';
      tc2char[tcCIQ6] = 'X';

      validRinexSystems = "GRESC";

      // NB these tc characters are ORDERED ~best to worst
      validRinexTrackingCodes['G']['1'] = "PYWLMIQSXCN* ";      // except no C1N
      validRinexTrackingCodes['G']['2'] = "PYWLMIQSXCDN* ";     // except no C2N
      validRinexTrackingCodes['G']['5'] = "XIQ* ";
      validRinexTrackingCodes['R']['1'] = "PC* ";
      validRinexTrackingCodes['R']['2'] = "PC* ";
      validRinexTrackingCodes['E']['1'] = "ABCIQXZ* ";
      validRinexTrackingCodes['E']['5'] = "XIQ* ";
      validRinexTrackingCodes['E']['7'] = "XIQ* ";
      validRinexTrackingCodes['E']['8'] = "XIQ* ";
      validRinexTrackingCodes['E']['6'] = "ABCIQXZ* ";
      validRinexTrackingCodes['S']['1'] = "C* ";
      validRinexTrackingCodes['S']['5'] = "XIQ* ";
      validRinexTrackingCodes['C']['1'] = "* ";
      validRinexTrackingCodes['C']['2'] = "XIQ* ";
      validRinexTrackingCodes['C']['7'] = "XIQ* ";
      validRinexTrackingCodes['C']['6'] = "XIQ* ";
   }


   // Construct this object from the string specifier
   ObsID::ObsID(const std::string& strID) throw(InvalidParameter)
   {
      int i = strID.length() - 3;
      if ( i < 0 || i > 1)
      {
         InvalidParameter e("identifier must be 3 or 4 characters long");
         GPSTK_THROW(e);
      }

      char sys = i ? strID[0] : 'G';
      char ot = strID[i];
      char cb = strID[i+1];
      char tc = strID[i+2];
      
      if (!char2ot.count(ot) || !char2cb.count(cb) || !char2tc.count(tc))
         idCreator(strID.substr(i,3));

      type = char2ot[ ot ];
      band = char2cb[ cb ];
      code = char2tc[ tc ];

      /// This next block takes care of fixing up the codes that are reused
      /// between the various signals
      if (sys == 'G') // GPS
      {
         if (tc=='X' && band==cbL5)
            code = tcIQ5;
      }
      if (sys == 'E') // Galileo
      {
         switch (code)
         {
            case tcCA: code = tcC; break;
            case tcI5: code = tcIE5; break;
            case tcQ5: code = tcQE5; break;
         }
         if (tc == 'X')
         {
            if (band == cbL1 || band == cbE6)
               code = tcBC;
            else if (band == cbL5 || band == cbE5b || band == cbE5ab)
               code = tcIQE5;
         }
      }
      else if (sys == 'R') // Glonass
      {
         switch (code)
         {
            case tcCA: code = tcGCA; break;
            case tcP: code = tcGP; break;
         }
         switch (band)
         {
            case cbL1: band = cbG1; break;
            case cbL2: band = cbG2; break;
         }
      }
      else if (sys == 'S') // SBAS or Geosync
      {
         switch (code)
         {
            case tcCA: code = tcSCA; break;     // 'C'
            case tcI5: code = tcSI5; break;     // 'I'
            case tcQ5: code = tcSQ5; break;     // 'Q'
            case tcC2LM: code = tcSIQ5; break;  // 'X'
         }
      }
      else if (sys == 'C') // Compass
      {
         //if(band == cbL1) {
         //   band = cbE1;
         //   if(code == tcCA) code = tcCCA;
         //}
         if(band == cbL2) {
            band = cbE2;
            switch (code)
            {
               case tcI5: code = tcCI2; break;     // 'I'
               case tcQ5: code = tcCQ2; break;     // 'Q'
               case tcC2LM: code = tcCIQ2; break;  // 'X'
            }
         }
         if(band == cbE5b) switch (code)
         {
            case tcI5: code = tcCI5; break;     // 'I'
            case tcQ5: code = tcCQ5; break;     // 'Q'
            case tcC2LM: code = tcCIQ5; break;  // 'X'
         }
         if(band == cbE6) {
            band = cbC6;
            switch (code)
            {
               case tcI5: code = tcCI6; break;     // 'I'
               case tcQ5: code = tcCQ6; break;     // 'Q'
               case tcC2LM: code = tcCIQ6; break;  // 'X'
            }
         }
      } // end of checking which GNSS system this obs is for
   }


   // Convenience output method
   std::ostream& ObsID::dump(std::ostream& s) const
   {
      s << ObsID::cbDesc[band] << " "
        << ObsID::tcDesc[code] << " "
        << ObsID::otDesc[type];
      return s;
   } // ObsID::dump()




   // This is used to register a new ObsID & Rinex 3 identifier.  The syntax for the
   // Rinex 3 identifier is the same as for the ObsID constructor. 
   // If there are spaces in the provided identifier, they are ignored
   ObsID ObsID::newID(const std::string& strID, const std::string& desc)
      throw(InvalidParameter)
   {
      if (char2ot.count(strID[0]) && 
          char2cb.count(strID[1]) && 
          char2tc.count(strID[2]))
          GPSTK_THROW(InvalidParameter("Identifier " + strID + " already defined."));

      return idCreator(strID, desc);
   }


   ObsID ObsID::idCreator(const std::string& strID, const std::string& desc)
   {
      char ot = strID[0];
      ObservationType type;
      if (!char2ot.count(ot))
      {
         type = (ObservationType)otDesc.size();
         otDesc[type] = desc;
         char2ot[ot] = type;
         ot2char[type] = ot;
      }
      else
         type = char2ot[ot];

      char cb = strID[1];
      CarrierBand band;
      if (!char2cb.count(cb))
      {
         band = (CarrierBand)cbDesc.size();
         cbDesc[band] = desc;
         char2cb[cb] = band;
         cb2char[band] = cb;
      }
      else
         band = char2cb[cb];

      char tc = strID[2];
      TrackingCode code;
      if (!char2tc.count(tc))
      {
         code = (TrackingCode) tcDesc.size();
         tcDesc[code] = desc;
         char2tc[tc] = code;
         tc2char[code] = tc;
      }
      else
         code = char2tc[tc];
      
      return ObsID(type, band, code);
   }


   // Equality requires all fields to be the same unless the field is unknown
   bool ObsID::operator==(const ObsID& right) const
   {
      bool ot = type == otAny || right.type == otAny || type == right.type;
      bool cb = band == cbAny || right.band == cbAny || band == right.band;
      bool tc = code == tcAny || right.code == tcAny || code == right.code;
      return ot && cb && tc;
   }


   // This ordering is somewhat arbitrary but is required to be able
   // to use an ObsID as an index to a std::map. If an application needs
   // some other ordering, inherit and override this function.
   bool ObsID::operator<(const ObsID& right) const
   {
      if (band == right.band)
         if (code == right.code)
            return type < right.type;
         else
            return code < right.code;
      else
         return band < right.band;

      // This should never be reached...
      return false;
   }


   namespace StringUtils
   {
      // convert this object to a string representation
      std::string asString(const ObsID& p)
      {
         std::ostringstream oss;
         p.dump(oss);
         return oss.str();
      }
   }


   // stream output for ObsID
   std::ostream& operator<<(std::ostream& s, const ObsID& p)
   {
      p.dump(s);
      return s;
   }

}
