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

#include "FolderSetting.h"
#include "UserInterface.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <cstdlib>
#include <dirent.h>

FolderSetting::FolderSetting(UserInterface ui, string name, string description, 
                             string settings_change_prompt, string default_param,
                             bool can_be_empty)
: Settings(ui, name, description, settings_change_prompt, default_param), can_be_empty_(can_be_empty)
{
}

FolderSetting::FolderSetting(const FolderSetting& orig)
: Settings(orig)
{
}

FolderSetting::~FolderSetting()
{
}

FolderSetting::PARAM_CHANGE_RETURN FolderSetting::checkParam(string const& new_param, bool ui_output)
{
  struct stat folder_check;
  if (((can_be_empty_ && new_param.empty())) == false &&
      (stat(new_param.c_str(), &folder_check) != 0 || S_ISDIR(folder_check.st_mode) == false))
  {
    if (ui_output)
    {
      ui_.writeString("Please enter a valid path or 'exit' to abort!", true);
    }
    return PARAM_CHANGE_ERROR;
  }

  return PARAM_CHANGE_SUCCESS;
}

void FolderSetting::attemptToChangeParam(string new_param)
{
  string new_writable_param = new_param;
  size_t position = new_writable_param.find_last_of("/");

  if (position != new_writable_param.length() - 1)
  {
    new_writable_param.append("/");
  }

  changeParam(new_writable_param);
}
