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

#include "FileSetting.h"

#include "UserInterface.h"

FileSetting::FileSetting(UserInterface ui, string name, string description,
												 string settings_change_prompt, string default_param,
												 string error_promp_str)
: Settings(ui, name, description, settings_change_prompt, default_param),
  error_promp_str_(error_promp_str)
{
}

FileSetting::FileSetting(const Settings& orig)
: Settings(orig)
{
}

FileSetting::~FileSetting()
{
}

FileSetting::PARAM_CHANGE_RETURN FileSetting::checkParam(string const& new_param, bool ui_output)
//note: this method does not check whether a file actually exists, but
//only if the string provided by "new_param" is non-empty!
{
  if(new_param.empty())
  {
    if (ui_output)
    {
      ui_.writeString(error_promp_str_, true);
    }
    return PARAM_CHANGE_ERROR;
  }

  return PARAM_CHANGE_SUCCESS;
}
