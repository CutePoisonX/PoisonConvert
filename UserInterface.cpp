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

#include <sstream>
#include <iostream>
#include <algorithm>
#include "UserInterface.h"

UserInterface::UserInterface()
{

}

UserInterface::~UserInterface() throw()
{
}

std::string UserInterface::readString(bool show_arrows)
{
  if (show_arrows == true)
  {
    std::cout << ">> ";
  }
  std::string input;
  std::getline(std::cin, input);
  return input;
}

std::string UserInterface::readStringNoCapitalize(bool show_arrows)
{
  if (show_arrows == true)
  {
    std::cout << ">> ";
  }
  std::string input;
  std::getline(std::cin, input);
  std::transform(input.begin(), input.end(), input.begin(), ::tolower);
  return input;
}

bool UserInterface::stringToUnsignedInt(std::string& string_number,
                                        unsigned int& value)
{
  std::istringstream stream(string_number);
  stream >> value;
  if(stream.fail() || ! stream.eof())
  {
    return false;
  }
  return true;
}

void UserInterface::writeNumber(unsigned int output, bool new_line,
                                std::string color)
{
  if (color == "blue")
    std::cout << "\033[34;1m";
  else if (color == "yellow")
    std::cout << "\033[33;1m";
  else if (color == "green")
    std::cout << "\033[32;1m";
  else if (color == "red")
    std::cout << "\033[31;1m";
  else if (color == "white")
    std::cout << "\e[0m";
  else
    std::cout << "\e[0m";
  
  std::cout << output << "\e[0m";
  if(new_line == true)
    std::cout << std::endl;

}

void UserInterface::writeString(std::string output, bool new_line,
                                std::string color) const
{
  if (color == "blue")
    std::cout << "\033[34;1m";
  else if (color == "yellow")
    std::cout << "\033[33;1m";
  else if (color == "green")
    std::cout << "\033[32;1m";
  else if (color == "red")
    std::cout << "\033[31;1m";
  else if (color == "white")
    std::cout << "\e[0m";
  else
    std::cout << "\e[0m";

  std::cout << output << "\e[0m";
  if (new_line == true)
    std::cout << std::endl;
}

UserInterface& UserInterface::operator=(const UserInterface& original_ui)
{
  return *this;
}
