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

#ifndef SETTINGSMODE_H
#define	SETTINGSMODE_H

#include "Mode.h"
#include "Settings.h"
#include <string>
#include <vector>

//order of variables in enum must be exactly as order of vector element-settings in settings_vector_
enum { 
       CONFIGNAME,
       CONFIGLOC,
       DELETESET, 
       OPTIMIZESET,
       LOGPATH,
       MOVIEPATH,
       DESTINATION
     };
     
using namespace std;
class UserInterface;

class SettingsMode : public Mode {
public:
  
                SettingsMode(UserInterface& ui);
                SettingsMode(const SettingsMode& orig);
  virtual      ~SettingsMode();
  
  int           executeCommand();

  const string getSettingsParam(unsigned int setting_nr)
  {
    if(setting_nr >= 0)
    {
        return settings_vector_.at(setting_nr)->getParam();
    } else
    {
      return "ERROR - INDEX OUT OF BOUNCE";
    }
  }

  const string getSettingsName(unsigned int setting_nr)
  {
    if(setting_nr >= 0)
    {
        return settings_vector_.at(setting_nr)->getName();
    } else
    {
      return "ERROR - INDEX OUT OF BOUNCE";
    }
  }
  
  const unsigned int getVectorLen()
  {
    return settings_vector_.size();
  }
  
  const int writeParam(string new_setting, unsigned int setting_nr)
  {
    return settings_vector_.at(setting_nr)->writeParam(new_setting);
  }
  
private:
  
  //----------------------------------------------------------------------------
  vector<Settings*> settings_vector_;
  //----------------------------------------------------------------------------
  
  void          standartExecutePrompt();
  void          changeDeleteOriginal(); //Second setting
  void          changeOptimizeForStreaming(); //Third setting
  int           changeFilename(); //first_setting
  int           listSettings();
  void          AlignSpaces(unsigned int align_len, unsigned int vec_element);
  unsigned int  IdentifyLongestName();
};

#endif	/* SETTINGSMODE_H */

