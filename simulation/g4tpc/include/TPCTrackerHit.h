/// \file TPCTrackerHit.hh
/// \brief Definition of the TPCTrackerHit class

#ifndef TPCTrackerHit_h
#define TPCTrackerHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

/// Tracker hit class
///
/// It defines data members to store the trackID, chamberNb, energy deposit,
/// and position of charged particles in a selected volume:
/// - fTrackID, fChamberNB, fEdep, fPos
namespace E75 {

  class TPCTrackerHit : public G4VHit {
  public:
    TPCTrackerHit();
    TPCTrackerHit(const TPCTrackerHit&);
    virtual ~TPCTrackerHit();

    // operators
    const TPCTrackerHit& operator=(const TPCTrackerHit&);
    G4bool operator==(const TPCTrackerHit&) const;

    inline void* operator new (size_t);
    inline void  operator delete (void*);

    // methods from base class
    virtual void Draw();
    virtual void Print();

    // Set methods
    void SetTrackID(G4int track)      { fTrackID = track; };
    void SetChamberNb(G4int chamb)      { fChamberNb = chamb; };
    void SetEdep(G4double de)      { fEdep = de; };
    void SetPos(G4ThreeVector xyz) { fPos = xyz; };
    // momentum
    void SetMoment(G4ThreeVector uvw) { fMoment = uvw; };

    // Get methods
    G4int GetTrackID() const     { return fTrackID; };
    G4int GetChamberNb() const   { return fChamberNb; };
    G4double GetEdep() const     { return fEdep; };
    G4ThreeVector GetPos() const { return fPos; };
    // momentum
    G4ThreeVector GetMoment() const { return fMoment; };

  private:

    G4int         fTrackID;
    G4int         fChamberNb;
    G4double      fEdep;
    G4ThreeVector fPos;
    // momentum
    G4ThreeVector    fMoment;

  };

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  typedef G4THitsCollection<TPCTrackerHit> TPCTrackerHitsCollection;

  extern G4ThreadLocal G4Allocator<TPCTrackerHit>* TPCTrackerHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  inline void* TPCTrackerHit::operator new (size_t)
  {
    if (!TPCTrackerHitAllocator)
      TPCTrackerHitAllocator = new G4Allocator<TPCTrackerHit>;
    return (void*) TPCTrackerHitAllocator->MallocSingle();
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  inline void TPCTrackerHit::operator delete (void* hit)
  {
    TPCTrackerHitAllocator->FreeSingle((TPCTrackerHit*) hit);
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
}
#endif
