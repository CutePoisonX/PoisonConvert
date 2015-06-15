/*
 * DestinationFolderSetting.h
 *
 *  Created on: 02.06.2015
 *      Author: Christoph
 */

#ifndef DESTINATIONFOLDERSETTING_H_
#define DESTINATIONFOLDERSETTING_H_

#include "FolderSetting.h"

class DestinationFolderSetting: public FolderSetting {
  public:
    DestinationFolderSetting(UserInterface const& ui);
    virtual ~DestinationFolderSetting();

  private:
    virtual PARAM_CHANGE_RETURN checkParam(string const& new_param, bool ui_output);
    void changeParam(string const& new_param);
};

#endif /* DESTINATIONFOLDERSETTING_H_ */
