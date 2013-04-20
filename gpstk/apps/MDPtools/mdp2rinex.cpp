#pragma ident "$Id: mdp2rinex.cpp 3148 2012-06-20 16:13:26Z snelsen $"

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

/** @file Converts an MDP stream into RINEX obs/nav files */

#include "StringUtils.hpp"
#include "InOutFramework.hpp"
#include "GPSWeekSecond.hpp"
#include "RinexObsStream.hpp"
#include "RinexObsData.hpp"
#include "RinexNavStream.hpp"
#include "RinexNavData.hpp"
#include "YDSTime.hpp"
#include "MDPStream.hpp"
#include "MDPNavSubframe.hpp"
#include "MDPObsEpoch.hpp"

#include "RinexConverters.hpp"

using namespace std;
using namespace gpstk;


class MDP2Rinex : public InOutFramework<MDPStream, RinexObsStream>
{
public:
   MDP2Rinex(const string& applName)
      throw()
      : InOutFramework<MDPStream, RinexObsStream>(
         applName, "Converts an MDP stream to RINEX."),
        anyNav(false), fixHalf(true)
   {}

   bool initialize(int argc, char *argv[]) throw()
   {
      CommandOptionWithAnyArg 
         navFileOpt('n', "nav",
                    "Filename to write RINEX nav data to."),
         antPosOpt('p',"pos",
                   "Antenna position to write into obs file header. "
                   "Format as string: \"X Y Z\"."),
         thinningOpt('t', "thinning", 
                     "A thinning factor for the data, specified in seconds "
                     "between points. Default: none.");
      CommandOptionNoArg
         c2Opt('c', "l2c",
               "Enable output of L2C data in C2"),
         anyNavOpt('a',"any-nav-source", 
                "Accept subframes from any code/carrier");
         //fixHalfOpt('f',"fix-half-cycle","Apply half-cycle corrections to phase");

      if (!InOutFramework<MDPStream, RinexObsStream>::initialize(argc,argv))
         return false;

      if (navFileOpt.getCount())
         rinexNavOutput.open(navFileOpt.getValue()[0].c_str(), ios::out);
      else
         rinexNavOutput.clear(ios::badbit);

      if (thinningOpt.getCount())
      {
         thin = true;
         thinning = StringUtils::asInt(thinningOpt.getValue()[0]);
         if (debugLevel)
            cout << "Thinning data modulo " << thinning << " seconds." << endl;
      }
      else
         thin = false;

      if (anyNavOpt.getCount())
         anyNav = true;

      //if (fixHalfOpt.getCount())
      //   fixHalf = true;

      roh.valid |= RinexObsHeader::allValid21;
      roh.fileType = "Observation";
      roh.fileProgram = "mdp2rinex";
      roh.markerName = "Unknown";
      roh.observer = "Unknown";
      roh.agency = "Unknown";
      roh.antennaOffset = Triple(0,0,0);
      roh.wavelengthFactor[0] = 1;
      roh.wavelengthFactor[1] = 1;
      roh.recType = "Unknown MDP";
      roh.recVers = "Unknown";
      roh.recNo = "1";
      roh.antType = "Unknown";
      roh.antNo = "1";
      roh.obsTypeList.push_back(RinexObsHeader::C1);
      roh.obsTypeList.push_back(RinexObsHeader::P1);
      roh.obsTypeList.push_back(RinexObsHeader::L1);
      roh.obsTypeList.push_back(RinexObsHeader::D1);
      roh.obsTypeList.push_back(RinexObsHeader::S1);
      roh.obsTypeList.push_back(RinexObsHeader::P2);
      roh.obsTypeList.push_back(RinexObsHeader::L2);
      roh.obsTypeList.push_back(RinexObsHeader::D2);
      roh.obsTypeList.push_back(RinexObsHeader::S2);
        
      if (antPosOpt.getCount())
      {
        double x, y, z;
        sscanf(antPosOpt.getValue().front().c_str(),"%lf %lf %lf", &x, &y, &z);
        antPos = Triple(x,y,z);
      }
      else
        antPos = Triple(0,0,0);

      roh.antennaPosition = antPos;

      if (c2Opt.getCount())
         roh.obsTypeList.push_back(RinexObsHeader::C2);
        
      rnh.valid = RinexNavHeader::allValid21;
      rnh.fileType = "Navigation";
      rnh.fileProgram = "mdp2rinex";
      rnh.fileAgency = "Unknown";
      rnh.version = 2.1;


      firstObs = true;;
      firstEph = true;

      if (debugLevel>2)
         MDPHeader::debugLevel = debugLevel-2;
      MDPHeader::hexDump = debugLevel>3;

      if (!input)
      {
         cout << "Error: could not open input." << endl;
         return false;
      }

      if (!output)
      {
         cout << "Error: could not open output." << endl;
         return false;
      }

      return true;
   } // MDP2Rinex::initialize()
   
protected:
   virtual void spinUp()
   {}

