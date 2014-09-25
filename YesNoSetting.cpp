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

#include "YesNoSetting.h"
#include "UserInterface.h"

YesNoSetting::YesNoSetting(UserInterface ui, string name, string description, 
                           string settings_change_prompt, string default_param)
: Settings(ui, name, description, settings_change_prompt, default_param)
{
}

YesNoSetting::YesNoSetting(const YesNoSetting& orig)
: Settings(orig)
{
}

YesNoSetting::~YesNoSetting()
{
}

const int YesNoSetting::checkParam(string new_param)
{
  if (new_param == "y")
  {
    settings_param_ = "Yes";
  } else if (new_param == "n")
  {
    settings_param_ = "No";
  } else
  {
    ui_.writeString("Please enter [y] for 'yes' and [n] for 'no'!", true);
    return -1;
  }
  
  return 0;
}

const string YesNoSetting::setParam()
{
  string new_param;
  
  ui_.writeString(settings_change_prompt_, true, "green");
  do
  {
    new_param = ui_.readStringNoCapitalize();
    
  } while (checkParam(new_param) != 0);
  ui_.writeString("Saved.", true, "green");
  ui_.readString(false);
  
  return settings_param_;
}

const int YesNoSetting::writeParam(string new_param)
{
  if(new_param == "Yes" || new_param == "No")
  {
    settings_param_ = new_param;

    return 0;
  }
  
  return -1;
}
