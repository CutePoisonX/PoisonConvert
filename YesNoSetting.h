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

#ifndef YESNOSETTING_H
#define	YESNOSETTING_H

#include "SelectionSetting.h"

class YesNoSetting : public SelectionSetting {
public:
  
                YesNoSetting(UserInterface ui, string name, string description, 
                           string settings_change_prompt, string default_param);
                YesNoSetting(const YesNoSetting& orig);
  virtual      ~YesNoSetting();
    
private:
    std::vector<std::string> generateAllowedSettingsVector() const;
};

#endif	/* YESNOSETTING_H */

