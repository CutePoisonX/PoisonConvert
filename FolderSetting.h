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

#ifndef FOLDERSETTING_H
#define	FOLDERSETTING_H

#include "Settings.h"

class FolderSetting : public Settings {
public:
                FolderSetting(UserInterface ui, string name, string description,
                              string settings_change_prompt, string default_param,
                              bool can_be_empty = false);
                FolderSetting(const FolderSetting& orig);
  virtual      ~FolderSetting();
  
  virtual PARAM_CHANGE_RETURN checkParam(string const& new_param, bool ui_output);
    
protected:
    virtual void changeParam(string const& new_param);
    
private:
    bool can_be_empty_;
};

#endif	/* FOLDERSETTING_H */

