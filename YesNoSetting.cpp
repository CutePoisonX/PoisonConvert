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

YesNoSetting::PARAM_CHANGE_RETURN YesNoSetting::checkParam(string const& new_param, bool ui_output)
{
  if (new_param != "y" && new_param != "n" && new_param != "Yes" && new_param != "No" &&
      new_param != "yes" && new_param != "no")
  {
    if (ui_output)
    {
      ui_.writeString("Please enter [y] for 'yes' and [n] for 'no'!", true);
    }

    return PARAM_CHANGE_ERROR;
  }
  
  return PARAM_CHANGE_SUCCESS;
}

void YesNoSetting::changeParam(string const& new_param)
//We can assume that new_param is either y or n since checkParam was called before ...
{
  std::string actual_param;
    
  if (new_param == "y" || new_param == "yes")
  {
    actual_param = "Yes";
  }
  else if (new_param == "n" || new_param == "no")
  {
    actual_param = "No";
  }
  else
  {
    actual_param = new_param;
  }
    
  settings_param_ = actual_param;
}
