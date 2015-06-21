//
//    Copyright 2015 CutePoisonX (CutePoisonXI@gmail.com)
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

#ifndef VECTORMANAGER_H
#define	VECTORMANAGER_H

#include <vector>
#include "StreamPreference.h"

enum {
       SRCVIDEO = 0, 
       SRCAUDIO = 1, 
       SRCSUB   = 2, 
       TARVIDEO = 3, 
       TARAUDIO = 4,
       TARSUB   = 5
     };

using namespace std;

class VectorManager {
public:
  
                VectorManager();
                VectorManager(const VectorManager& orig);
  virtual      ~VectorManager();

  virtual int   saveToVector(string& param1, string& param2, string& param3,
                             string& param4, string& param5,
                             unsigned int priority, unsigned int identifier) = 0;
  string        sendSinglePreference(unsigned int priority, unsigned int identifier,
                                     unsigned int param_nr);
  unsigned int  getVectorLen(unsigned int identifier);
  virtual void  clearAllInstances() = 0;
  
protected:

  //----------------------------------------------------------------------------
  vector<StreamPreference*> source_audio_vect_;
  vector<StreamPreference*> source_sub_vect_;
  vector<StreamPreference*> source_video_vect_;
  //----------------------------------------------------------------------------
  
  virtual vector<StreamPreference*>&  getVector(unsigned int identifier) = 0;
  vector<StreamPreference*>::iterator getSourceIterator(unsigned int pos,
                                                        vector<StreamPreference*>& tmp);
};

#endif	/* VECTORMANAGER_H */

