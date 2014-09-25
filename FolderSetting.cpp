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
#include <string.h>

FolderSetting::FolderSetting(UserInterface ui, string name, string description, 
                           string settings_change_prompt, string default_param)
: Settings(ui, name, description, settings_change_prompt, default_param)
{
}

FolderSetting::FolderSetting(const FolderSetting& orig)
: Settings(orig)
{
}

FolderSetting::~FolderSetting()
{
}

const int FolderSetting::checkParam(string new_param)
{
  size_t position = new_param.find_last_of("/");
  struct stat folder_check;
  
  if (stat(new_param.c_str(), &folder_check) == 0 && S_ISDIR(folder_check.st_mode))
  {
    if( position != new_param.length() -1)
    {
      new_param.append("/");
    }
    settings_param_ = new_param;
  } else if (new_param == "exit")
  {
    return -2;
  } else
  {
    ui_.writeString("Please enter a valid path or 'exit' to abort!", true);
    return -1;
  }

  return 0;
}

const string FolderSetting::setParam()
{
  string new_param;
  int errorcase = 0;
  ui_.writeString(settings_change_prompt_, true, "green");
  do
  {
    new_param = ui_.readString();
    errorcase = checkParam(new_param);
  } while (errorcase != 0 && errorcase != -2);
  if(errorcase == 0)
  {
    ui_.writeString("Saved.", true, "green");
    ui_.readString(false);
  }
  
  return settings_param_;
}

const int FolderSetting::writeParam(string new_param)
{
  string check_param_first = "[ -d \"/";
  string check_param_sec = "\" ]";
  
  check_param_first.append(new_param);
  check_param_first.append(check_param_sec);
  
  if (system(check_param_first.c_str()) == -1)
  {
    return -1;
  } else
  {
    settings_param_ = new_param;
  }
  
  return 0;
}
