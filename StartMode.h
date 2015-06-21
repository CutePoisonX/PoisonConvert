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

#ifndef STARTMODE_H
#define	STARTMODE_H

#include "Mode.h"
#include "VectorSourceManager.h"
#include "AnalyzeMedia.h"
#include "FileManager.h"
#include "SettingsMode.h"

#include <vector>
#include <map>

using namespace std;

class StartMode : public Mode{
public:
  
                        StartMode(UserInterface& ui, VectorSourceManager& man,
                                  FileManager& fileman, AnalyzeMedia& analyze, 
                                  SettingsMode& settings);
                        StartMode(const StartMode& orig);
  virtual              ~StartMode();
  
  int                   executeCommand();
  
private:

  //----------------------------------------------------------------------------
  VectorSourceManager&                      vecman_;
  FileManager&                              fileman_;
  vector<string>                            important_files_;
  SettingsMode&                             settings_;
  vector<unsigned int>                      used_orig_id_;
  AnalyzeMedia&                             analyze_;
  
  string                                    movie_duration_;
  string                                    targets_;
  string                                    maps_;
  string                                    out_container_;
  string                                    logfile_;
  unsigned int                              nr_video_targets_;
  unsigned int                              nr_audio_targets_;
  unsigned int                              nr_sub_targets_;

  map<string, map<unsigned int, string> >   prev_param_map_;
  //----------------------------------------------------------------------------
  
  int                   listDirectory(string dir, vector<string>& files_of_interest);
  int                   gettingFiles();
  int                   gettingInfos(string const& filename, string& movie_duration);
  bool                  applySettings();
  
  void                  evaluatingTargets(unsigned int priority_wish, 
                                          unsigned int identifier,
                                          unsigned int priority_orig);
  void                  optimizeFile(string& filename, string erase_log);
  void                  WriteLog(string message);
  void                  WriteLogHeader(int job);
  void                  WriteLogAnalyze(unsigned int identifier);
  void                  WriteLogFfmpeg(string ffmpeg_cmd);
  void                  WriteLogOptimize();
  void                  MoveFile(string filename_full_path);
  bool                  checkForConversionSuccess(int ffmpeg_response, string const& path_to_file);
};

#endif	/* STARTMODE_H */

