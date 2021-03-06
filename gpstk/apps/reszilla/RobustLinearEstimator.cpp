#pragma ident "$Id: RobustLinearEstimator.cpp 3140 2012-06-18 15:03:02Z susancummins $"

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

#include <math.h>
#include <algorithm>
#include <limits>

#include "RobustLinearEstimator.hpp"

// A little bit of syntatical sugar...
typedef DoubleDoubleVec::const_iterator DDVci;

using namespace std;


double median(vector<double>& vec)
{
   size_t N=vec.size();
   double m=vec[N/2];
   if (! (N & 1))
      m = 0.5 * (m + vec[N/2-1]);
   return m;
}

double medTail(vector<double>& vec, double f)
{
   size_t N=vec.size();
   size_t M=std::max(static_cast<int>(N*(1-f)), 0);
   double med = median(vec);
   double ml = abs(int(vec[M]-med));
   double mh = abs(int(vec[N-M]-med));
   return std::max(ml, mh);
}


void RobustLinearEstimator::process(
   DoubleDoubleVec::const_iterator b,
   DoubleDoubleVec::const_iterator e)
{
   // Yea, we could probably find a more efficient way to do this but...
   process(DoubleDoubleVec(b,e));
}


void RobustLinearEstimator::process(const DoubleDoubleVec& d)
{
   std::vector<double> y(d.size());
   size_t j=0;

   baseX = d.begin()->first;

   if (debugLevel)
      cout << "processing " << d.size() << " elements" << endl;

   for (DDVci i=d.begin(); i!=d.end(); i++,j++)
      y[j] = i->second;

   sort(y.begin(), y.end());
   stripY=medTail(y, stripPercent);
   medianY=median(y);

   // As a first guess for a and b, find the least-squares fit
   for (DDVci i=d.begin(); i!=d.end(); i++)
   {
      if (std::abs(int(i->second - medianY)) > stripY)
         continue;

      double x = i->first-baseX;
      double y = i->second;
      data.push_back(DoubleDouble(x, y));
      sumX += x;
      sumY += y;
      sumXY += x*y;
      sumXX += x*x;
   }

   if (data.size()<3)
      return;

   double n = static_cast<double>(data.size());
   double del = n*sumXX - sumX*sumX;
   a = (sumXX*sumY - sumX*sumXY) / del;
   b = (n*sumXY - sumX*sumY) / del;

   // Now compute the chi^2 for the LR computed a, b
   double chisq=0;
   for (DDVci i=data.begin(); i!=data.end(); i++)
   {
      double x = i->first;
      double y = i->second;
      double tmp = y - (a + b*x);
      chisq += tmp*tmp;
   }

   // The standard deviation will give some idea of how big an iteration
   // step to take.
   double sig_b = sqrt(chisq/del);
   double b1 = b;

   if (debugLevel>1)
      cout << setprecision(3) << fixed
           << "n=" << (int)n << " sig_b=" << sig_b << " chisq=" << chisq << endl
           << "a="<< a << " b=" << b << endl;

   // If the sigma on b is already zero, then the current values of a & b are
   // perfect. This should never happen with N>3, I think.
   if (sig_b == 0.0)
      return;

   valid = true;
   double f1 = rofunc(b1);
   double b2 = b + (f1>=0 ? 3:-3) * sig_b;
   double f2 = rofunc(b2);

   if (b2 == b1)
      return;

   while (f1*f2 > 0)
   {
      if (debugLevel>1)
         cout << "f1=" << f1 << " f2=" << f2 << endl;
      b  = b2 + 2*(b2-b1);
      b1 = b2;
      f1 = f2;
      b2 = b;
      f2 = rofunc(b2);
   }
   sig_b *= 0.01;

   // Refine until the error is a negligible number of standard deviations
   while (std::abs(int(b2-b1)) > sig_b)
   {
      b = b1 + 0.5 * (b2-b1);
      if (b == b1 || b == b2) 
         break;
      double f = rofunc(b);
      if (f*f1 >= 0)
      {
         f1=f;
         b1=b;
      }
      else
      {
         f2=f;
         b2=b;
      }
   }
}

// Evaluates the right-hand side of equation (15.7.16) of Numerical Recipes 
// for a given value of b.
// Note that this function updates a, and abdev
double RobustLinearEstimator::rofunc(const double b_est)
{
   vector<double> abxVec(data.size());

   size_t j=0;
   for (DDVci i=data.begin(); i!=data.end(); i++)
   {
      double x = i->first;
      double y = i->second;
      abxVec[j++] = y - b_est * x;
   }

   sort(abxVec.begin(), abxVec.end());
   a = median(abxVec);

   abdev=0;
   double sum=0;
   for (DDVci i=data.begin(); i != data.end(); i++)
   {
      double x=i->first;
      double y=i->second;
      double d = y - (b_est * x + a);
      abdev += std::abs(int(d));
      sum += d >= 0 ? x : -x;
   }

   abdev /= data.size();

   if (debugLevel>1)
      cout <<"a="<< a << " b=" << b << " b_est=" << b_est 
           << " f=" << sum << " abdev=" << abdev << endl;

   return sum;
}
