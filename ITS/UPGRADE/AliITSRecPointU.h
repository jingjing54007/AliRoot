#ifndef ALIITSRECPOINTU_H
#define ALIITSRECPOINTU_H
/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */


/////////////////////////////////////////////////////////////////
//  Class to set the local coordinates in ITS Upgrade recpoint //
/////////////////////////////////////////////////////////////////

#include <AliITSRecPoint.h>


class AliITSRecPointU : public AliITSRecPoint {

public :
  AliITSRecPointU();
  virtual ~AliITSRecPointU() {}; // distructor
  AliITSRecPointU(const AliITSRecPointU& pt);
  AliITSRecPointU& operator=(const AliITSRecPointU &source);

  void SetLocalCoord(Float_t x, Float_t z) {fXloc=x; fZloc=z;}
  void SetModule(Int_t i){fModule=i;} 
  void SetNTracksIdMC(Int_t nLabels) {fNTracksIdMC = nLabels;}
  void AddTrackID(Int_t tid); 

  Int_t GetModule(){return fModule;}
  Int_t GetNTracksIdMC() const {return fNTracksIdMC;}
  Int_t GetTrackID(Int_t ipart) {if(ipart<0 || ipart >=kMaxLab) return -1; else return fTrackIdMC[ipart];}

  virtual void Print(Option_t* option = "") const;

 protected:
  enum {kMaxLab=24}; // maximum number of MC labels associated to the cluster

  Int_t fModule;
  Int_t fNTracksIdMC;     // total number of associated MC labels (could be more than 3!)
  Int_t fTrackIdMC[kMaxLab];  // MC track labels 

  ClassDef(AliITSRecPointU,1)  // AliITSRecPointU class

};
#endif
