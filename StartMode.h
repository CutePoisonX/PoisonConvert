/* 
 * File:   StartMode.h
 * Author: CutePoisonX
 *
 * Created on 10. Februar 2013, 10:02
 */

#ifndef STARTMODE_H
#define	STARTMODE_H

#include "Mode.h"
#include "VectorSourceManager.h"
#include "AnalyzeMedia.h"
#include "FileManager.h"
#include "SettingsMode.h"

#include <vector>

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
  VectorSourceManager&  vecman_;
  FileManager&          fileman_;
  vector<string>        important_files_;
  SettingsMode&         settings_;
  vector<unsigned int>  used_orig_id_;
  AnalyzeMedia&         analyze_;
  
  string                targets_;
  string                maps_;
  string                out_container_;
  string                logfile_;
  unsigned int          nr_video_targets_;
  unsigned int          nr_audio_targets_;
  unsigned int          nr_sub_targets_;
  //----------------------------------------------------------------------------
  
  int                   listDirectory(string dir, vector<string>& files_of_interest);
  int                   gettingFiles();
  int                   gettingInfos(string& filename);
  int                   applySettings();
  
  void                  evaluatingTargets(unsigned int priority_wish, 
                                          unsigned int identifier,
                                          unsigned int priority_orig);
  void                  optimizeFile(string& filename, string erase_log);
  void                  WriteLog(string message);
  void                  WriteLogHeader(int job);
  void                  WriteLogAnalyze();
  void                  WriteLogFfmpeg(string ffmpeg_cmd);
  void                  WriteLogOptimize();
  void                  MoveFile(string filename_full_path);
};

#endif	/* STARTMODE_H */

