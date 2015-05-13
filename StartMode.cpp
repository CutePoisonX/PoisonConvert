//
//    Copyright 2014 CutePoisonX (CutePoisonXI@gmail.com)
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

#include "StartMode.h"
#include "UserInterface.h"
#include <sstream>
#include <unistd.h>
#include <stdio.h>
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

int StartMode::listDirectory(string dir, vector<string>& files_of_interest)
{
  DIR* target_dir;

  target_dir = opendir(dir.c_str());
  if(target_dir == NULL)
  {
    return -1;
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

    stat(path.c_str(), &filestat);

    //Recursively call this function if current object is a directory
    if(S_ISDIR(filestat.st_mode)) 
    {
      listDirectory(path, files_of_interest);
      continue;
    }

    string path_ext = path.substr(path.find_last_of(".") + 1);
    unsigned int vec_size = important_files_.size();

    for(unsigned int k = 0; k < vec_size; k++)
    {
      if ((path_ext == important_files_.at(k) || //matches exactly
          important_files_.at(k) == "-" || //all fileextensions match
          (important_files_.at(k).compare(0,3, "not") && important_files_.at(k).compare(3, string::npos, path_ext))) //matches when filextension is "not"...
          && path_ext != "old") //extension "old" is never a match
      {
        bool add_file = true;
        for(unsigned int important_file_nr = 0; important_file_nr < vec_size; important_file_nr++) //check if file is already added
        {
        	if (important_files_.at(important_file_nr) == path)
        	{
        		add_file = false;
        		break;
        	}
        }
        if (add_file == true)
        {
        	ui_.writeString("Found file: " + path, true);
        	files_of_interest.push_back(path);
        }
        break;
      }
    }
  }

  closedir(target_dir);

  return 0;
}

