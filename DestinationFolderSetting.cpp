/*
 * DestinationFolderSetting.cpp
 *
 *  Created on: 02.06.2015
 *      Author: Christoph
 */

#include "DestinationFolderSetting.h"
#include "UserInterface.h"

DestinationFolderSetting::DestinationFolderSetting(UserInterface const& ui)
: FolderSetting(ui, "destination", "Where to save processed movies",
                "Please enter the path where poisonconvert should save processed movies "
                "(if you leave the optoin empty, processed movies stay in their source directory).",
                "Destination equals source")
{
}

DestinationFolderSetting::~DestinationFolderSetting()
{
}

DestinationFolderSetting::PARAM_CHANGE_RETURN DestinationFolderSetting::checkParam(string const& new_param,
                                                                                           bool ui_output)
{
  if (new_param.empty() || "Destination equals source")
  {
    if (ui_output)
    {
      ui_.writeString("The video will not be moved after processing has finished.", true);
    }
    return PARAM_CHANGE_SUCCESS;
  }
  else
  {
    return FolderSetting::checkParam(new_param, ui_output);
  }
}

void DestinationFolderSetting::attemptToChangeParam(string new_param)
//We can assume that new_param is either y or n since checkParam was called before ...
{
  if (new_param.empty())
  {
    new_param = "Destination equals source";
  }
  else
  {
    size_t position = new_param.find_last_of("/");
      
    if (position != new_param.length() - 1)
    {
      new_param.append("/");
    }
  }
    
  changeParam(new_param);
}
