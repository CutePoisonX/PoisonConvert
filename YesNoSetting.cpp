//
//    Copyright 2015 CutePoisonX (CutePoisonXI@gmail.com)
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
: SelectionSetting(ui, name, description, settings_change_prompt, default_param,
                   generateAllowedSettingsVector())
{
}

YesNoSetting::YesNoSetting(const YesNoSetting& orig)
: SelectionSetting(orig)
{
}

YesNoSetting::~YesNoSetting()
{
}

std::vector<std::string> YesNoSetting::generateAllowedSettingsVector() const
{
  std::vector<std::string> allowed_settings;

  allowed_settings.push_back("yes");
  allowed_settings.push_back("no");
    
  return allowed_settings;
}
