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
/// \file B2PrimaryGeneratorAction.cc
/// \brief Implementation of the B2PrimaryGeneratorAction class

#include "B2PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

#include "Randomize.hh"
#include "G4RandomDirection.hh"

#include "GlobalFileName.hh"
#include <cmath>
#include <random>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B2PrimaryGeneratorAction::B2PrimaryGeneratorAction()
  : G4VUserPrimaryGeneratorAction()
{
  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);

  srand(GlobalGetrandamIGlo());
  double randamNormal = 1. + (rand() * (10.0 - 1.0 + 1.0) / (1.0 + RAND_MAX));
  double randam =  0.00001 * randamNormal ;
  std::cout << "randam = " << randam << std::endl;
  const double PI = 3.14159265358979323846;
  double momIn = GlobalGetmomDGlo();
  double thetaIn = GlobalGetthetaDGlo();
  double phiIn = GlobalGetphiDGlo();

  double thetaInRad = (thetaIn / 180.) * PI;
  double phiInRad = (phiIn / 180.) * PI;

  double xIn = sin(thetaInRad) * cos(phiInRad);
  // double xIn = sin(thetaInRad) *cos(phiInRad)+randam;
  double yIn = sin(thetaInRad) * sin(phiInRad);
  double zIn = cos(thetaInRad);

  std::cout << "@@@@@@@@@@@@ Primary Action Information@@@@@@@@@@@@@@" << std::endl;
  std::cout << "mom = " << momIn << "  [MeV/c]" << std::endl;
  std::cout << "thetaIn = " << thetaIn << "  [°]" << std::endl;
  std::cout << "phiIn = " << phiIn << "  [°]" << std::endl;
  std::cout << "(xIn, yIn, zIn) = ( " << xIn << " , " << yIn << " , " << zIn << " ) " << std::endl;
  std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;

  // default particle kinematic

  G4ParticleDefinition* particleDefinition
    = G4ParticleTable::GetParticleTable()->FindParticle("pi-");

  fParticleGun->SetParticleDefinition(particleDefinition);
  // デフォルトの粒子の入射方向
  // fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(xIn, yIn, zIn));
  // 大きさが1のG4ThreeVectorを4πにわたって一様に生成
  // fParticleGun->SetParticleMomentumDirection(G4RandomDirection());
  // fParticleGun->SetParticleEnergy(133*MeV);
  fParticleGun->SetParticleMomentum(momIn * MeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B2PrimaryGeneratorAction::~B2PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // This function is called at the begining of event

  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get world volume
  // from G4LogicalVolumeStore.

  G4double worldZHalfLength = 0;
  G4LogicalVolume* worldLV
    = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
  G4Box* worldBox = NULL;
  if (worldLV) worldBox = dynamic_cast<G4Box*>(worldLV->GetSolid());
  if (worldBox) worldZHalfLength = worldBox->GetZHalfLength();
  else  {
    // G4cerr << "World volume of box not found." << G4endl;
    // G4cerr << "Perhaps you have changed geometry." << G4endl;
    // G4cerr << "The gun will be place in the center." << G4endl;
  }

  fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -worldZHalfLength));

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
