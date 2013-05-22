/* 
 * File:   FileSettingsException.cpp
 * Author: CutePoisonX
 * 
 * Created on 08. Februar 2013, 10:46
 */

#include "FileReadException.h"

FileReadException::FileReadException()
{
}

FileReadException::FileReadException(const FileReadException& orig)
{
}

FileReadException::~FileReadException() throw()
{
}

const char* FileReadException::what() const throw()
{
  return "There occured an error at reading out a file!";
}
