/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

#include "TParticle.h"

#include "AliRun.h"
#include "AliMC.h"
#include "AliStack.h"

//#include "AliITSU.h"
//#include "AliITSUGeomTGeo.h"
#include "AliITSMFTHit.h"

ClassImp(AliITSMFTHit)

////////////////////////////////////////////////////////////////////////
//
// At the moment the same functionality/data-members as parent AliITShit
// except the geometry transformation uses AliITSgeomTGeoUp
//
////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------
AliITSMFTHit::AliITSMFTHit() : AliHit(),
fStatus(0), // Track Status
fModule(0), // Module number
fPx(0),     // PX of particle at the point of the hit
fPy(0),     // PY of particle at the point of the hit
fPz(0),     // PZ of particle at the point of the hit
fDestep(0), // Energy deposited in the current step
fTof(0),    // Time of flight at the point of the hit
fStatus0(0),// Track Status of Starting point
fx0(0),     // Starting point of this step
fy0(0),     // Starting point of this step
fz0(0),     // Starting point of this step
ft0(0)     // Starting point of this step
{
}

//----------------------------------------------------------------------
AliITSMFTHit::AliITSMFTHit(Int_t shunt,Int_t track,Int_t *vol,Float_t edep,Float_t tof,
                           TLorentzVector &x,TLorentzVector &x0,TLorentzVector &p)
: AliHit(shunt,track),
fStatus(vol[3]), // Track Status
fModule(vol[0]),  // Module number
fPx(p.Px()),     // PX of particle at the point of the hit
fPy(p.Py()),     // PY of particle at the point of the hit
fPz(p.Pz()),     // PZ of particle at the point of the hit
fDestep(edep), // Energy deposited in the current step
fTof(tof),    // Time of flight at the point of the hit
fStatus0(vol[4]),// Track Status of Starting point
fx0(x0.X()),     // Starting point of this step
fy0(x0.Y()),     // Starting point of this step
fz0(x0.Z()),     // Starting point of this step
ft0(x0.T())     // Starting point of this step
{
    // ct-r
    SetPosition(x);
}

//______________________________________________________________________
AliITSMFTHit::AliITSMFTHit(Int_t shunt, Int_t track, Int_t *vol, Float_t *hits)
: AliHit(shunt, track),
fStatus(vol[3]), // Track Status
fModule(vol[0]),  // Module number
fPx(hits[3]),     // PX of particle at the point of the hit
fPy(hits[4]),     // PY of particle at the point of the hit
fPz(hits[5]),     // PZ of particle at the point of the hit
fDestep(hits[6]), // Energy deposited in the current step
fTof(hits[7]),    // Time of flight at the point of the hit
fStatus0(vol[4]),// Track Status of Starting point
fx0(hits[8]),     // Starting point of this step
fy0(hits[9]),     // Starting point of this step
fz0(hits[10]),     // Starting point of this step
ft0(hits[11])     // Starting point of this step
{
    // c-tor
    fX          = hits[0];  // Track X global position
    fY          = hits[1];  // Track Y global position
    fZ          = hits[2];  // Track Z global position
}

//______________________________________________________________________
AliITSMFTHit::AliITSMFTHit(const AliITSMFTHit &h)
: AliHit(h),
fStatus(h.fStatus), // Track Status
fModule(h.fModule),  // Module number
fPx(h.fPx),     // PX of particle at the point of the hit
fPy(h.fPy),     // PY of particle at the point of the hit
fPz(h.fPz),     // PZ of particle at the point of the hit
fDestep(h.fDestep), // Energy deposited in the current step
fTof(h.fTof),    // Time of flight at the point of the hit
fStatus0(h.fStatus0),// Track Status of Starting point
fx0(h.fx0),     // Starting point of this step
fy0(h.fy0),     // Starting point of this step
fz0(h.fz0),     // Starting point of this step
ft0(h.ft0)     // Starting point of this step
{
    // cp c-tor
    if(this == &h) return;
    return;
}

