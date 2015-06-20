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

#include "SelectionSetting.h"
#include "UserInterface.h"

#include <algorithm>

SelectionSetting::SelectionSetting(UserInterface ui, string name, string description,
                                   string settings_change_prompt, string default_param,
                                   std::vector<std::string> const& allowed_settings)
: Settings(ui, name, description, settings_change_prompt, default_param),
  allowed_settings_lc_(allowed_settings)
{
}

SelectionSetting::SelectionSetting(const SelectionSetting& orig)
: Settings(orig)
{
}

SelectionSetting::~SelectionSetting()
{
}

void SelectionSetting::attemptToChangeParam(string new_param)
{
  new_param[0] = toupper(new_param[0]);
  std::transform(new_param.begin() + 1, new_param.end(), new_param.begin() + 1,
                 ::tolower);
    
  changeParam(new_param);
}

Settings::PARAM_CHANGE_RETURN SelectionSetting::checkParam(string const& new_param, bool ui_output)
{
  std::string new_param_lowercase = new_param;
  std::transform(new_param_lowercase.begin(), new_param_lowercase.end(), new_param_lowercase.begin(),
                                                   ::tolower);
    
  for (std::vector<std::string>::const_iterator allowed_value = allowed_settings_lc_.begin();
       allowed_value != allowed_settings_lc_.end(); ++allowed_value)
  {
    if (new_param_lowercase == *allowed_value)
    {
      return PARAM_CHANGE_SUCCESS;
    }
  }

  if (ui_output)
  {
    ui_.writeString("Warning: possible values:", true, "yellow");
    for (unsigned int allowed_value_num = 0; allowed_value_num < allowed_settings_lc_.size() - 1;
         ++allowed_value_num)
    {
      ui_.writeString(allowed_settings_lc_[allowed_value_num] + ",", true);
    }
    ui_.writeString(allowed_settings_lc_[allowed_settings_lc_.size() - 1], true);
    ui_.writeString("", true);
  }
  return PARAM_CHANGE_ERROR;
}
