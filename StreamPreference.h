/* 
 * File:   StreamPreference.h
 * Author: CutePoisonX
 *
 * Created on 04. Februar 2013, 09:40
 */

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

