/* 
 * File:   FileManager.h
 * Author: CutePoisonX
 *
 * Created on 08. Februar 2013, 09:29
 */

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

  int           readSettings() throw (FileReadException, OpenFileException);
  
  void          saveSettingsToFile() throw(FileWriteException);
  void          readPreferences() throw (FileReadException, OpenFileException);
  int          savePreferencesToFile() throw(FileWriteException);
  string           checkPathToConfig();
  void          readImportantFiles(vector<string>& filenames) throw (FileReadException, 
                                                                      OpenFileException);
  void          readProperties(string filename) throw (FileReadException, OpenFileException);
  void          readPropsVideo(string line, unsigned int pos_start, string (&params)[5]);
  void          readPropsAudio(string line, unsigned int pos_start, string (&params)[5]);
  void          readPropsSub(string line, unsigned int pos_start, string& param0,
                             string& param1);
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

