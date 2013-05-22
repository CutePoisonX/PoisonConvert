/* 
 * File:   VectorManager.h
 * Author: CutePoisonX
 *
 * Created on 03. Februar 2013, 23:28
 */

#ifndef VECTORSOURCEMANAGER_H
#define	VECTORSOURCEMANAGER_H

#include "VectorManager.h"
#include <string>

using namespace std;

class VectorSourceManager : public VectorManager{
public:
  
                VectorSourceManager();
                VectorSourceManager(const VectorSourceManager& orig);
  virtual      ~VectorSourceManager();
  
  int           saveToVector(string& param1, string& param2, string& param3,
                             string& param4, string& param5,
                             unsigned int priority, unsigned int identifier);
  int           deleteFromVector(unsigned int priority, unsigned int identifier);
  int           editPreference(unsigned int priority, unsigned int identifier,
                               unsigned int param_nr, string& new_param);
  int           moveElement(unsigned int priority, unsigned int new_priority, 
                            unsigned int identifier);
  int           linkNewTarget(unsigned int target_id, unsigned int identifier,
                              unsigned int priority);
  int           deleteAllTarget(unsigned int priority, unsigned int identifier,
                                unsigned int target_id);
  int           editTarget(unsigned int priority, unsigned int identifier,
                           unsigned int new_target, unsigned int src_tar_vec_pos);
  unsigned int  getTargetIDfromSource(unsigned int priority, unsigned int identifier,
                                      unsigned int item);

  unsigned int  getTargetVectorLenFromSrc(unsigned int identifier, unsigned int priority);
  void          clearAllInstances();
  
private:
  
  //----------------------------------------------------------------------------
  vector<StreamPreference*> target_audio_vect_;
  vector<StreamPreference*> target_sub_vect_;
  vector<StreamPreference*> target_video_vect_;
  //----------------------------------------------------------------------------
  
  vector<StreamPreference*>& getVector(unsigned int identifier);
};

#endif	/* VECTORMANAGER_H */

