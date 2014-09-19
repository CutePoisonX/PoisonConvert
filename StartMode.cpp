/* 
 * File:   StartMode.cpp
 * Author: CutePoisonX
 * 
 * Created on 10. Februar 2013, 10:02
 */

#include "StartMode.h"
#include "UserInterface.h"
#include <sstream>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

StartMode::StartMode(UserInterface& ui, VectorSourceManager& man,
                     FileManager& fileman, AnalyzeMedia& analyze,
                     SettingsMode& settings)
: Mode(ui), vecman_(man), fileman_(fileman), analyze_(analyze), settings_(settings),
  nr_video_targets_(0), nr_audio_targets_(0), nr_sub_targets_(0)
{
}

StartMode::StartMode(const StartMode& orig)
: Mode(ui_), vecman_(orig.vecman_), fileman_(orig.fileman_), analyze_(orig.analyze_),
        nr_video_targets_(orig.nr_video_targets_), nr_audio_targets_(orig.nr_audio_targets_), 
        nr_sub_targets_(orig.nr_sub_targets_), settings_(orig.settings_)
{
}

StartMode::~StartMode()
{
}

vector<string> StartMode::listDirectory(string dir)
{
  DIR* target_dir;
  vector<string> files_of_interest;
  
  target_dir = opendir(dir.c_str());
  if(target_dir == NULL)
  {
    ui_.writeString("Could not open source directory! Can not continue ...");
    return files_of_interest;
  }

  struct dirent* dirP;
  struct stat filestat;
  std::string path;
  while( (dirP = readdir(target_dir)) ) 
  {
    //Skip current object if it is this directory or parent directory
    if(!strncmp(dirP->d_name, ".", 1) || !strncmp(dirP->d_name, "..", 2))
    {
      continue;
    }

    if(dir == ".")
    {
      path = dirP->d_name;
    }
    else
    {
      path = dir + "/" + dirP->d_name;
    }   
    //Skip current file / directory if it is invalid in some way
    if(stat(path.c_str(), &filestat))
    {
      continue;
    }

    //Recursively call this function if current object is a directory
    if(S_ISDIR(filestat.st_mode)) 
    {
      listDirectory(path);
      continue;
    }

    string path_ext = path.substr(path.find_last_of(".") + 1);
    unsigned int vec_size = important_files_.size();
    
    for(unsigned int k = 0; k < vec_size; k++)
    {      
      if (path_ext == important_files_.at(k))
      {
          files_of_interest.push_back(path);
          break;
      }
    }
  }

  closedir(target_dir);

  return files_of_interest;
}

int StartMode::gettingFiles()
{
  string fileextension;
  string list_dirs = "ls "; // linux: "ls *." windows: "dir *."
  
  bool already_here = false;
  unsigned int vec_size = 0;
  //getting fileextensions
  
  for (unsigned int priority = 1; priority <= vecman_.getVectorLen(SRCVIDEO); priority++)
  {
    vec_size = important_files_.size();
    already_here = false;
    fileextension = vecman_.sendSinglePreference(priority, SRCVIDEO, 1);
    if (vec_size > 0)
    {
      for(unsigned int i = 0; i < vec_size; i++)
      {
        if(fileextension == important_files_.at(i))
        {
          already_here = true;
        }
      }
    }
    if(already_here == false)
    {
      important_files_.push_back(fileextension);
    }
  }
  
  //getting files
  vector<string> files_of_interest = listDirectory(settings_.getSettingsParam(MOVIEPATH));
  
  if (files_of_interest.empty())
  {
      throw OpenFileException();
  }
  else
  {
    important_files_.clear();
    important_files_ = files_of_interest;
  }
}

