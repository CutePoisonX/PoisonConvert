/*
 * File:   AnalyzeMedia.cpp
 * Author: CutePoisonX
 *
 * Created on 10. Februar 2013, 10:14
 */

#include <string>

#include "AnalyzeMedia.h"

AnalyzeMedia::AnalyzeMedia()
: VectorManager()
{ 
}

AnalyzeMedia::AnalyzeMedia(const AnalyzeMedia& orig)
: VectorManager()
{
}

AnalyzeMedia::~AnalyzeMedia()
{
}

int AnalyzeMedia::saveToVector(string& param1, string& param2, string& param3,
                               string& param4, string& param5, unsigned int priority,
                               unsigned int identifier)
{
  vector<StreamPreference*>& tmp = getVector(identifier);
    
  tmp.push_back(new StreamPreference(param1, param2, param3, param4, param5));
}

vector<StreamPreference*>& AnalyzeMedia::getVector(unsigned int identifier)
{
    switch (identifier)
  {
    case SRCVIDEO:
      return source_video_vect_;
    case SRCAUDIO:
      return source_audio_vect_;
    case SRCSUB:
      return source_sub_vect_;
  }
}

void AnalyzeMedia::clearEverything()
{
  source_audio_vect_.clear();
  source_sub_vect_.clear();
  source_video_vect_.clear();
}

void AnalyzeMedia::clearAllInstances()
{
  //source video
  for(unsigned int index = 0; index < source_video_vect_.size(); index++)
  {
    delete source_video_vect_.at(index);
  }
  source_video_vect_.clear();
  
  //source audio
  for(unsigned int index = 0; index < source_audio_vect_.size(); index++)
  {
    delete source_audio_vect_.at(index);
  }
  source_audio_vect_.clear();
  
  //source subtitles
  for(unsigned int index = 0; index < source_sub_vect_.size(); index++)
  {
    delete source_sub_vect_.at(index);
  }
  source_sub_vect_.clear();
}