   virtual void process(MDPNavSubframe& nav)
   {
      if (!rinexNavOutput)
         return;

      if (firstEph)
      {
         rinexNavOutput << rnh;
         if (debugLevel)
         	cout << "Got first nav SF" << endl;
      }

      nav.cookSubframe();
      if (debugLevel > 1)
      {
         if (nav.neededCooking)
            cout << "Subframe required cooking" << endl;
         if (nav.inverted)
            cout << "Subframe " << nav.getSFID() << " was inverted" << endl;
         if (!nav.parityGood)
            cout << "Parity error" << endl;
      }

      firstEph=false;

      if (!nav.parityGood)
         return;

      short sfid = nav.getSFID();
      //BWT process all subframes
      //if (sfid > 3) return;

      short week = static_cast<GPSWeekSecond>(nav.time).week;
      long sow = nav.getHOWTime();
      if (sow > FULLWEEK)
      {
         if (debugLevel)
            cout << "Bad week" << endl;
         return;
      }

      //BWT if (debugLevel>1)
         nav.dump(cout);

      //BWT if (!anyNav && (nav.range != rcCA || nav.carrier != ccL1))
         //BWT return;

      NavIndex ni(RangeCarrierPair(nav.range, nav.carrier), nav.prn);
      ephData[ni] = nav;

      ephPageStore[ni][sfid] = nav;
      EngEphemeris engEph;
      if (makeEngEphemeris(engEph, ephPageStore[ni]))
      {
         RinexNavData rnd(engEph);
         rinexNavOutput << rnd;
         ephPageStore[ni].clear();
      }
   } // MDP2Rinex::process(MDPNavSubframe)


   virtual void process(MDPObsEpoch& obs)
   {

      const CommonTime& t=epoch.begin()->second.time;

      if (!firstObs && t<prevTime)
      {
         if (debugLevel)
            cout << "Out of order data at " << t << endl;
         return;
      }

      if (epoch.size() > 0 && t != obs.time)
      {
         if (!thin || (static_cast<int>(static_cast<YDSTime>(t).sod) % thinning) == 0)
         {
            if (firstObs)
            {
               roh.firstObs = t;
               output << roh;
               firstObs=false;
               if (debugLevel)
               	cout << "Got first obs" << endl;
            }

            RinexObsData rod;
            rod = makeRinexObsData(epoch);
            output << rod;
         }
         epoch.clear();
         prevTime = t;
      }

      //cout << "Before correction\n"; obs.dump(cout);
      if(fixHalf) correctHalfCycle(obs);
      //cout << "After correction\n"; obs.dump(cout);

      epoch.insert(pair<const int, MDPObsEpoch>(obs.prn, obs));

   } // MDP2Rinex::process(MDPObsEpoch)

   void correctHalfCycle(MDPObsEpoch& epoch)
   {
      int prn(epoch.prn);
      MDPObsEpoch::ObsMap& obsmap(epoch.obs);
      MDPObsEpoch::ObsMap::iterator jt;
      for(jt = obsmap.begin(); jt != obsmap.end(); ++jt) {
         CarrierCode cc(jt->first.first);
         RangeCode rc(jt->first.second);

         NavIndex ni(RangeCarrierPair(rc, cc), prn);
         if(ephData.find(ni) == ephData.end())        // no nav - can't fix
            continue;

         MDPNavSubframe& nav(ephData[ni]);
         //if(debugLevel)
         //cout << "halfcycle inversion prn " << prn
            //<< " L" << cc << " " << (rc==rcCA ? "CA":"Y")
            //<< " " << (nav.inverted ? "-1":"+1") << endl;
         if(!nav.inverted)                            // not inverted - no fix needed
            continue;

         // fix the phase data
         jt->second.phase += 0.5;
         //if(debugLevel)
         //cout << "correct phase prn " << prn << " L" << cc
         //   << " " << (rc==rcCA ? "CA":"Y") << endl;
         //jt->second.dump(cout); cout << endl;
      }
   }

   virtual void process()
   {
      static MDPHeader header;
      MDPNavSubframe nav;
      MDPObsEpoch obs;

      input >> header;
      
      switch (header.id)
      {
         case MDPNavSubframe::myId :
            input >> nav;
            if (!nav)
            {
               if (input && debugLevel)
                  cout << "Error decoding nav " << endl;
            }
            else
               process(nav);
            break;
            
         case MDPObsEpoch::myId :
            input >> obs;
            if (!obs)
            {
               if (input && debugLevel)
                  cout << "Error decoding obs " << endl;
            }
            else
               process(obs);
            break;
      }
      timeToDie |= !input;
   } // MDP2Rinex::process()

   virtual void shutDown()
   {
      if (debugLevel)
      	cout << "Done" << endl;
   }

private:
   RinexObsHeader roh;
   RinexNavHeader rnh;
   RinexNavStream rinexNavOutput;
   MDPEpoch epoch;

   typedef pair<RangeCode, CarrierCode> RangeCarrierPair;
   typedef pair<RangeCarrierPair, short> NavIndex;
   typedef map<NavIndex, MDPNavSubframe> NavMap;
   NavMap ephData;
   map<NavIndex, EphemerisPages> ephPageStore;
   map<NavIndex, EngEphemeris> ephStore;

   bool thin;
   bool anyNav;
   bool fixHalf;
   int thinning;
   bool firstObs, firstEph;
   CommonTime prevTime;
   Triple antPos;
};


int main(int argc, char *argv[])
{
   MDP2Rinex crap(argv[0]);
   
   if (!crap.initialize(argc, argv))
      exit(0);

   crap.run();
}
