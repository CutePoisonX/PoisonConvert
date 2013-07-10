/* 
 * File:   Settings.cpp
 * Author: CutePoisonX
 * 
 * Created on 16. Juni 2013, 21:48
 */

#include "Settings.h"
#include "UserInterface.h"

Settings::Settings(UserInterface ui, string name, string description, 
                   string settings_change_prompt, string default_param)
: ui_(ui), settings_name_(name), settings_description_(description), 
  settings_change_prompt_(settings_change_prompt), settings_param_(default_param)
{
}

Settings::Settings(const Settings& orig)
: ui_(orig.ui_), settings_name_(orig.settings_name_), settings_description_(orig.settings_description_),
  settings_change_prompt_(orig.settings_change_prompt_), settings_param_(orig.settings_param_)
{
}

Settings::~Settings()
{
}

const string Settings::setParam()
{
  string new_param;
  
  ui_.writeString(settings_change_prompt_, true, "green");
  do
  {
    new_param = ui_.readString();
    
  } while (checkParam(new_param) != 0);
  
  ui_.writeString("Saved.", true, "green");
  ui_.readString(false);
  
  return settings_param_;
}

const int Settings::checkParam(string new_param)
{
  if(new_param != "")
  {
    settings_param_ = new_param;
  } else
  {
    ui_.writeString("Please enter the name of the config-file", true);
    return -1;
  }
  return 0;
}

const int Settings::writeParam(string new_param)
{
  if (new_param != "")
  {
    settings_param_ = new_param;

    return 0;
  }

  return -1;
}
