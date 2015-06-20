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
  virtual         ~Settings();
  
  enum PARAM_CHANGE_RETURN
	{
        PARAM_CHANGE_SUCCESS,
		PARAM_CHANGE_ERROR,
		EXIT_SETTING
	};

  string const getName()
  {
    return settings_name_;
  }
  
  string const getParam()
  {
    return settings_param_;
  }
    
  string const getDefaultParam()
  {
    return settings_default_param_;
  }

  string const getDescription()
  {
    return settings_description_;
  }
  
  void resetSetting()
  {
    setParamSilent(settings_default_param_);
  }
    
  virtual string const setParam();
  virtual PARAM_CHANGE_RETURN setParamSilent(string const& new_param);
  virtual PARAM_CHANGE_RETURN checkParam(string const& new_param, bool ui_output) = 0;
  PARAM_CHANGE_RETURN checkParam(bool ui_output)
  {
    return checkParam(settings_param_, ui_output);
  }
  
protected:
  
  virtual void attemptToChangeParam(string new_param);
          void changeParam(string const& new_param);
  
  UserInterface&    ui_;
  
  const string      settings_name_;
  const string      settings_description_;
  const string      settings_change_prompt_;
  const string      settings_default_param_;
    
private:
        string      settings_param_;
  
};

#endif	/* SETTINGS_H */
