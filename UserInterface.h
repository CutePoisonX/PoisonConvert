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

#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <string>

class UserInterface
{
  public:

                    UserInterface();
    virtual        ~UserInterface() throw();
   
    std::string     readString(bool show_arrows = true);
    std::string     readStringNoCapitalize(bool show_arrows = true);
    
    bool            stringToUnsignedInt(std::string& string_number, 
                                        unsigned int& value);
    
    void            writeNumber(unsigned int output, bool new_line = false,
                                std::string color = "white"); 
    void            writeString(std::string output, bool new_line = false,
                                std::string color = "white") const ;
    
    UserInterface& operator=(const UserInterface& original_ui);
};
#endif // USERINTERFACE_H
