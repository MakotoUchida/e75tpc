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
/// \file TPCDetectorMessenger.cc
/// \brief Implementation of the TPCDetectorMessenger class

#include "TPCDetectorMessenger.h"
#include "TPCDetectorConstruction.h"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

using namespace E75;
namespace E75 {
  TPCDetectorMessenger::TPCDetectorMessenger(TPCDetectorConstruction* Det)
    : G4UImessenger(),
      fDetectorConstruction(Det)
  {
    fTPCDirectory = new G4UIdirectory("/TPC/");
    fTPCDirectory->SetGuidance("UI commands specific to this example.");

    fDetDirectory = new G4UIdirectory("/TPC/det/");
    fDetDirectory->SetGuidance("Detector construction control");

    fTargMatCmd = new G4UIcmdWithAString("/TPC/det/setTargetMaterial", this);
    fTargMatCmd->SetGuidance("Select Material of the Target.");
    fTargMatCmd->SetParameterName("choice", false);
    fTargMatCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fChamMatCmd = new G4UIcmdWithAString("/TPC/det/setChamberMaterial", this);
    fChamMatCmd->SetGuidance("Select Material of the Chamber.");
    fChamMatCmd->SetParameterName("choice", false);
    fChamMatCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fStepMaxCmd = new G4UIcmdWithADoubleAndUnit("/TPC/det/stepMax", this);
    fStepMaxCmd->SetGuidance("Define a step max");
    fStepMaxCmd->SetParameterName("stepMax", false);
    fStepMaxCmd->SetUnitCategory("Length");
    fStepMaxCmd->AvailableForStates(G4State_Idle);
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  TPCDetectorMessenger::~TPCDetectorMessenger()
  {
    delete fTargMatCmd;
    delete fChamMatCmd;
    delete fStepMaxCmd;
    delete fTPCDirectory;
    delete fDetDirectory;
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void TPCDetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
  {
    if (command == fTargMatCmd)
    { fDetectorConstruction->SetTargetMaterial(newValue);}

    if (command == fChamMatCmd)
    { fDetectorConstruction->SetChamberMaterial(newValue);}

    if (command == fStepMaxCmd) {
      fDetectorConstruction
      ->SetMaxStep(fStepMaxCmd->GetNewDoubleValue(newValue));
    }
  }

}
