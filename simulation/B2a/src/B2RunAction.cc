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
/// \file B2RunAction.cc
/// \brief Implementation of the B2RunAction class

#include "B2RunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"

#include "TRandom.h"

#include <string.h>
#include <stdlib.h>
#include <string>

// #include "E75FileName.hh"

#include "GlobalFileName.hh"

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B2RunAction::B2RunAction()
  : G4UserRunAction()
{
  // set printing event number per each 100 events
  G4RunManager::GetRunManager()->SetPrintProgress(1000);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B2RunAction::~B2RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2RunAction::BeginOfRunAction(const G4Run*)
{
  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // Set random number
  gRandom->SetSeed(22);

  // ~~~~~~~~~~~~~~~~~~SetfilenameROOT~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  std::string fileInput = GetfilenameGlo();
  std::string filenameROOT = fileInput;
  std::cout << "aaaa filenameROOT = " << filenameROOT << std::endl;
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


  fOutfile = new TFile(filenameROOT.c_str(), "RECREATE"); // output file
  t1 = new TTree("treeHits", "Hits"); // define the Tree
  t1->Branch("nHits", &tnHits, "tnHits/I");
  t1->Branch("PadID", &tPadID);
  t1->Branch("PositionX", &tPosX);
  t1->Branch("PositionY", &tPosY);
  t1->Branch("PositionZ", &tPosZ);
  t1->Branch("PositionXS", &tPosXS);
  t1->Branch("PositionYS", &tPosYS);
  t1->Branch("PositionZS", &tPosZS);

  B2RunAction* runAction2 = (B2RunAction*) G4RunManager::GetRunManager()->GetUserRunAction();
  runAction2->InitCountEntry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2RunAction::EndOfRunAction(const G4Run*)
{
  fOutfile->cd();
  t1->Write();
  fOutfile->Close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
