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

#ifndef FILEMANAGER_H
#define	FILEMANAGER_H

#include <fstream>

#include "FileReadException.h"
#include "OpenFileException.h"
#include "FileWriteException.h"
#include "SettingsMode.h"
#include "VectorSourceManager.h"
#include "AnalyzeMedia.h"

using namespace std;

class StreamPreference;

class FileManager {
public:
  
                FileManager(SettingsMode& settings, VectorSourceManager& vec_man,
                            AnalyzeMedia& analyze);
                FileManager(const FileManager& orig);
  virtual      ~FileManager();

  bool          readSettings() throw (FileReadException, OpenFileException);
  void          saveSettingsToFile() throw(FileWriteException);
    
  void          readPreferences() throw (FileReadException, OpenFileException);
  int           savePreferencesToFile() throw(FileWriteException);
    
  string        checkPathToConfig();
  void          readImportantFiles(vector<string>& filenames) throw (FileReadException, 
                                                                      OpenFileException);
  void          readProperties(string filename, string& duration) throw (FileReadException, OpenFileException);
  void          readPropsVideo(string line, string (&params)[5]);
  void          readPropsAudio(string line, string (&params)[5]);
  void          readPropsSub(string line, string& param0, string& param1);
  string 				processOrdinaryParameter(string const& line, string const& param_name);
private:
  
  //----------------------------------------------------------------------------
  SettingsMode&         setting_;
  VectorSourceManager&  vec_man_;
  AnalyzeMedia&         analyze_;
  //----------------------------------------------------------------------------
  
  void          ReadFileError(ifstream& readfile) throw (FileReadException);
  void          writePrefs(ofstream& writefile, unsigned int identifier);
  void          readPrefs(ifstream& readfile, unsigned int identifier, 
                          unsigned int tmp_count) throw(FileReadException);
  
  bool          stringToInt(std::string& string_number, unsigned int& value);
};

#endif	/* FILEMANAGER_H */

