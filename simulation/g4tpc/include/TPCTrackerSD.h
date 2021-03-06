/// \file TPCTrackerSD.hh
/// \brief Definition of the TPCTrackerSD class

#ifndef TPCTrackerSD_h
#define TPCTrackerSD_h 1

#include "G4VSensitiveDetector.hh"
#include "TPCTrackerHit.h"
#include <vector>

class G4Step;
class G4HCofThisEvent;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/// TPCTracker sensitive detector class
///
/// The hits are accounted in hits in ProcessHits() function which is called
/// by Geant4 kernel at each step. A hit is created with each step with non zero
/// energy deposit.
namespace E75 {

  class TPCTrackerSD : public G4VSensitiveDetector {
  public:
    TPCTrackerSD(const G4String& name,
                 const G4String& hitsCollectionName);
    virtual ~TPCTrackerSD();

    // methods from base class
    virtual void   Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);

  private:
    TPCTrackerHitsCollection* fHitsCollection;

    // FOR ROOT
    G4double PosX, PosY, PosZ;
    // G4double MomentX, MomentY, MomentZ;
    G4double PosXS, PosYS, PosZS;
    G4int PadID;
    G4int nHits;
    G4int pastPadID;

    G4double val;
    G4int valN;

    //std::vetor<Double_t> PosX, PosY, PosZ;
    //std::vetor<Double_t> PosZS;
    //std::vetor<Int_t> PadID;
    // G4double PosXS, PosYS, PosZS;
    // G4double MomentXS, MomentYS, MomentZS;
  };

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
}
#endif