//______________________________________________________________________
AliITSMFTHit& AliITSMFTHit::operator=(const AliITSMFTHit &h)
{
    // The standard = operator
    if(this == &h) return *this;
    this->fStatus  = h.fStatus;
    this->fModule  = h.fModule;
    this->fPx      = h.fPx;
    this->fPy      = h.fPy;
    this->fPz      = h.fPz;
    this->fDestep  = h.fDestep;
    this->fTof     = h.fTof;
    this->fStatus0 = h.fStatus0;
    this->fx0      = h.fx0;
    this->fy0      = h.fy0;
    this->fz0      = h.fz0;
    this->ft0      = h.ft0;
    return *this;
}

//______________________________________________________________________
void AliITSMFTHit::SetShunt(Int_t shunt){
    // Sets track flag based on shunt value. Code copied from
    // AliHit standar constructor.
    // Inputs:
    //   Int_t shunt    A flag to indecate what to do with track numbers
    // Outputs:
    //   none.
    // Return:
    //   none.
    Int_t primary,track,current,parent;
    TParticle *part;
    
    track = fTrack;
    if(shunt == 1) {
        primary = gAlice->GetMCApp()->GetPrimary(track);
        gAlice->GetMCApp()->Particle(primary)->SetBit(kKeepBit);
        fTrack=primary;
    }else if (shunt == 2) {
        // the "primary" particle associated to the hit is
        // the last track that has been flagged in the StepManager
        // used by PHOS to associate the hit with the decay gamma
        // rather than with the original pi0
        parent=track;
        while (1) {
            current=parent;
            part = gAlice->GetMCApp()->Particle(current);
            parent=part->GetFirstMother();
            if(parent<0 || part->TestBit(kKeepBit))
                break;
        }
        fTrack=current;
    }else {
        fTrack=track;
        gAlice->GetMCApp()->FlagTrack(fTrack);
    } // end if shunt
}

//______________________________________________________________________
void AliITSMFTHit::GetPositionL(Float_t &x,Float_t &y,Float_t &z,Float_t &tof)
{
    // Returns the position and time of flight of this hit in the local
    // coordinates of this chip, and in the units of the Monte Carlo.
    //
    x = -999;
    y = -999;
    z = -999;
    tof = -999;
    
    //
}

//______________________________________________________________________
void AliITSMFTHit::GetPositionL0(Double_t &x,Double_t &y,Double_t &z,Double_t &tof)
{
    // Returns the initial position and time of flight of this hit
    // in the local coordinates of this chip, and in the units of the
    x = -999;
    y = -999;
    z = -999;
    tof = -999;
}

//______________________________________________________________________
void AliITSMFTHit::GetChipID(Int_t &layer,Int_t &stave,Int_t &sstave, Int_t &mod,Int_t &det) const
{
    // Returns the layer stave and detector number lables for this
    // ITS chip. Note: indices start from 0!
    
    layer = -999;
    stave = -999;
    sstave = -999;
    mod = -999;
    det = -999;
}

//______________________________________________________________________
Int_t AliITSMFTHit::GetLayer() const
{
    // Returns the layer. Note: indices start from 0!
    return -999;
}

//______________________________________________________________________
Int_t AliITSMFTHit::GetStave() const
{
    // Returns the stave of TS chip. Note: indices start from 0!
    return -999;
}

//______________________________________________________________________
Int_t AliITSMFTHit::GetHalfStave() const
{
    // Returns the substave of the chip. Note: indices start from 0!
    return -999;
}

//______________________________________________________________________
Int_t AliITSMFTHit::GetModule() const
{
    return -999;
}

//______________________________________________________________________
Int_t AliITSMFTHit::GetChipInModule() const // former GetDetector
{
    // Returns the detector within the module(or stave). Note: indices start from 0!
    return -999;
}

//______________________________________________________________________
void AliITSMFTHit::Print(Option_t */*option*/) const
{
    // print itself
    printf("Mod%4d Tr:%5d DE:%.2e TOF: %.3e| P:%.3f %.3f %.3f |>%.4f %.4f %.4f >%.4f %.4f %.4f\n",
           fModule,fTrack,fDestep,fTof,fPx,fPy,fPz, fx0,fy0,fz0,fX,fY,fZ);
    
}