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

#include "ExecutableFileSetting.h"

#include "UserInterface.h"

#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

ExecutableFileSetting::ExecutableFileSetting(UserInterface ui, string name, string description,
                                             string settings_change_prompt, string default_param,
                                             bool allow_for_empty_string)
: FileSetting(ui, name, description, settings_change_prompt, default_param, "Executable does not exist or I can't execute it"),
  allow_for_empty_string_(allow_for_empty_string)
{
}

ExecutableFileSetting::~ExecutableFileSetting()
{
}

ExecutableFileSetting::PARAM_CHANGE_RETURN ExecutableFileSetting::checkParam(string const& new_param, bool ui_output)
{
  struct stat file_check;
    
  if (new_param.empty())
  {
    if (allow_for_empty_string_)
    {
      return PARAM_CHANGE_SUCCESS;
    }
    else
    {
      if (ui_output)
      {
        ui_.writeString("Empty string is not valid.", true);
      }
      return PARAM_CHANGE_ERROR;
    }
  }
  
  if ((stat(new_param.c_str(), &file_check) != 0) || (!file_check.st_mode & S_IXUSR))
  {
    std::string const delimiter = ":";
    std::string const path = std::string(getenv("PATH"));
    size_t start_pos = 0;
    size_t end_pos = 0;

    while ((end_pos = path.find(':', start_pos)) != std::string::npos)
    {
      std::string current_path = path.substr(start_pos, end_pos - start_pos) + "/" + new_param;

      if ((stat(current_path.c_str(), &file_check) == 0) && (file_check.st_mode & S_IXOTH))
      {
        return PARAM_CHANGE_SUCCESS;
      }

      start_pos = end_pos + 1;
    }

    if (ui_output)
    {
      ui_.writeString("Executable does not exist or I can't execute it", true);
    }
    return PARAM_CHANGE_ERROR;
  }
  else
  {
    return PARAM_CHANGE_SUCCESS;
  }
    
}
