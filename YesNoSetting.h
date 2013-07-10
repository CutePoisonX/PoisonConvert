/* 
 * File:   YesNoSetting.h
 * Author: CutePoisonX
 *
 * Created on 16. Juni 2013, 22:12
 */

#ifndef YESNOSETTING_H
#define	YESNOSETTING_H

#include "Settings.h"

class YesNoSetting : public Settings {
public:
  
                YesNoSetting(UserInterface ui, string name, string description, 
                           string settings_change_prompt, string default_param);
                YesNoSetting(const YesNoSetting& orig);
  virtual      ~YesNoSetting();
  
  const string setParam();
  
  public:
  
    const int checkParam(string new_param);
    const int writeParam(string new_param);
};

#endif	/* YESNOSETTING_H */

