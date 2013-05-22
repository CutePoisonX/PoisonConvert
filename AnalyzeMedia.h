/* 
 * File:   AnalyzeMedia.h
 * Author: CutePoisonX
 *
 * Created on 10. Februar 2013, 10:14
 */

#ifndef ANALYZEMEDIA_H
#define	ANALYZEMEDIA_H

#include "VectorManager.h"

class AnalyzeMedia : public VectorManager {
public:
                AnalyzeMedia();
                AnalyzeMedia(const AnalyzeMedia& orig);
  virtual      ~AnalyzeMedia();
  
  int           saveToVector(string& param1, string& param2, string& param3,
                             string& param4, string& param5, unsigned int priority,
                             unsigned int identifier);
  void          clearEverything();
  void          clearAllInstances();
  
private:
  
  vector<StreamPreference*>& getVector(unsigned int identifier);
};

#endif	/* ANALYZEMEDIA_H */

