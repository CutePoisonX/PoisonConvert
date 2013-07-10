/* 
 * File:   SettingsMode.h
 * Author: CutePoisonX
 *
 * Created on 08. Februar 2013, 09:40
 */

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

