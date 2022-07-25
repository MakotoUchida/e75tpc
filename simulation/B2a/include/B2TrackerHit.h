/// \file B2TrackerHit.hh
/// \brief Definition of the B2TrackerHit class

#ifndef B2TrackerHit_h
#define B2TrackerHit_h 1

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

class B2TrackerHit : public G4VHit {
public:
  B2TrackerHit();
  B2TrackerHit(const B2TrackerHit&);
  virtual ~B2TrackerHit();

  // operators
  const B2TrackerHit& operator=(const B2TrackerHit&);
  G4bool operator==(const B2TrackerHit&) const;

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

typedef G4THitsCollection<B2TrackerHit> B2TrackerHitsCollection;

extern G4ThreadLocal G4Allocator<B2TrackerHit>* B2TrackerHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* B2TrackerHit::operator new (size_t)
{
  if (!B2TrackerHitAllocator)
    B2TrackerHitAllocator = new G4Allocator<B2TrackerHit>;
  return (void*) B2TrackerHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void B2TrackerHit::operator delete (void* hit)
{
  B2TrackerHitAllocator->FreeSingle((B2TrackerHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
