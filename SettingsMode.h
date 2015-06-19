
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
     
using namespace std;
class UserInterface;

class SettingsMode : public Mode {
public:
    //order of variables in enum must be exactly as order of vector element-settings in settings_vector_
    enum SETTING_SPECIFIER {
          CONFIGNAME = 0,
          CONFIGLOC,
          FFMPEG_CMD,
          FFPROBE_CMD,
          QT_CMD,
          DELETESET,
          OPTIMIZESET,
          LOGPATH,
          LOGGING,
          MOVIEPATH,
          DESTINATION
         };
    
    enum SAVE_SETTINGS {
        DONT_SAVE_SETTINGS = -1,
        SAVE_SETTINGS      = -2
    };
  
                SettingsMode(UserInterface& ui);
                SettingsMode(const SettingsMode& orig);
  virtual      ~SettingsMode();
  
  int           executeCommand();

  const string getSettingsParam(unsigned int setting_nr) const
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
  
  const Settings::PARAM_CHANGE_RETURN writeParam(string new_setting, SETTING_SPECIFIER setting_nr, bool manually_changed = true)
  {
    Settings::PARAM_CHANGE_RETURN changes_success = settings_vector_.at(setting_nr)->setParamSilent(new_setting);
    
    if (manually_changed && changes_success == Settings::PARAM_CHANGE_SUCCESS)
    {
      there_were_changes_to_settings_ = true;
    }
    
    return changes_success;
  }
    
  const Settings::PARAM_CHANGE_RETURN writeParam(string new_setting, std::string setting_name, bool manually_changed = true)
  {
    for (unsigned int setting_num = 0; setting_num < settings_vector_.size(); ++ setting_num)
    {
      SETTING_SPECIFIER setting_specifier = static_cast<SETTING_SPECIFIER>(setting_num);
      if (setting_name == settings_vector_[setting_specifier]->getName())
      {
        return writeParam(new_setting, setting_specifier, manually_changed);
      }
    }

    return Settings::PARAM_CHANGE_ERROR;
  }
    
  const Settings::PARAM_CHANGE_RETURN checkParam(SETTING_SPECIFIER setting_nr, bool ui_output) const
  {
    return settings_vector_.at(setting_nr)->checkParam(ui_output);
  }
  
private:
  
  //----------------------------------------------------------------------------
  vector<Settings*> settings_vector_;
  bool there_were_changes_to_settings_;
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

