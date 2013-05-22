/* 
 * File:   FileWriteException.h
 * Author: CutePoisonX
 *
 * Created on 09. Februar 2013, 13:03
 */

#ifndef FILEWRITEEXCEPTION_H
#define	FILEWRITEEXCEPTION_H

#include "FileException.h"


class FileWriteException : public FileException{
public:
  
                FileWriteException();
                FileWriteException(const FileWriteException& orig);
  virtual      ~FileWriteException() throw();

  virtual const char* what() const throw ();
};

#endif	/* FILEWRITEEXCEPTION_H */

