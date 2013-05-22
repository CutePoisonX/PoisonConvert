/* 
 * File:   VectorManager.cpp
 * Author: CutePoisonX
 * 
 * Created on 03. Februar 2013, 23:28
 */

#include "VectorSourceManager.h"

VectorSourceManager::VectorSourceManager()
: VectorManager()
{
}

VectorSourceManager::VectorSourceManager(const VectorSourceManager& orig)
{
}

VectorSourceManager::~VectorSourceManager()
{
}

int VectorSourceManager::saveToVector(string& param1, string& param2, string& param3,
                                      string& param4, string& param5,
                                      unsigned int priority, unsigned int identifier)
{
  vector<StreamPreference*>& tmp = getVector(identifier);
  priority--;
  vector<StreamPreference*>::iterator it = getSourceIterator(priority, 
                                           tmp);
  tmp.insert(it, new StreamPreference(param1, param2, param3, param4, param5));
}

vector<StreamPreference*>& VectorSourceManager::getVector(unsigned int identifier)
{
  switch (identifier)
  {
    case SRCVIDEO:
      return source_video_vect_;
    case SRCAUDIO:
      return source_audio_vect_;
    case SRCSUB:
      return source_sub_vect_;
    case TARVIDEO:
      return target_video_vect_;
    case TARAUDIO:
      return target_audio_vect_;
    case TARSUB:
      return target_sub_vect_;
  }
}

int VectorSourceManager::deleteFromVector(unsigned int priority,
                                          unsigned int identifier)
{
  vector<StreamPreference*>& tmp = getVector(identifier);
  priority--;
  vector<StreamPreference*>::iterator it = getSourceIterator(priority, 
                                           tmp);
  delete tmp.at(priority);
  tmp.erase(it);
}

int VectorSourceManager::editPreference(unsigned int priority, unsigned int identifier,
                                        unsigned int param_nr, string& new_param)
{
  priority--;
    switch (param_nr)
  {
    case 1:
        getVector(identifier).at(priority)->SetParam1(new_param);
        break;
    case 2:
        getVector(identifier).at(priority)->SetParam2(new_param);
        break;
    case 3:
        getVector(identifier).at(priority)->SetParam3(new_param);
        break;
    case 4:
        getVector(identifier).at(priority)->SetParam4(new_param);
        break;
    case 5:
        getVector(identifier).at(priority)->SetParam5(new_param);
        break;
  }
}

int VectorSourceManager::moveElement(unsigned int priority, 
                                     unsigned int new_priority, 
                                     unsigned int identifier)
{
  priority--;
  new_priority--;
  vector<StreamPreference*>& tmp_vec = getVector(identifier);
  StreamPreference* tmp_point = tmp_vec.at(priority);
  
  vector<StreamPreference*>::iterator it_old = getSourceIterator(priority, 
                                           tmp_vec);
  
  tmp_vec.erase(it_old);
  
  vector<StreamPreference*>::iterator it_new = getSourceIterator(new_priority, 
                                           tmp_vec);
  tmp_vec.insert(it_new, tmp_point);
}

int VectorSourceManager::linkNewTarget(unsigned int target_priority, 
                                       unsigned int identifier,
                                       unsigned int priority)
{
  priority--;
  target_priority--;
  getVector(identifier).at(priority)->saveTarget(target_priority);
}

int VectorSourceManager::deleteAllTarget(unsigned int priority, 
                                      unsigned int identifier,
                                      unsigned int target_id)
{
  priority--;
  target_id--;
  return getVector(identifier).at(priority)->deleteAllTargets(target_id);
}

int VectorSourceManager::editTarget(unsigned int priority, 
                                      unsigned int identifier,
                                      unsigned int new_target,
                                      unsigned int src_tar_vec_pos)
{
  priority--;
  src_tar_vec_pos--;
  new_target--;
  getVector(identifier).at(priority)->editTarget(new_target, src_tar_vec_pos);
}

unsigned int VectorSourceManager::getTargetIDfromSource(unsigned int priority, 
                                                        unsigned int identifier,
                                                        unsigned int item)
{
  priority--;
  item--;
  return getVector(identifier).at(priority)->GetTargetID(item) + 1;
}

unsigned int VectorSourceManager::getTargetVectorLenFromSrc(unsigned int identifier,
                                                            unsigned int priority)
{
  priority--;
  return getVector(identifier).at(priority)->GetTargetVecLen();
}

void VectorSourceManager::clearAllInstances()
{
  //targets audio
  for(unsigned int index = 0; index < target_audio_vect_.size(); index++)
  {
    delete target_audio_vect_.at(index);
  }
  target_audio_vect_.clear();
  
  //targets video
  for(unsigned int index = 0; index < target_video_vect_.size(); index++)
  {
    delete target_video_vect_.at(index);
  }
  target_video_vect_.clear();
  
  //targets subtitles
  for(unsigned int index = 0; index < target_sub_vect_.size(); index++)
  {
    delete target_sub_vect_.at(index);
  }
  target_sub_vect_.clear();
  
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