int StartMode::executeCommand()
{
  bool exit_now = false;
  string ffmpeg_input = "ffmpeg -i \"";
  string filename_noext;
  string old_filename;
  string old_fileext;

  string rename = "mv \"";
  string delete_orig = "rm \"";
  string log_erase;
  
  unsigned int position = 0;
  unsigned int job = 0;
  
  try
  {
   gettingFiles(); 
  } catch (FileException)
  {
    ui_.writeString("Could not get any files to process.", true);
    exit_now = false;
  }
 
  if (exit_now != true)
  {
    for(job = 0; job < important_files_.size(); job++)
    {
      string filename_without_path = important_files_.at(job).substr(important_files_.at(job).find_last_of("/") + 1);
      logfile_ = "\" >> \"";
      logfile_.append(settings_.getSettingsParam(LOGPATH));
      logfile_.append(filename_without_path);
      logfile_.append(".log");
      logfile_.append("\"");
      
      WriteLogHeader(job);
              
      ui_.writeString("", true);
      ui_.writeString("Started job ", false, "red");
      ui_.writeNumber(job + 1, false, "red");
      ui_.writeString("/", false, "red");
      ui_.writeNumber(important_files_.size(), false, "red");
      ui_.writeString(": ", false, "red");
      ui_.writeString(filename_without_path, true, "red");
      
      gettingInfos(important_files_.at(job)); 
      
      applySettings();
      
      rename = "mv \"";
      filename_noext = important_files_.at(job);
      position = filename_noext.find_last_of(".");
      old_fileext = filename_noext.substr(position, filename_noext.length() - position);
      filename_noext.erase(position + 1, filename_noext.length());
      
      
      old_filename = important_files_.at(job);
      old_filename.append(".old");
      
      if(nr_video_targets_ == 0 && vecman_.getVectorLen(SRCVIDEO) != 0 &&
         analyze_.getVectorLen(SRCVIDEO) != 0)
        old_filename.append("_nv");
      if(nr_audio_targets_ == 0 && vecman_.getVectorLen(SRCAUDIO) != 0 &&
         analyze_.getVectorLen(SRCAUDIO) != 0)
        old_filename.append("_na");
      if(nr_sub_targets_ == 0 && vecman_.getVectorLen(SRCSUB) != 0 &&
         analyze_.getVectorLen(SRCSUB))
        old_filename.append("_ns");
      
      rename.append(important_files_.at(job));
      rename.append("\" \"");
      rename.append(old_filename);
      rename.append("\" 2>/dev/null");
      
      system(rename.c_str());
      
      ffmpeg_input = "ffmpeg -i \"";
      ffmpeg_input.append(old_filename);
      ffmpeg_input.append("\"");
      ffmpeg_input.append(maps_);
      ffmpeg_input.append(targets_);
      ffmpeg_input.append(" -strict -2 -y \"");
      
      filename_noext.append(out_container_);
      ffmpeg_input.append(filename_noext);
      
      WriteLogFfmpeg(ffmpeg_input);
      
      ffmpeg_input.append("\" 2>> ");
      log_erase = logfile_;
      log_erase.erase(0,5); //delete: [" >> ] (without brackets) from logfile
      ffmpeg_input.append(log_erase);
      
      ui_.writeString("  Converting...", true, "yellow");
      system(ffmpeg_input.c_str());
      ui_.writeString("  Finished converting...", true, "yellow");
      
      if(settings_.getSettingsParam(OPTIMIZESET) == "Yes" && (out_container_ == "m4v" ||
         out_container_ == "mov" || out_container_ == "mp4"))
        optimizeFile(filename_noext, log_erase);
      
      system("rm /opt/tmp/poisonXprobelist 2>/dev/null");
      MoveFile(filename_noext);
      
      maps_.clear();
      targets_.clear();
      nr_audio_targets_ = 0;
      nr_sub_targets_ = 0;
      nr_video_targets_ = 0;

      used_orig_id_.clear();    
      analyze_.clearEverything();
      ui_.writeString("Finished job ", false, "red");
      ui_.writeString(important_files_.at(job), true, "red");
      if(settings_.getSettingsParam(DELETESET) == "Yes")
      {
        delete_orig.append(old_filename);
        delete_orig.append("\" 2>/dev/null");
        system(delete_orig.c_str());
        delete_orig = "rm \"";
      }
    }
    if(job != 0)
      ui_.writeString("Done :)", true, "green");
    else
      ui_.writeString("Nothing to be done :p", true, "green");
  }
}

int StartMode::gettingInfos(string& filename)
{
  string tmp_cmd = "ffprobe -print_format compact \"";

  WriteLog("1. Analyzed File");
  WriteLog("");
  
  tmp_cmd.append(filename);
  tmp_cmd.append("\" 2> /opt/tmp/poisonXprobelist");

  system(tmp_cmd.c_str());
  fileman_.readProperties(filename);

  WriteLogAnalyze();
  
}

