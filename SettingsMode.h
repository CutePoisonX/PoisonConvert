/* 
 * File:   SettingsMode.h
 * Author: CutePoisonX
 *
 * Created on 08. Februar 2013, 09:40
 */

#ifndef SETTINGSMODE_H
#define	SETTINGSMODE_H

#include "Mode.h"
#include <string>
using namespace std;

//settings in list-command
const string first_list_setting = "Selecting filename of config - file";
const string second_list_setting = "Delete original file after conversion";
const string third_list_setting = "Optimize file for streaming (mp4/m4v/mov only)";

//selections
const string first_selection = "filename";
const string second_selection = "delete";
const string third_selection = "stream";

class UserInterface;

class SettingsMode : public Mode {
public:
  
                SettingsMode(UserInterface& ui, string pref_filename,
                            bool erase_original, bool optimize_streaming);
                SettingsMode(const SettingsMode& orig);
  virtual      ~SettingsMode();
  
  int           executeCommand();
  
  //----------------------------------------------------------------------------
  const bool  getStreamingSetting() const
  {
    return optimize_streaming_;
  }

  const bool GetEraseSetting() const
  {
    return erase_original_;
  }

  const string getFilenameSetting() const
  {
    return pref_filename_;
  }

  void setStreamingSetting(bool optimize_streaming_)
  {
    this->optimize_streaming_ = optimize_streaming_;
  }

  void setEraseSetting(bool erase_original_)
  {
    this->erase_original_ = erase_original_;
  }

  void setFilenameSetting(string pref_filename_)
  {
    this->pref_filename_ = pref_filename_;
  }
  //----------------------------------------------------------------------------
  
private:
  
  //----------------------------------------------------------------------------
  string        pref_filename_;
  bool          erase_original_;
  bool          optimize_streaming_;
  //----------------------------------------------------------------------------
  
  void          standartExecutePrompt();
  void          changeDeleteOriginal(); //Second setting
  void          changeOptimizeForStreaming(); //Third setting
  int           changeFilename(); //first_setting
  int           listSettings();
};

#endif	/* SETTINGSMODE_H */

