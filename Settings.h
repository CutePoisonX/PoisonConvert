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