int StartMode::applySettings()
{
  stringstream ss;
  string orig_pref;
  string wish_pref;

  bool preferences[5] = {false};
  bool use_this_id = true;
  
  unsigned int vec_len_wish = 0;
  unsigned int vec_len_orig = 0;
  unsigned int correcture = 0;
  
  //selecting maps
  for (unsigned int identifier = SRCVIDEO; identifier <= SRCSUB; identifier++)
  {
    vec_len_wish = vecman_.getVectorLen(identifier);
    vec_len_orig = analyze_.getVectorLen(identifier);

    if (identifier == SRCVIDEO)
      correcture = 0;
    if (identifier == SRCAUDIO)
      correcture = analyze_.getVectorLen(SRCVIDEO);
    if (identifier == SRCSUB)
      correcture = analyze_.getVectorLen(SRCVIDEO) + analyze_.getVectorLen(SRCAUDIO);

    for (unsigned int priority_orig = 1; priority_orig <= vec_len_orig; priority_orig++)
    {
      for (unsigned int priority_wish = 1; priority_wish <= vec_len_wish; priority_wish++)
      {
        use_this_id = true;
        for (unsigned int used_item = 0; used_item < used_orig_id_.size(); used_item++)
        {
          //check, whether id is already in use
          if ((priority_orig + correcture) == used_orig_id_.at(used_item))
          {
            use_this_id = false;
          }
        }
        preferences[0] = false;
        preferences[1] = false;
        preferences[2] = false;
        preferences[3] = false;
        preferences[4] = false;

        if (use_this_id == true)
        {
          for (unsigned int param_nr = 1; param_nr <= 5; param_nr++)
          {
            //Analyzing a certain parameter
            orig_pref = analyze_.sendSinglePreference(priority_orig, identifier, param_nr);
            wish_pref = vecman_.sendSinglePreference(priority_wish, identifier, param_nr);
 
            if (wish_pref.compare(0, 3, "NOT") == 0) //this value should not be equal
            {
              if (wish_pref.compare(3, wish_pref.length(), orig_pref) != 0)
              {
                preferences[param_nr - 1] = true;
              }
            } else if (wish_pref == "-" || wish_pref == "OFF") //This parameter is not important or does not exist
            {
              preferences[param_nr - 1] = true;
            } else if (wish_pref == orig_pref) //Parameters should be equal
            {
              preferences[param_nr - 1] = true;
            }
          }
          if (preferences[0] == true && preferences[1] == true && preferences[2] == true &&
              preferences[3] == true && preferences[4] == true)
          {
            maps_.append(" -map 0:");
            ss << ((priority_orig - 1) + correcture);
            maps_.append(ss.str());
            ss.str("");
            used_orig_id_.push_back(priority_orig + correcture);
            
            //evaluating targets
            evaluatingTargets(priority_wish, identifier, priority_orig);
          }
        }
      }
    }
  }
}

