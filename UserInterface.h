/* 
 * File:   UserInterface.h
 * Author: CutePoisonX
 *
 * Created on 04. Februar 2013, 09:38
 */

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
