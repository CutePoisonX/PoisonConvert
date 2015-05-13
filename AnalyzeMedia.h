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

#ifndef ANALYZEMEDIA_H
#define	ANALYZEMEDIA_H

#include "VectorManager.h"

class AnalyzeMedia : public VectorManager {
public:
                AnalyzeMedia();
                AnalyzeMedia(const AnalyzeMedia& orig);
  virtual      ~AnalyzeMedia();
  
  int           saveToVector(string& param1, string& param2, string& param3,
                             string& param4, string& param5, unsigned int priority,
                             unsigned int identifier);
  void          clearEverything();
  void          clearAllInstances();
	unsigned int  getVectorLength(unsigned int identifier);
  
private:
  
  vector<StreamPreference*>& getVector(unsigned int identifier);
};

#endif	/* ANALYZEMEDIA_H */

