/// \file B2TrackerSD.cc
/// \brief Implementation of the B2TrackerSD class

#include "B2TrackerSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include "B2RunAction.hh"
#include "G4RunManager.hh"

#include "TRandom.h"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B2TrackerSD::B2TrackerSD(const G4String& name,
                         const G4String& hitsCollectionName)
  : G4VSensitiveDetector(name),
    fHitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B2TrackerSD::~B2TrackerSD()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2TrackerSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection

  fHitsCollection
    = new B2TrackerHitsCollection(SensitiveDetectorName, collectionName[0]);

  // Add this collection in hce

  G4int hcID
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcID, fHitsCollection);

  // Initialize Hit No.
  nHits = 0;
  pastPadID = -1;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool B2TrackerSD::ProcessHits(G4Step* aStep,
                                G4TouchableHistory*)
{


  // energy deposit
  G4double edep = aStep->GetTotalEnergyDeposit();

  // Particle name
  G4ParticleDefinition* pd = aStep->GetTrack()->GetDefinition();
  G4String particlename = pd->GetParticleName();

//  if (edep==0.) return false; // θ=58°とかでHitが抜けるのはこいつのせいだった！(2021.12.07)

  B2TrackerHit* newHit = new B2TrackerHit();

  newHit->SetTrackID(aStep->GetTrack()->GetTrackID());
  newHit->SetChamberNb(aStep->GetPreStepPoint()->GetTouchableHandle()
                       ->GetCopyNumber());
  newHit->SetEdep(edep);
  newHit->SetPos(aStep->GetPostStepPoint()->GetPosition());

  newHit->SetMoment(aStep->GetPostStepPoint()->GetMomentumDirection());

  fHitsCollection->insert(newHit);

  // newHit->Print(); //もともとはコメント

  // 位置座標を出力？
  /*G4ParticleDefinition* pd = aStep->GetTrack()->GetDefinition();
  G4String particlename = pd->GetParticleName();
  const G4StepPoint* preStepPoint = aStep->GetPreStepPoint();

  G4ThreeVector pos = preStepPoint->GetPosition(); */
  /*std::cout << std::endl
           << "$$$$$$$$$$$$$$$$$ This is given positions $$$$$$$$$$$$$$$" << std::endl
           // << "pos = "  << pos << std::endl
           << "newHit->GetTrackID() = " << newHit->GetTrackID() << std::endl
           << "newHit->GetChamberNb() = " << newHit->GetChamberNb() << std::endl
           << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << std::endl;
  */

  PosX = newHit->GetPos().getX() ;
  PosY = newHit->GetPos().getY() ;
  PosZ = newHit->GetPos().getZ() ;
  //MomentX = newHit->GetMoment().getX() ;
  //MomentY = newHit->GetMoment().getY() ;
  //MomentZ = newHit->GetMoment().getZ() ;
  PadID = newHit->GetChamberNb();

  // std::cout << "PadNo = " << PadID << std::endl;

  // Set Position Resolution
  G4double Res = 700.0 * um;
  G4double ResXY = 200.0 * um;

  // Smeared data with a position resolution along drift direction
  PosXS = gRandom->Gaus(newHit->GetPos().getX(), ResXY);
  PosYS = gRandom->Gaus(newHit->GetPos().getY(), ResXY);
  PosZS = gRandom->Gaus(newHit->GetPos().getZ(), Res) ; // Smear drift direction
  // MomentXS = newHit->GetMoment().getX() ;
  // MomentYS = newHit->GetMoment().getY() ;
  // MomentZS = newHit->GetMoment().getZ() ;

  // std::cout << "paticleName = " << particlename << std::endl;

  // TTree *t1 = runAction->GetTTreePtr();

  // G4int evtn = 6;
  B2RunAction* runAction = (B2RunAction*) G4RunManager::GetRunManager()->GetUserRunAction();
  // sumEntry = runAction->GetCountEntry();

  if (strcmp(particlename, "pi-") == 0) {
    if (nHits < 9) {
      // if (PadID < pastPadID){pastPadID = 10;}
      if (PadID > pastPadID) {
        // 運動量小さい飛跡が螺旋を数周描いてgenfitでエラーが出るため条件文を!=から>へ変更(2020.12.14)
        // runAction->CheckParticleTrue();
        nHits += 1;

        valN = nHits;
        runAction->SettnHits(&valN);
        valN = PadID;
        runAction->SettPadID(&valN);

        //G4double tEProton;
        //std::cout << "runAction -> SettPosX(&valN, &val); " << std::endl;
        val = PosX ;
        runAction -> SettPosX(&val);
        // std::cout << "runAction -> SettPosX(&valN, &val); " << std::endl;
        // runAction->ShowPosX();
        val = PosY ;
        runAction -> SettPosY(&val);
        val = PosZ ;
        runAction -> SettPosZ(&val);
        /*val = MomentX ;
        runAction -> SettMomentX(&val);
        val = MomentY ;
        runAction -> SettMomentY(&val);
        val = MomentZ ;
        runAction -> SettMomentZ(&val);*/

        val = PosXS ;
        runAction -> SettPosXS(&val);
        val = PosYS ;
        runAction -> SettPosYS(&val);
        val = PosZS ;
        runAction -> SettPosZS(&val);
        // val = Res;
        // runAction->SettRes(&val);

        //G4cout << "PosZ =  " << PosZ << G4endl;
        //G4cout << "val =  " << val << G4endl;
        // G4cout << "tPosX =  " << tPosX << G4endl;
        // G4cout << "newHit->GetPos().getZ() =  " << newHit->GetPos().getZ() << G4endl;

        // t1->Fill();

        /* TTree *t2 = runAction->GetTTreePtrS();

        val = PosXS ;
        runAction -> SettPosXS(&val);
        val = PosYS ;
        runAction -> SettPosYS(&val);
        val = PosZS ;
        runAction -> SettPosZS(&val);
        val = MomentXS ;
        runAction -> SettMomentXS(&val);
        val = MomentYS ;
        runAction -> SettMomentYS(&val);
        val = MomentZS ;
        runAction -> SettMomentZS(&val);
        val = Res;
        runAction->SettRes(&val);

        G4cout << "PosZS =  " << PosZS << G4endl;
        */
        // G4cout << "Res =  " << Res << G4endl;
        // G4cout << "gRandom->Gaus(newHit->GetPos().getZ(), Res) =  " << gRandom->Gaus(newHit->GetPos().getZ(), Res) << G4endl;
        // ここで出力すると乱数振り直し

        //runAction->ShowPosX();
        // t2->Fill();

        pastPadID = PadID;
      } // PadID != pastPadID
    } // nHits < 9
  } // particle = pi-

  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2TrackerSD::EndOfEvent(G4HCofThisEvent*)
{
  if (verboseLevel > 1) {
    G4int nofHits = fHitsCollection->entries();
    G4cout << G4endl
           << "-------->Hits Collection: in this event they are " << nofHits
           << " hits in the tracker chambers: " << G4endl;
    for (G4int i = 0; i < nofHits; i++)(*fHitsCollection)[i]->Print();
  }

  G4int maxEntry = 51000;
  G4int sumEntry;
  B2RunAction* runAction1 = (B2RunAction*) G4RunManager::GetRunManager()->GetUserRunAction();
  sumEntry = runAction1->GetCountEntry();
  if (runAction1->GettnHits() > 4) {
    if (sumEntry < maxEntry) {
      TTree* t1 = runAction1->GetTTreePtr();
      t1->Fill();
      runAction1->AddCountEntry();
    }
  }
  runAction1->CleartPosX();
  runAction1->CleartPosY();
  runAction1->CleartPosZ();
  runAction1->CleartPosXS();
  runAction1->CleartPosYS();
  runAction1->CleartPosZS();
  runAction1->CleartPadID();
  runAction1->CheckParticleTrue();
  runAction1->CleartnHits();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