void StartMode::evaluatingTargets(unsigned int priority_wish, unsigned int identifier,
                                  unsigned int priority_orig)
{
  stringstream ss;
  unsigned int target_vec_len = vecman_.getTargetVectorLenFromSrc(identifier, priority_wish);
  unsigned int tmp_target_id = 0;
  unsigned int target_identifier = identifier + 3;
  
  string tmp_param;
  
  bool copy_whole_thing = false;
  
  for (unsigned int target_nr = 1; target_nr <= target_vec_len; target_nr++)
  {
    tmp_target_id = vecman_.getTargetIDfromSource(priority_wish, identifier, target_nr);
    copy_whole_thing = false;
    tmp_param = vecman_.sendSinglePreference(tmp_target_id, target_identifier, 1); //1 is the container if identifier is SRCVIDEO
    if (identifier == SRCVIDEO)
    {
      out_container_ = vecman_.sendSinglePreference(tmp_target_id, TARVIDEO, 1);
      if (out_container_ == "-")
      {
        out_container_ = analyze_.sendSinglePreference(priority_orig, SRCVIDEO, 1); 
      }
    }
    if (identifier == SRCAUDIO) //Codec - Audio
    {
      targets_.append(" -c:a:");
      ss << nr_audio_targets_;
      targets_.append(ss.str());
      ss.str("");
      targets_.append(" ");
      if (tmp_param == "copy")
        copy_whole_thing = true;
      if(tmp_param == "-")
        tmp_param = analyze_.sendSinglePreference(priority_orig, identifier, 1); 
      targets_.append(tmp_param);
    }
    if (identifier == SRCSUB)
    {
      targets_.append(" -c:s:");
      ss << nr_sub_targets_;
      targets_.append(ss.str());
      ss.str("");
      targets_.append(" ");
      targets_.append(tmp_param);
      copy_whole_thing = true;
    }
    if (copy_whole_thing == false)
    {
      //Codec at SRCVIDEO, Channels at SRCAUDIO
      tmp_param = vecman_.sendSinglePreference(tmp_target_id, target_identifier, 2);
      if (tmp_param != "-" || identifier == SRCVIDEO)
      {
        if (identifier == SRCVIDEO)
        {
          targets_.append(" -c:v:");
          ss << nr_video_targets_;
        }
        if (identifier == SRCAUDIO)
        {
          targets_.append(" -ac:");
          ss << nr_audio_targets_;
        }
        targets_.append(ss.str());
        ss.str("");
        targets_.append(" ");
        if (tmp_param == "copy")
          copy_whole_thing = true;
        if((tmp_param == "copy" && identifier != SRCVIDEO) || tmp_param == "-" && identifier == SRCVIDEO)
        {
          tmp_param = analyze_.sendSinglePreference(priority_orig, identifier, 2);
        }
        targets_.append(tmp_param);
      }
      if (copy_whole_thing == false)
      {
        //Bitrate at SRCVIDEO or Language at SRCAUDIO
        tmp_param = vecman_.sendSinglePreference(tmp_target_id, target_identifier, 3);
        if (tmp_param != "-")
        {
          if (identifier == SRCVIDEO)
          {
            targets_.append(" -b:v:");
            ss << nr_video_targets_;
            targets_.append(ss.str());
            targets_.append(" ");
          }

          if (identifier == SRCAUDIO)
          {
            targets_.append(" -metadata:s:a:");
            ss << nr_audio_targets_;
            targets_.append(ss.str());
            targets_.append(" language=");
          }
          ss.str("");
          if (tmp_param == "copy")
          {
            tmp_param = analyze_.sendSinglePreference(priority_orig, identifier, 3);
          }
          targets_.append(tmp_param);
        }
        //resolution at SRCVIDEO or Bitrate at SRCAUDIO
        tmp_param = vecman_.sendSinglePreference(tmp_target_id, target_identifier, 4);
        if (tmp_param != "-")
        {
          if (identifier == SRCVIDEO)
          {
            targets_.append(" -s:v:");
            ss << nr_video_targets_;
          }
          if (identifier == SRCAUDIO)
          {
          targets_.append(" -b:a:");
          ss << nr_audio_targets_;
          }
          targets_.append(ss.str());
          ss.str("");
          targets_.append(" ");
          if (tmp_param == "copy")
          {
            tmp_param = analyze_.sendSinglePreference(priority_orig, identifier, 4);
          }
          targets_.append(tmp_param);
        }
        
        //fps at SRCVIDEO, sample rate at SRCAUDIO
        tmp_param = vecman_.sendSinglePreference(tmp_target_id, target_identifier, 5);
        if (tmp_param != "-")
        {
          if (identifier == SRCVIDEO)
          {
            targets_.append(" -r:v:");
            ss << nr_video_targets_;
          }
          if (identifier == SRCAUDIO)
          {
            targets_.append(" -ar:");
            ss << nr_audio_targets_ + nr_video_targets_;
          }
          targets_.append(ss.str());
          ss.str("");
          targets_.append(" ");
          if (tmp_param == "copy")
          {
            tmp_param = analyze_.sendSinglePreference(priority_orig, identifier, 5);
          }
          targets_.append(tmp_param);
        }
      }
    }
    if(identifier == SRCVIDEO)
      nr_video_targets_++;
    if(identifier == SRCAUDIO)
      nr_audio_targets_++;
    if(identifier == SRCSUB)
      nr_sub_targets_++;
  }
}

