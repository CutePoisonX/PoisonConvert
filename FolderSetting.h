/* 
 * File:   FolderSetting.h
 * Author: CutePoisonX
 *
 * Created on 18. Juni 2013, 21:01
 */

#ifndef FOLDERSETTING_H
#define	FOLDERSETTING_H

#include "Settings.h"

class FolderSetting : public Settings {
public:
  
                FolderSetting(UserInterface ui, string name, string description, 
                           string settings_change_prompt, string default_param);
                FolderSetting(const FolderSetting& orig);
  virtual      ~FolderSetting();
  
  const string setParam();
  
  public:
  
    const int checkParam(string new_param);
    const int writeParam(string new_param);
};

#endif	/* FOLDERSETTING_H */