int StartMode::gettingFiles()
{
  string fileextension;
  
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
  vector<string> files_of_interest;
  string directory_to_list = settings_.getSettingsParam(MOVIEPATH);

  if (listDirectory(directory_to_list.substr(0, directory_to_list.size() - 1), files_of_interest) == -1)
  {
    ui_.writeString("Could not open source directory! Can not continue ...");
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

  string log_erase;
  
  unsigned int position = 0;
  unsigned int job = 0;
  unsigned int failed_items = 0;
  
  if (vecman_.getVectorLen(SRCVIDEO) == 0)
  {
    ui_.writeString("You have to enter at least one video-rule to start.", true);
    return -1;
  }

  try
  {
   gettingFiles(); 
  } catch (FileException)
  {
    ui_.writeString("Could not get any files to process.", true);
    exit_now = true;
  }
 
  if (exit_now != true)
  {
    for(job = 0; job < important_files_.size(); job++)
    {
      int ffmpeg_response = 1;
      bool conversion_success = false;

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
      
      WriteLog("1. Analyzed File");
      WriteLog("");
      gettingInfos(important_files_.at(job), movie_duration_);
      
      applySettings();
      
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
      
      rename(important_files_.at(job).c_str(), old_filename.c_str());
      
      ffmpeg_input = "ffmpeg -i \"";
      ffmpeg_input.append(old_filename);
      ffmpeg_input.append("\"");
      ffmpeg_input.append(maps_);
      ffmpeg_input.append(targets_);
      ffmpeg_input.append(" -strict -2 -y \"");
      
      if (out_container_ == "-")
      {
        out_container_ = important_files_.at(job).substr(0, important_files_.at(job).find_last_of("."));
      }
      filename_noext.append(out_container_);
      ffmpeg_input.append(filename_noext);
      
      WriteLogFfmpeg(ffmpeg_input);
      
      ffmpeg_input.append("\" 2>> ");
      log_erase = logfile_;
      log_erase.erase(0,5); //delete: [" >> ] (without brackets) from logfile
      ffmpeg_input.append(log_erase);
      
      ui_.writeString("  Converting...", true, "yellow");
      ffmpeg_response = system(ffmpeg_input.c_str());
      conversion_success = checkForConversionSuccess(ffmpeg_response, filename_noext);
      if (conversion_success)
      {
        ui_.writeString("  Finished converting...", true, "yellow");
        if(settings_.getSettingsParam(OPTIMIZESET) == "Yes" && (out_container_ == "m4v" ||
           out_container_ == "mov" || out_container_ == "mp4"))
        {
          optimizeFile(filename_noext, log_erase);
        }

        MoveFile(filename_noext);
      }
      else
      {
        ui_.writeString("  Conversion failed...", true, "yellow");

        //delete new file (incomplete)
        remove(filename_noext.c_str());
        //revert .old... extension:
        rename(old_filename.c_str(), important_files_.at(job).c_str());

        WriteLog("");
        WriteLog("C O N V E R S I O N    F A I L E D");
        failed_items++;
      }

      remove("/opt/tmp/poisonXprobelist");

      maps_.clear();
      targets_.clear();
      nr_audio_targets_ = 0;
      nr_sub_targets_ = 0;
      nr_video_targets_ = 0;
      prev_param_map_.clear();

      used_orig_id_.clear();    
      analyze_.clearEverything();
      ui_.writeString("Finished job ", false, "red");
      ui_.writeString(important_files_.at(job), true, "red");
      if(settings_.getSettingsParam(DELETESET) == "Yes" &&
         conversion_success)
      {
        remove(old_filename.c_str());
      }
    }
    if(job != 0)
    {
      if (failed_items == 0)
      {
        ui_.writeString("Done - all items succeeded! :)", true, "green");
      }
      else if (failed_items != job)
      {
        ui_.writeString("Done - ", false, "green");
        ui_.writeNumber(failed_items, false, "green");
        if (failed_items == 1)
        {
          ui_.writeString(" item failed! :/", true, "green");
        }
        else
        {
          ui_.writeString(" items failed! :/", true, "green");
        }
      }
      else
      {
        ui_.writeString("Done - all items failed! :(", true, "green");
      }
    }
    else
    {
      ui_.writeString("Nothing to be done :p", true, "green");
    }
  }
}

int StartMode::gettingInfos(string const& filename, string& movie_duration)
{
  string tmp_cmd = "ffprobe -print_format compact -show_streams \"";
  
  tmp_cmd.append(filename);
  tmp_cmd.append("\" &> /opt/tmp/poisonXprobelist");

  system(tmp_cmd.c_str());
  fileman_.readProperties(filename, movie_duration);

  WriteLogAnalyze(SRCVIDEO);
  WriteLogAnalyze(SRCAUDIO);
  WriteLogAnalyze(SRCSUB);
  
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
      if (out_container_ == "-" || out_container_ == "copy")
      {
        out_container_ = analyze_.sendSinglePreference(priority_orig, SRCVIDEO, 1); 
      }

      prev_param_map_["Video"][1] = out_container_;

    }
    else if (identifier == SRCAUDIO) //Codec - Audio
    {
      targets_.append(" -c:a:");
      ss << nr_audio_targets_;
      targets_.append(ss.str());
      ss.str("");
      targets_.append(" ");
      if (tmp_param == "copy")
      {
        copy_whole_thing = true;
      }
      else if(tmp_param == "-")
      {
        tmp_param = analyze_.sendSinglePreference(priority_orig, identifier, 1);
      }

      if (copy_whole_thing)
      {
        prev_param_map_["Audio"][1] = analyze_.sendSinglePreference(priority_orig, identifier, 1);
      }
      else
      {
        prev_param_map_["Audio"][1] = tmp_param;
      }
      targets_.append(tmp_param);
    }
    else if (identifier == SRCSUB)
    {
      targets_.append(" -c:s:");
      ss << nr_sub_targets_;
      targets_.append(ss.str());
      ss.str("");
      targets_.append(" ");
      targets_.append(tmp_param);
      copy_whole_thing = true; //because subtitle has no other parameters!

      if (tmp_param == "copy")
      {
        prev_param_map_["Subtitle"][1] = analyze_.sendSinglePreference(priority_orig, SRCSUB, 1);
      }
      else
      {
        prev_param_map_["Subtitle"][1] = tmp_param;
      }
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
        else if (identifier == SRCAUDIO)
        {
          targets_.append(" -ac:");
          ss << nr_audio_targets_;
        }
        targets_.append(ss.str());
        ss.str("");
        targets_.append(" ");
        if (tmp_param == "copy")
          copy_whole_thing = true;
        if((tmp_param == "copy" && identifier != SRCVIDEO) || (tmp_param == "-" && identifier == SRCVIDEO))
        {
          tmp_param = analyze_.sendSinglePreference(priority_orig, identifier, 2);
        }

        targets_.append(tmp_param);

        //fill the map:
        if (tmp_param == "copy")
        {
          tmp_param = analyze_.sendSinglePreference(priority_orig, identifier, 2);
        }
        if (identifier == SRCVIDEO)
        {
          prev_param_map_["Video"][2] = tmp_param;
        }
        else if (identifier == SRCAUDIO)
        {
          prev_param_map_["Audio"][2] = tmp_param;
        }
      }
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
        else if (identifier == SRCAUDIO)
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

        //fill the map:
        if (tmp_param == "copy")
        {
          tmp_param = analyze_.sendSinglePreference(priority_orig, identifier, 3);
        }
        if (identifier == SRCVIDEO)
        {
          prev_param_map_["Video"][3] = tmp_param;
        }
        else if (identifier == SRCAUDIO)
        {
          prev_param_map_["Audio"][3] = tmp_param;
        }
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

        //fill the map:
        if (tmp_param == "copy")
        {
          tmp_param = analyze_.sendSinglePreference(priority_orig, identifier, 4);
        }
        if (identifier == SRCVIDEO)
        {
          prev_param_map_["Video"][4] = tmp_param;
        }
        else if (identifier == SRCAUDIO)
        {
          prev_param_map_["Audio"][4] = tmp_param;
        }
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
        
        //fill the map:
        if (tmp_param == "copy")
        {
          tmp_param = analyze_.sendSinglePreference(priority_orig, identifier, 5);
        }
        if (identifier == SRCVIDEO)
        {
          prev_param_map_["Video"][5] = tmp_param;
        }
        else if (identifier == SRCAUDIO)
        {
          prev_param_map_["Audio"][5] = tmp_param;
        }
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

void StartMode::optimizeFile(string& filename, string erase_log)
{
  string new_filename = "\"";
  string filename_no_ext;
  string qt_start = "qt-faststart \"";
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
  
  remove(filename.c_str());
  rename(new_filename.c_str(), filename.c_str());
  
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

void StartMode::WriteLogAnalyze(unsigned int identifier)
{
  string output;
  string tmp_param[5] = {""};
  
  if (identifier == SRCVIDEO)
  {
    tmp_param[0] = "   Video:     Container       ";
    tmp_param[1] = "              Codec           ";
    tmp_param[2] = "              Bitrate         ";
    tmp_param[3] = "              Resolution      ";
    tmp_param[4] = "              Fps             ";

		for(unsigned int stream=1; stream<=analyze_.getVectorLength(SRCVIDEO); stream++)
		{
	    for(int i=0; i<5; i++)
	    {
	      try
	      {
	        output = tmp_param[i];
	        output.append(analyze_.sendSinglePreference(stream, SRCVIDEO, i + 1));
	        WriteLog(output);
	      }
	      catch (exception)
	      {
	        output.append("-");
	        WriteLog(output);
	      }
	    }
		}
  }
  else if (identifier == SRCAUDIO)
  {
    tmp_param[0] = "   Audio:     Codec           ";
    tmp_param[1] = "              Channels        ";
    tmp_param[2] = "              Language        ";
    tmp_param[3] = "              Bitrate         ";
    tmp_param[4] = "              Sample Rate     ";
    
		for(unsigned int stream=1; stream<=analyze_.getVectorLength(SRCAUDIO); stream++)
		{
	    for (int i = 0; i < 5; i++)
	    {
	      try
	      {
	        output = tmp_param[i];
	        output.append(analyze_.sendSinglePreference(stream, SRCAUDIO, i + 1));
	        WriteLog(output);
	      }
	      catch(exception)
	      {
	        output.append("-");
	        WriteLog(output);
	      }
	    }
		}
  }
  else if (identifier == SRCSUB)
  {
    tmp_param[0] = "   Subtitle:  Codec           ";
    tmp_param[1] = "              Language        ";

	for(unsigned int stream=1; stream<=analyze_.getVectorLength(SRCSUB); stream++)
	{
	  for(int i=0; i<2; i++)
	  {
      try
      {
        output = tmp_param[i];
        output.append(analyze_.sendSinglePreference(stream, SRCSUB, i + 1));
        WriteLog(output);
      }
      catch(exception)
      {
        output.append("-");
        WriteLog(output);
      }
	  }
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

void StartMode::MoveFile(string filename_full_path)
{
  string move_file = "mv \"";
  string movie_destination = settings_.getSettingsParam(DESTINATION);
  string filename_without_path = filename_full_path.substr(filename_full_path.find_last_of("/"));
  
  move_file.append(filename_full_path);
  move_file.append("\" \"");
  move_file.append(movie_destination.substr(0, movie_destination.size() - 1));
  move_file.append(filename_without_path);
  move_file.append("\" 2>/dev/null");
  
  system(move_file.c_str());
}

bool StartMode::checkForConversionSuccess(int ffmpeg_response,
                                          string const& path_to_file)
{
  string new_movie_duration;

  //checking response of ffmpeg:
  if (ffmpeg_response != 0)
  {
    return false;
  }

  WriteLog("");
  WriteLog("4. Analyzed processed file");
  WriteLog("");
  analyze_.clearEverything();
  gettingInfos(path_to_file, new_movie_duration);

  //check duration:
  if (movie_duration_ != new_movie_duration)
  {
    return false;
  }

  //check stream parameter:
	for(unsigned int stream=1; stream<=analyze_.getVectorLength(SRCVIDEO); stream++)
	{
		for(int i=0; i<2; i++)
	  {
	    try
	    {
	      //this will throw if param is "-":
	      string should_output = analyze_.sendSinglePreference(stream, SRCVIDEO, i + 1);
	      if (prev_param_map_.at("Video").at(i+1) != should_output)
	      {
	        return false;
	      }
	    }
	    catch (exception)
	    {
	
	    }
	  }
	}
	
	for(unsigned int stream=1; stream<=analyze_.getVectorLength(SRCAUDIO); stream++)
	{
	  for(int i=0; i<5; i++)
	  {
	    try
	    {
	      //this will throw if param is "-":
	      string should_output = analyze_.sendSinglePreference(stream, SRCAUDIO, i + 1);
	      if (prev_param_map_.at("Audio").at(i+1) != should_output)
	      {
	        return false;
	      }
	    }
	    catch (exception)
	    {
	
	    }
	  }
	}

	for(unsigned int stream=1; stream<=analyze_.getVectorLength(SRCSUB); stream++)
	{
	  for(int i=0; i<2; i++)
	  {
		  try
		  {
		    //this will throw if param is "-":
		    string should_output = analyze_.sendSinglePreference(stream, SRCSUB, 1);
		    if (prev_param_map_.at("Subtitle").at(1) != should_output)
		    {
		      return false;
		    }
		  }
		  catch (exception)
		  {
		
		  }
	  }
	}

  WriteLog("C O N V E R S I O N    S U C C E E D E D");
  return true;
}