void StartMode::optimizeFile(string& filename, string erase_log)
{
  string new_filename = "\"";
  string filename_no_ext;
  string qt_start = "qt-faststart \""; //"qtfaststart" for OS X?
  string remove = "rm \"";
  string rename = "mv ";
  unsigned int position;
  
  ui_.writeString("  Started optimizing...", true, "yellow");

  position = filename.find_last_of(".");
  filename_no_ext = filename.substr(0, position);
  
  WriteLogOptimize();
  new_filename.append(filename_no_ext);
  new_filename.append(".optimized.");
  new_filename.append(out_container_);
  
  qt_start.append(filename);
  qt_start.append("\" ");
  qt_start.append(new_filename);
  qt_start.append(logfile_);
  qt_start.append(" 2>> ");
  qt_start.append(erase_log);

  system(qt_start.c_str());
  
  remove.append(filename);
  remove.append("\" 2>/dev/null");
  system(remove.c_str());
  
  rename.append(new_filename);
  rename.append("\" \"");
  rename.append(filename);
  rename.append("\" 2>/dev/null");

  system(rename.c_str());
  
  ui_.writeString("  Finished optimizing...", true, "yellow");
}

void StartMode::WriteLog(string message)
{
  string out = "echo \"";
  
  message.append(logfile_);
  out.append(message);

  system(out.c_str());
}

void StartMode::WriteLogHeader(int job)
{
  string message;
  
  WriteLog("-----------------------------------------------------");
  WriteLog("-----------------------------------------------------");
  message = "PoisonConvert Logfile for Job: ";
  message.append(important_files_.at(job));
  WriteLog(message);
  WriteLog("-----------------------------------------------------");
  WriteLog("Started Processing");
  WriteLog("");
}

void StartMode::WriteLogAnalyze()
{
  string output;
  string tmp_param[5] = { "   Video:     Container       ",
                          "              Codec           ",
                          "              Bitrate         ",
                          "              Resolution      ",
                          "              Fps             "};
  
  for(int i=0; i<5; i++)
  {
    try
    {
      output = tmp_param[i];
      output.append(analyze_.sendSinglePreference(1, SRCVIDEO, i + 1));
      WriteLog(output);
    }    
    catch (exception)
    {
      output.append("-");
      WriteLog(output);
    }
  }
  
  WriteLog("");
  tmp_param[0] = "   Audio:     Codec           ";
  tmp_param[1] = "              Channels        ";
  tmp_param[2] = "              Language        ";
  tmp_param[3] = "              Bitrate         ";
  tmp_param[4] = "              Sample Rate     ";
  
  for (int i = 0; i < 5; i++)
  {
    try
    {
      output = tmp_param[i];
      output.append(analyze_.sendSinglePreference(1, SRCAUDIO, i + 1));
      WriteLog(output);
    }
    catch(exception)
    {
      output.append("-");
      WriteLog(output);
    }
  }

  WriteLog("");
  tmp_param[0] = "   Subtitles: Codec           ";
  tmp_param[1] = "              Language        ";
  
  for(int i=0; i<2; i++)
  {
    try
    {
      output = tmp_param[i];
      output.append(analyze_.sendSinglePreference(1, SRCSUB, i + 1));
      WriteLog(output);
    }
    catch(exception)
    {
      output.append("-");
      WriteLog(output);
    }
  }
  WriteLog("");
}

void StartMode::WriteLogFfmpeg(string ffmpeg_cmd)
{
  string ffmpeg_log = "   ";

  WriteLog("2. ffmpeg command line");
  WriteLog("");

  for(int i=0;i < ffmpeg_cmd.length(); i++)
  {
    if(ffmpeg_cmd[i] == '\"')
    {
      ffmpeg_cmd.erase(i,1);
    }
  }
  ffmpeg_log.append(ffmpeg_cmd);
  WriteLog(ffmpeg_log);
  WriteLog("");
  WriteLog("3. ffmpeg output");
  WriteLog("-----------------------------------------------------");
}

void StartMode::WriteLogOptimize()
{
  WriteLog("-----------------------------------------------------");
  WriteLog("");
  WriteLog("4. qt-faststart output");
  WriteLog("");
  
}
void StartMode::MoveFile(string filename)
{
  string move_file = "mv \"";
  
  move_file.append(settings_.getSettingsParam(MOVIEPATH));
  move_file.append(filename);
  move_file.append("\" \"");
  move_file.append(settings_.getSettingsParam(DESTINATION));
  move_file.append(filename);
  move_file.append("\"");
  
  system(move_file.c_str());
}