#pragma ident "$Id: fic2rin.cpp 3145 2012-06-19 19:32:25Z snelsen $"

/**
 * @file fic2rin.cpp Convert FIC files to RINEX.
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

#include "FICStream.hpp"
#include "FICHeader.hpp"
#include "FICData.hpp"
#include "RinexNavStream.hpp"
#include "RinexNavData.hpp"
#include "FICFilterOperators.hpp"
#include "RinexNavFilterOperators.hpp"
#include "FileFilterFrame.hpp"
#include "GPSWeekSecond.hpp"
#include "SystemTime.hpp"
#include "CivilTime.hpp"
#include <string>

using namespace std;
using namespace gpstk;

int main(int argc, char* argv[])
{
   try
   {
      if ((argc != 3) && (argc != 4))
      {
         cout << "fic2rin" << endl
              << "  converts a binary FIC file to a Rinex Nav file" << endl
              << endl
              << "usage:" << endl
              << "    fic2rin inputfile outputfile [param]" << endl
              << endl
              << "where:" << endl
              << "    inputfile: an input binary FIC file name" << endl
              << "    outputfile: an output Rinex Nav file name" << endl
              << "    param (optional): --9 (default) or --109 or --all," << endl
              << "                      which FIC block numbers to use" << endl;
         return 0;
      }

      // What is up
      FileFilterFrame<FICStream, FICData> input(argv[1]);
      list<FICData> alist = input.getData();
   
         // write the header info
      RinexNavStream rns(argv[2], ios::out|ios::trunc);
      RinexNavHeader rnh;
      rnh.fileType = "Navigation";
      rnh.fileProgram = "fic2rin";
      rnh.fileAgency = "";
      ostringstream ostr;
      ostr << CivilTime(SystemTime());
      rnh.date = ostr.str();
      rnh.version = 2.1;
      rnh.valid |= RinexNavHeader::versionValid;
      rnh.valid |= RinexNavHeader::runByValid;
      rnh.valid |= RinexNavHeader::endValid;
      rns.header = rnh;
      rnh.putRecord(rns);
      rns.close();

         // filter the FIC data for block 9
      list<long> blockList;
      if (argc == 4)
      {
        string parg(argv[3]);
        if (parg == "--109")
        {
          blockList.push_back(109); //block 109
          input.filter(FICDataFilterBlock(blockList));
          input.sort(FICDataOperatorLessThanBlock109());
          input.unique(FICDataUniqueBlock109());
        }
        else if (parg == "--all") //block 9 and block 109
        {
          blockList.push_back(9);
          blockList.push_back(109);
          input.filter(FICDataFilterBlock(blockList));
        }
        else
        {
          blockList.push_back(9); //default to block 9
          input.filter(FICDataFilterBlock(blockList));
          input.sort(FICDataOperatorLessThanBlock9());
          input.unique(FICDataUniqueBlock9());
        }
      }
      else
      {
        blockList.push_back(9); //default to block 9
        input.filter(FICDataFilterBlock(blockList));
        input.sort(FICDataOperatorLessThanBlock9());
        input.unique(FICDataUniqueBlock9());
      }
         // some hand waving for the data conversion
      list<RinexNavData> rndList;
      list<FICData>& ficList = input.getData();
      list<FICData>::iterator itr = ficList.begin();
      while (itr != ficList.end())
      {
            // use TOE and transmit week number to determine time
         CommonTime time;
         if( (*itr).blockNum == 9 ) // block 9
            time=GPSWeekSecond(short((*itr).f[5]), (double)(*itr).f[33]);
         else // block 109
            time=GPSWeekSecond(short((*itr).i[0]), (double)(((*itr).i[21] & 0x3FFFFFFFL)>>14)*16);
            // this station number is bogus, but it's unused so it should be ok
         EngEphemeris ee(*itr);
         rndList.push_back(RinexNavData(ee));
         itr++;
      }
   
         // write the file data
      FileFilterFrame<RinexNavStream, RinexNavData> output;
      output.addData(rndList);
      output.sort(RinexNavDataOperatorLessThanFull());
      output.writeFile(argv[2], true);

   }
   catch (Exception& exc)
   {
      cerr << exc;
      return 1;
   }
   catch (...)
   {
      cerr << "Caught unknown exception" << endl;
      return 1;
   }
   return 0;
}
