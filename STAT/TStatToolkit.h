#ifndef TSTATTOOLKIT_H
#define TSTATTOOLKIT_H
/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

//
// some utilities which do net exist in the standard ROOT
//
/// \file TStatToolkit.h
/// \class TStatToolkit
/// \brief Summary of statistics functions

#include "TObject.h"
#include "TVectorD.h"
#include "TMatrixD.h"
//#include "TGraph2D.h"
//#include "TGraph.h"

class TH1F;
class TH1;
class TH2;
class TH3;
class TString;
class TTree;
class TGraph;
class TGraph2D;
class TCanvas;
class TMultiGraph; 
class TGraphErrors; 
class TObjString;
class TCut;
class TTreeSRedirector;


class TStatToolkit : public TObject
{
 public:
  enum TStatType {kEntries, kSum, kMean, kRMS, kMedian, kLTM, kLTMRMS}; 
  enum ENormType {kL1, kL2, kLp, kMax, kHamming, kNNormType };   // http://en.wikipedia.org/w/index.php?title=Norm_(mathematics)&oldid=655824636
  TStatToolkit();
  virtual ~TStatToolkit();
  //
  //
  //
  static void    EvaluateUni(Int_t nvectors, Double_t *data, Double_t &mean, Double_t &sigma, Int_t hh);
  static void    EvaluateUniExternal(Int_t nvectors, Double_t *data, Double_t &mean, Double_t &sigma, Int_t hh, Float_t externalfactor=1);
  static Int_t  Freq(Int_t n, const Int_t *inlist, Int_t *outlist, Bool_t down);    
  //
  // HISTOGRAMS TOOLS
  //
  static  void TruncatedMean(const TH1 * his, TVectorD *param, Float_t down=0, Float_t up=1.0, Bool_t verbose=kFALSE);
  static void MedianFilter(TH1 * his1D, Int_t nmedian);
  static Bool_t  LTMHisto(TH1 * his, TVectorD &param , Float_t fraction=1);
  //
  static void LTM(TH1 * his, TVectorD *param=0 , Float_t fraction=1,  Bool_t verbose=kFALSE);
  static Double_t  FitGaus(TH1* his, TVectorD *param=0, TMatrixD *matrix=0, Float_t xmin=0, Float_t xmax=0,  Bool_t verbose=kFALSE);
  static Double_t  FitGaus(Float_t *arr, Int_t nBins, Float_t xMin, Float_t xMax, TVectorD *param=0, TMatrixD *matrix=0, Bool_t verbose=kFALSE);
  static Float_t  GetCOG(const Short_t *arr, Int_t nBins, Float_t xMin, Float_t xMax, Float_t *rms=0, Float_t *sum=0);

  static TGraph2D *  MakeStat2D(TH3 * his, Int_t delta0, Int_t delta1, Int_t type);
  static TGraphErrors *  MakeStat1D(TH2 * his, Int_t deltaBin, Double_t fraction, Int_t returnType, Int_t markerStyle, Int_t markerColor);
  //
  // Graph tools
  //
  static TGraph * MakeGraphSparse(TTree * tree, const char * expr="Entry", const char * cut="1",  Int_t mstyle=25, Int_t mcolor=1, Float_t msize=-1, Float_t offset=0.0);
  static TGraphErrors * MakeGraphErrors(TTree * tree, const char * expr="Entry", const char * cut="1",  Int_t mstyle=25, Int_t mcolor=1, Float_t msize=-1, Float_t offset=0.0);

