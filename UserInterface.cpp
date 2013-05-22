/* 
 * File:   StreamPreference.cpp
 * Author: CutePoisonX
 * 
 * Created on 04. Februar 2013, 10:41
 */
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
