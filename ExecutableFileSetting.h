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

#ifndef EXECUTABLEFILESETTING_H
#define	EXECUTABLEFILESETTING_H

#include "FileSetting.h"
#include <string>

class UserInterface;
using namespace std;

class ExecutableFileSetting : public FileSetting {
public:
                  ExecutableFileSetting(UserInterface ui, string name, string description,
										string settings_change_prompt, string default_param,
										bool allow_for_empty_string);
                  ExecutableFileSetting(const Settings& orig);
    virtual      ~ExecutableFileSetting();

    virtual PARAM_CHANGE_RETURN checkParam(string const& new_param, bool ui_output);
    
private:
    bool allow_for_empty_string_;
};

#endif	/* EXECUTABLEFILESETTING_H */
