#ifndef AliHMPIDReconstructor_h
#define AliHMPIDReconstructor_h
/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */
//.
// HMPID base class to reconstruct an event
//.
#include <AliReconstructor.h>       //base class
#include "AliHMPIDTracker.h"         //CreateTracker()
#include <TMatrixF.h>               //UseDig()
#include <TClonesArray.h>           //UseDig()
class AliRawReader;                 //Reconstruct() with raw data   
class AliHMPIDDigit;                 //Dig2Clu(), UseDig()
class AliHMPIDCluster;               //Dig2Clu()

class AliHMPIDReconstructor: public AliReconstructor 
{
public:
           AliHMPIDReconstructor();              
  virtual ~AliHMPIDReconstructor()                                  {delete fDig;delete fClu;}//dtor  
//framework part  
  AliTracker*  CreateTracker         (AliRunLoader*                      )const{return new AliHMPIDTracker;}            //from AliReconstructor for clusters->PID
  void         ConvertDigits         (AliRawReader *pRR, TTree *pDigTree) const;                                        //from AliReconstruction for raw->digit
  Bool_t       HasDigitConversion()   const {return kTRUE;}                                                             //HMPID digits converted with ConvertDigits 
  void         Reconstruct           (TTree* digitsTree, TTree* clustersTree) const;                                    //from AliReconstruction for digit->cluster
  void         Reconstruct           (AliRunLoader *pAL,AliRawReader* pRR)const;                                        //from AliReconstruction for raw->cluster with Digits on fly
  Bool_t       HasLocalReconstruction() const {return kTRUE;}                                                           // HMPID has local reconstruction algorithm
  void         FillESD               (AliRunLoader* pAL,AliESD *pESD)const;                                             //calculate pid for HMPID
  
  using AliReconstructor::FillESD;                                                                                      //
  using AliReconstructor::Reconstruct;                                                                                  // 

  //private part  
  static        void           Dig2Clu (TObjArray *pDigLst,TObjArray *pCluLst,Bool_t isUnfold=kTRUE                      );//digits->clusters
  static        void           FormClu (AliHMPIDCluster *pClu,AliHMPIDDigit *pDig,TClonesArray *pDigLst,TMatrixF *pPadMap);//cluster formation recursive algorithm
  static inline AliHMPIDDigit* UseDig  (Int_t padX,Int_t padY,                    TClonesArray *pDigLst,TMatrixF *pDigMap);//use this pad's digit to form a cluster

  protected:
  TObjArray *fDig;                     // tmp list of digits
  TObjArray *fClu;                     // tmp list of clusters
  ClassDef(AliHMPIDReconstructor, 0)   //class for the HMPID reconstruction
};

//__________________________________________________________________________________________________
AliHMPIDDigit* AliHMPIDReconstructor::UseDig(Int_t padX,Int_t padY,TClonesArray *pDigLst,TMatrixF *pPadMap)
{
//Digit map contains a matrix if digit numbers.
//Main operation in forming initial cluster is done here. Requested digit pointer is returned and this digit marked as taken.
//Arguments: padX,padY - pad number
//           pDigLst   - list of digits for one sector
//           pDigMap   - map of those digits
//  Returns: pointer to digit if not yet used or 0 if used
  Int_t iDig=(Int_t)(*pPadMap)(padX,padY);(*pPadMap)(padX,padY)=-1;//take digit number from the map and reset this map cell to -1
  if(iDig!=-1)    return (AliHMPIDDigit*)pDigLst->At(iDig);        //digit pointer
  else            return 0;
}

#endif
