/* 
 * File:   FileSettingsException.h
 * Author: CutePoisonX
 *
 * Created on 08. Februar 2013, 10:46
 */

#ifndef FILEREADEXCEPTION_H
#define	FILEREADEXCEPTION_H

#include "FileException.h"


class FileReadException : public FileException{
public:
  
                FileReadException();
                FileReadException(const FileReadException& orig);
  virtual      ~FileReadException() throw();
  
  virtual const char* what() const throw ();

};

#endif	/* FILEREADEXCEPTION_H */

