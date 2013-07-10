/* 
 * File:   VectorManager.cpp
 * Author: CutePoisonX
 * 
 * Created on 10. Februar 2013, 10:31
 */

#include "VectorManager.h"
#include <stdexcept> 

VectorManager::VectorManager()
{
}

VectorManager::VectorManager(const VectorManager& orig)
{
}

VectorManager::~VectorManager()
{
}

vector<StreamPreference*>::iterator VectorManager::getSourceIterator(unsigned int pos,
                                                                     vector<StreamPreference*>& tmp)
{
  vector<StreamPreference*>::iterator it = tmp.begin();
  for (unsigned int count = 0; count < pos; count++)
  {
    it++;
  }
  return it;
}

string VectorManager::sendSinglePreference(unsigned int priority,
                                           unsigned int identifier,
                                           unsigned int param_nr)
{
  priority--;
  switch (param_nr)
  {
    case 1:
      return getVector(identifier).at(priority)->GetParam1();
      break;
    case 2:
      return getVector(identifier).at(priority)->GetParam2();
      break;
    case 3:
      return getVector(identifier).at(priority)->GetParam3();
      break;
    case 4:
      return getVector(identifier).at(priority)->GetParam4();
      break;
    case 5:
      return getVector(identifier).at(priority)->GetParam5();
      break;
  }
}

unsigned int VectorManager::getVectorLen(unsigned int identifier)
{
  return getVector(identifier).size();
}