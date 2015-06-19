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

#ifndef SELECTIONSETTING_H
#define	SELECTIONSETTING_H

#include "Settings.h"

#include <vector>

class SelectionSetting : public Settings {
public:
  
                SelectionSetting(UserInterface ui, string name, string description,
                                 string settings_change_prompt, string default_param,
                                 std::vector<std::string> const& allowed_settings);
                SelectionSetting(const SelectionSetting& orig);
  virtual      ~SelectionSetting();

  virtual Settings::PARAM_CHANGE_RETURN checkParam(string const& new_param, bool ui_output);

private:
    void changeParam(string const& new_param);
    std::vector<std::string> const allowed_settings_lc_;
};

#endif	/* SELECTIONSETTING_H */