  //
  // Fitting function
  //
  static TString* FitPlane(TTree * tree, const char* drawCommand, const char* formula, const char* cuts, Double_t & chi2, Int_t &npoints,  TVectorD &fitParam, TMatrixD &covMatrix, Float_t frac=-1, Int_t start=0, Int_t stop=10000000, Bool_t fix0=kFALSE);
  static TString* FitPlaneFixed(TTree * tree, const char* drawCommand, const char* formula, const char* cuts, Double_t & chi2, Int_t &npoints,  TVectorD &fitParam, TMatrixD &covMatrix, Float_t frac=-1, Int_t start=0, Int_t stop=10000000);
  //
  //Linear fitter helper function
  //
  static TString* FitPlaneConstrain(TTree * tree, const char* drawCommand, const char* formula, const char* cuts, Double_t & chi2, Int_t &npoints,  TVectorD &fitParam, TMatrixD &covMatrix, Float_t frac=-1, Int_t start=0, Int_t stop=10000000, Double_t constrain=-1);
  static Int_t GetFitIndex(const TString fString, const TString subString);
 static TString FilterFit(const TString &input, const TString filter, TVectorD &vec, TMatrixD &covar);
 static void Update1D(Double_t delta, Double_t sigma, Int_t s1, TMatrixD &param, TMatrixD &covar);
  static void   Constrain1D(const TString &input, const TString filter, TVectorD &param, TMatrixD & covar, Double_t mean, Double_t sigma);
  static TString  MakeFitString(const TString &input, const TVectorD &param, const TMatrixD & covar, Bool_t verbose=kFALSE);
  //
  // TTree function for the trending
  //
  static Int_t  MakeStatAlias(TTree * tree, const char * expr, const char * cut, const char * alias);
  static Int_t  SetStatusAlias(TTree * tree, const char * expr, const char * cut, const char * alias);
  static TMultiGraph*  MakeStatusMultGr(TTree * tree, const char * expr, const char * cut, const char * alias, Int_t igr=0);  
  static void  AddStatusPad(TCanvas* c1, Float_t padratio, Float_t bottommargin);
  static void  DrawStatusGraphs(TObjArray* oaMultGr);
  static TTree*  WriteStatusToTree(TObject* oStatusGr);
  static TMultiGraph*  MakeStatusLines(TTree * tree, const char * expr, const char * cut, const char * alias);
  static void  MakeSummaryTree(TTree* treeIn, TTreeSRedirector *pcstream, TObjString& sumID, TCut &selection);
  static Double_t GetDefaultStat(TTree * tree, const char * var, const char * selection, TStatType statType);
  //
  // norm (distance) functions
  //
  /// Collect all variables from the last draw in one array
  /// It is assumed that the Draw function of the TTree was called before
  /// if e.g. Draw("v1:v2:v3") had been called, then values will contain
  /// the concatenated array of the values from v1,v2 and v3
  /// \param[in]  tree   input tree
  /// \param[out] values array in which to summarise all 'drawn' values
  static void     CombineArray(TTree *tree, TVectorD &values);
  /// Calculate the distance of the elements in values using a certain norm
  /// \param[in] values array with input values
  /// \param[in] normType normalisation to use
  /// \param[in] pvalue the p value for the p-type norm, ignored for all other norms
  /// \return           calculated distance
  static Double_t GetDistance(const TVectorD &values, const ENormType normType, const Int_t pvalue=1);
  /// Calculate the distance of the elements in values using a certain norm
  /// \sa GetDistance()
  static Double_t GetDistance(const Int_t size, const Double_t *values, const ENormType normType, const Int_t pvalue=1);
  /// Calculate the distance of the values selecte in tree->Draw(var, selection)
  /// If var contains more than one variable (separated by ':' as usual) the arrays
  /// are concatenated
  /// \param[in] tree      input tree
  /// \param[in] var       variable expression for the tree->Draw()
  /// \param[in] selection selection for the tree->Draw()
  /// \param[in] normType  norm to use for calculating the point distances
  /// \param[in] pvalue    p-value for the p-norm (ignored for other norm types
  /// \return              calculated distnace
  static Double_t GetDistance(TTree * tree, const char * var, const char * selection, const ENormType normType, const Int_t pvalue=1);
  //
  // TTree function for robust draw
  //
  static TH1* DrawHistogram(TTree * tree, const char* drawCommand, const char* cuts = "1", const char* hname = "histo", const char* htitle = "histo", Int_t nsigma = 4, Float_t fraction = 0.75);
  //
  // TestFunctions:
  //
 static  void TestGausFit(Int_t nhistos=5000);
 static  void CheckTreeAliases(TTree * tree, Int_t ncheck);
 ClassDef(TStatToolkit,0) // Various mathematical tools for physics analysis - which are not included in ROOT TMath
 
};
#endif
