//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B2RunAction.hh
/// \brief Definition of the B2RunAction class

#ifndef B2RunAction_h
#define B2RunAction_h 1

#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4UserRunAction.hh"
#include "globals.hh"
#include "TFile.h"
#include "TTree.h"
#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;

/// Run action class

class B2RunAction : public G4UserRunAction {
public:
  B2RunAction();
  virtual ~B2RunAction();

  virtual void BeginOfRunAction(const G4Run* run);
  virtual void EndOfRunAction(const G4Run* run);

  inline TFile* GetOutFilePtr() const {return fOutfile;}
  inline TTree* GetTTreePtr() const {return t1;}

  void SettPosX(G4double* val) {tPosX.push_back(*val);}
  void SettPosY(G4double* val) {tPosY.push_back(*val);}
  void SettPosZ(G4double* val) {tPosZ.push_back(*val);}

  void SettPosXS(G4double* val) {tPosXS.push_back(*val);}
  void SettPosYS(G4double* val) {tPosYS.push_back(*val);}
  void SettPosZS(G4double* val) {tPosZS.push_back(*val);}
  void SettPadID(G4int* valN) {tPadID.push_back(*valN);}
  void SettnHits(G4int* valN) {tnHits = *valN;}

  void CleartPosX() {tPosX.clear();}
  void CleartPosY() {tPosY.clear();}
  void CleartPosZ() {tPosZ.clear();}
  void CleartPosXS() {tPosXS.clear();}
  void CleartPosYS() {tPosYS.clear();}
  void CleartPosZS() {tPosZS.clear();}
  void CleartPadID() {tPadID.clear();}
  void CleartnHits() {tnHits = 0;}

  int GettnHits() {return tnHits;}

  void CheckParticleFalse() {tCheckParticle = false;}
  void CheckParticleTrue() {tCheckParticle = true;}
  bool GetCheckParticle() {return tCheckParticle;}

  void InitCountEntry() {tCountEntry = 0;}
  void AddCountEntry() {tCountEntry += 1;}
  int GetCountEntry() {return tCountEntry;}

  void SetargvCtarget(char* argvIN)
  {
    strcpy(argvCtarget, argvIN);
    targetString = std::string(argvCtarget);
  }
  void SetargvCmom(char* argvIN) { strcpy(argvCmom, argvIN); }
  void SetargvCtheta(char* argvIN) { strcpy(argvCtheta, argvIN); }
  void SetargvCphi(char* argvIN) { strcpy(argvCphi, argvIN); }

  char* GetargvCtarget() { return argvCtarget; }
  char* GetargvCmom() { return argvCmom; }
  char* GetargvCtheta() { return argvCtheta; }
  char* GetargvCphi() { return argvCphi;  }
  std::string GettargetString() {return targetString;}


private:
  /*
   *  FOR ROOT OUTPUT.
   */
  TFile* fOutfile;  // output rootfile pointer
  TTree* t1;

  std::vector<Int_t>  tPadID;
  std::vector<Double_t> tPosX, tPosY, tPosZ;
  std::vector<Double_t> tPosXS, tPosYS, tPosZS;
  G4int tnHits;
  G4int tCountEntry;

  bool tCheckParticle;

  char argvCtarget[10];
  char argvCmom[10];
  char argvCtheta[10];
  char argvCphi[10];
  std::string targetString;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
