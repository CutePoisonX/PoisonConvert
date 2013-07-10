/* 
 * File:   Settings.h
 * Author: CutePoisonX
 *
 * Created on 16. Juni 2013, 21:48
 */

#ifndef SETTINGS_H
#define	SETTINGS_H

#include <string>

class UserInterface;
using namespace std;

class Settings {
public:
                  Settings(UserInterface ui, string name, string description, 
                           string settings_change_prompt, string default_param);
                  Settings(const Settings& orig);
    virtual      ~Settings();
  
  const string getName() 
  {
    return settings_name_;
  }
  
  const string getParam() 
  {
    return settings_param_;
  }

  const string getDescription() 
  {
    return settings_description_;
  }
  
  virtual const string setParam();
  virtual const int writeParam(string new_param);
  
protected:
  
  virtual const int checkParam(string new_param);
  
  UserInterface&    ui_;
  
  const string      settings_name_;
  const string      settings_description_;
  const string      settings_change_prompt_;
        string      settings_param_;
  
};

#endif	/* SETTINGS_H */
