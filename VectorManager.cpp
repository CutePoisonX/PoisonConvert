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
