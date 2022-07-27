/// \file TPCTrackerHit.cc
/// \brief Implementation of the TPCTrackerHit class

#include "TPCTrackerHit.h"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>
namespace E75 {

  G4ThreadLocal G4Allocator<TPCTrackerHit>* TPCTrackerHitAllocator = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  TPCTrackerHit::TPCTrackerHit()
    : G4VHit(),
      fTrackID(-1),
      fChamberNb(-1),
      fEdep(0.),
      fPos(G4ThreeVector())
  {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  TPCTrackerHit::~TPCTrackerHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  TPCTrackerHit::TPCTrackerHit(const TPCTrackerHit& right)
    : G4VHit()
  {
    fTrackID   = right.fTrackID;
    fChamberNb = right.fChamberNb;
    fEdep      = right.fEdep;
    fPos       = right.fPos;
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  const TPCTrackerHit& TPCTrackerHit::operator=(const TPCTrackerHit& right)
  {
    fTrackID   = right.fTrackID;
    fChamberNb = right.fChamberNb;
    fEdep      = right.fEdep;
    fPos       = right.fPos;

    return *this;
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  G4bool TPCTrackerHit::operator==(const TPCTrackerHit& right) const
  {
    return (this == &right) ? true : false;
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void TPCTrackerHit::Draw()
  {
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if (pVVisManager) {
      G4Circle circle(fPos);
      circle.SetScreenSize(4.);
      circle.SetFillStyle(G4Circle::filled);
      G4Colour colour(1., 0., 0.);
      G4VisAttributes attribs(colour);
      circle.SetVisAttributes(attribs);
      pVVisManager->Draw(circle);
    }
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void TPCTrackerHit::Print()
  {
    G4cout
        << "  trackID: " << fTrackID << " chamberNb: " << fChamberNb
        << "Edep: "
        << std::setw(7) << G4BestUnit(fEdep, "Energy")
        << " Position: "
        << std::setw(7) << G4BestUnit(fPos, "Length")
        << G4endl;
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
}
