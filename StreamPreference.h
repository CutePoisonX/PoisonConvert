//
//    Copyright 2014 CutePoisonX (CutePoisonXI@gmail.com)
//
//    This file is part of PoisonConvert.
//
//    PoisonConvert is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    PoisonConvert is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with PoisonConvert.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef STREAMPREFERENCE_H
#define	STREAMPREFERENCE_H

#include <string>
#include <vector>

static const unsigned int ALLPARAMS = 5;
using namespace std;

class StreamPreference {
public:

                StreamPreference(string param1, string param2, string param3,
                                 string param4, string param5);
                StreamPreference(const StreamPreference& orig);
  virtual      ~StreamPreference();

  int           saveTarget(unsigned int target);
  int           editTarget(unsigned int new_target,
                           unsigned int position);
  int           deleteAllTargets(unsigned int target);
  
  //----------------------------------------------------------------------------
  void SetParam5(string& param5_)
  {
    this->param5_ = param5_;
  }

  string GetParam5() const
  {
    return param5_;
  }

  void SetParam4(string& param4_)
  {
    this->param4_ = param4_;
  }

  string GetParam4() const
  {
    return param4_;
  }

  void SetParam3(string& param3_)
  {
    this->param3_ = param3_;
  }

  string GetParam3() const
  {
    return param3_;
  }

  void SetParam2(string& param2_)
  {
    this->param2_ = param2_;
  }

  string GetParam2() const
  {
    return param2_;
  }

  void SetParam1(string& param1_)
  {
    this->param1_ = param1_;
  }

  string GetParam1() const
  {
    return param1_;
  }

  unsigned int GetTargetID(unsigned int item) const
  {
    return link_target_.at(item);
  }
  
  unsigned int GetTargetVecLen() const
  {
    return link_target_.size();
  }
  //----------------------------------------------------------------------------
  
private:
  
  //----------------------------------------------------------------------------
  string param1_;
  string param2_;
  string param3_;
  string param4_;
  string param5_;
  
  vector<unsigned int>  link_target_;
  //----------------------------------------------------------------------------
};

#endif	/* STREAMPREFERENCE_H */

