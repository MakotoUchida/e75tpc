/// \file TPCaDetectorConstruction.cc
/// \brief Implementation of the TPCaDetectorConstruction class

#include "TPCDetectorConstruction.h"
#include "TPCDetectorMessenger.h"
#include "TPCTrackerSD.h"
#include "GlobalFileName.h"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SDManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trap.hh" // To build trapezoid
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4Polycone.hh"
#include "G4Polyhedra.hh"

#include "G4UnionSolid.hh" // To combine two Solids
#include "G4UniformMagField.hh" // to use uniformed magnetic field
#include "G4FieldManager.hh"
#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4Transportation.hh"


using namespace E75;
namespace E75 {


  G4ThreadLocal
  G4GlobalMagFieldMessenger* TPCDetectorConstruction::fMagFieldMessenger = 0;

  TPCDetectorConstruction::TPCDetectorConstruction()
    : G4VUserDetectorConstruction(),
      fNbOfChambers(0),
      fLogicTarget(NULL), fLogicChamber(NULL),
      fTargetMaterial(NULL), fChamberMaterial(NULL),
      fStepLimit(NULL),
      fCheckOverlaps(true)
  {
    fMessenger = new TPCDetectorMessenger(this);
    fNbOfChambers = 5;
    fLogicChamber = new G4LogicalVolume*[fNbOfChambers];
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  TPCDetectorConstruction::~TPCDetectorConstruction()
  {
    delete [] fLogicChamber;
    delete fStepLimit;
    delete fMessenger;
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  G4VPhysicalVolume* TPCDetectorConstruction::Construct()
  {
    // Define materials
    DefineMaterials();

    // Define volumes
    return DefineVolumes();
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void TPCDetectorConstruction::DefineMaterials()
  {
    // Material definition

    G4NistManager* nistManager = G4NistManager::Instance();

    // Air defined using NIST Manager
    nistManager->FindOrBuildMaterial("G4_AIR");

    // Lead defined using NIST Manager
    fTargetMaterial  = nistManager->FindOrBuildMaterial("G4_Pb");

    // Xenon gas defined using NIST Manager
    fChamberMaterial = nistManager->FindOrBuildMaterial("G4_Xe");

    // Print materials
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  G4VPhysicalVolume* TPCDetectorConstruction::DefineVolumes()
  {
    G4Material* air  = G4Material::GetMaterial("G4_AIR");

    // P10ガス(Ar:Ch4=9:1)の定義

    G4double atomAr  = 39.948 * CLHEP::g / CLHEP::mole ;
    G4double atomH = 1.00794 * CLHEP::g / CLHEP::mole ;
    G4double atomC = 12.0107 * CLHEP::g / CLHEP::mole ;
    G4int z, natoms, ncomponents;
    // G4double *symbol;
    G4double densityMethane = 0.000717 * CLHEP::g / CLHEP::cm3;
    G4double densityAr = 0.001784 * CLHEP::g / CLHEP::cm3;
    G4double densityP10 = (1 / 10.) * densityMethane + (9 / 10.) * densityAr ;

    G4double temperatureP10 = 300.0 * CLHEP::kelvin;
    G4double pressureP10 = 1.0 * CLHEP::atmosphere;

    G4Element* elH = new G4Element("Hydrogen", "H", z = 1., atomH);
    G4Element* elC = new G4Element("Carbon", "C", z = 12., atomC);
    G4Material* CH4 = new G4Material("Methane", densityMethane, ncomponents = 2);

    CH4 -> AddElement(elH, natoms = 4) ;
    CH4 -> AddElement(elC, natoms = 1) ;

    G4Material* gasAr = new G4Material("gasArgon", z = 18., atomAr, densityAr);

    G4Material* P10gas = new G4Material("P10gas", densityP10, ncomponents = 2,
                                        kStateGas, temperatureP10, pressureP10);
    P10gas -> AddMaterial(CH4, 0.1) ;
    P10gas -> AddMaterial(gasAr, 0.9) ;

    // 7Li標的の定義
    G4double densityLi = 0.534 * CLHEP::g / CLHEP::cm3;
    G4int izLi = 3;
    G4double atomLi = 7.016 * CLHEP::g / CLHEP::mole ;
    G4Material* Li7 = new G4Material("Lithium7", izLi, atomLi, densityLi);

    // 真空の定義
    G4double atomicNumber = 1.;
    G4double massOfMole = 1.008 * CLHEP::g / CLHEP::mole;
    G4double density = 1.e-25 * CLHEP::g / CLHEP::cm3;
    G4double temperature = 2.73 * CLHEP::kelvin;
    G4double pressure = 3.e-18 * CLHEP::pascal;
    G4Material* Vacuum =
      new G4Material("interGalactic", atomicNumber,
                     massOfMole, density, kStateGas,
                     temperature, pressure);

    // 磁場の定義
    G4FieldManager* localFieldMgr = nullptr;
    if (true) {
      G4MagneticField* magField = new G4UniformMagField(G4ThreeVector(0., 0., 2. * CLHEP::tesla));
      std::cout << "->->->----------> UNIFORM Magnetic Field On  <-----------<-<-<- " << std::endl;
      localFieldMgr = new G4FieldManager(magField) ;
    } else {
      G4MagneticField* magField = new E75MagneticField();
      std::cout << "->->->----------> MAP Magnetic Field On  <-----------<-<-<- " << std::endl;
      localFieldMgr = new G4FieldManager(magField) ;
    }
    // globalに変更(2020.12.23) -> それぞれLVの磁場のポインタをlocalFieldMgrから0に
    //G4FieldManager* globalFieldMgr =  G4TransportationManager::GetTransportationManager()->GetFieldManager();
    //globalFieldMgr->SetDetectorField(magField);
    //globalFieldMgr->CreateChordFinder(magField);

    // SolenoidのBore型のWorld作成
    G4double worldRmin = 0 * CLHEP::mm; // 円柱型のため空洞無し
    G4double worldRmax = 301.5 * CLHEP::mm; // Radius of bore
    G4double worldDz = 501.5 * CLHEP::mm; // half of depth
    G4double worldSPhi = 0 ;
    G4double worldDPhi = 2 * CLHEP::pi ;

    // 六角中型のTPC
    G4double TPCpRmax = (557.361 / 2) * CLHEP::mm;  // TPCの六角柱外接円の半径
    G4double TPCpRmin = (115.47 / 2) * CLHEP::mm;  // TPCの六角柱bore外接円の半径
    G4double TPCpDepth =  748 * CLHEP::mm; // TPC有感領域の奥行き長さ
    G4double TPCpDx1 = (TPCpRmin / 2) * CLHEP::mm;
    G4double TPCpDx2 = (TPCpRmax / 2) * CLHEP::mm;
    G4double TPCpDz = (TPCpDepth / 2) * CLHEP::mm;
    G4double TPCpTheta = 0 * CLHEP::degree;
    G4double TPCpPhi = 0 * CLHEP::degree;

    G4double TPCpDy1 = (sqrt(3) / 4) * (TPCpRmax - TPCpRmin) * CLHEP::mm;
    G4double TPCpDy2 = (sqrt(3) / 4) * (TPCpRmax - TPCpRmin) * CLHEP::mm; // the same as TPCpDy1
    G4double TPCpDx3 = (TPCpRmin / 2) * CLHEP::mm; // the same as TPCpDx1
    G4double TPCpDx4 = (TPCpRmax / 2) * CLHEP::mm; // the same as TPCpDx2
    G4double TPCpAlp1 = 0 * CLHEP::degree;
    G4double TPCpAlp2 = 0 * CLHEP::degree;
    G4double TPCcenter = TPCpRmin * (sqrt(3) / 2.0) + TPCpDy1; // 中心軸から各素片中心までの距離

    // Sizes of the principal geometrical components (solids)

    G4double chamberSpacing = 80 * CLHEP::cm; // from chamber center to center!

    G4double chamberWidth = 20.0 * CLHEP::cm; // width of the chambers
    G4double targetLength =  5.0 * CLHEP::cm; // full length of Targe

    G4double trackerLength = (fNbOfChambers + 1) * chamberSpacing;

    G4double worldLength = 1.2 * (2 * targetLength + trackerLength);

    G4double targetRadius  = 0.5 * targetLength; // Radius of Target
    targetLength = 0.5 * targetLength;           // Half length of the Target
    G4double trackerSize   = 0.5 * trackerLength; // Half length of the Tracker


    // Definitions of Solids, Logical Volumes, Physical Volumes

    // World


    G4GeometryManager::GetInstance()->SetWorldMaximumExtent(worldLength);

    G4cout << "Computed tolerance = "
           << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance() / CLHEP::mm
           << " mm" << G4endl;


    G4Tubs* worldS = new G4Tubs("World",
                                worldRmin, worldRmax, worldDz, worldSPhi, worldDPhi); // world with cylindrical shape

    G4LogicalVolume* worldLV
      = new G4LogicalVolume(
      worldS,   //its solid
      air,      //its material
      "World", //its name
      localFieldMgr); // 磁場

    /* // デフォルトのWorld
      G4Box* worldS
        = new G4Box("world",                                    //its name
                    worldLength/2,worldLength/2,worldLength/2); //its size
      G4LogicalVolume* worldLV
        = new G4LogicalVolume(
                     worldS,   //its solid
                     air,      //its material
                     "World"); //its name
      */


    //  Must place the World Physical volume unrotated at (0,0,0).
    //
    G4VPhysicalVolume* worldPV
      = new G4PVPlacement(
      0,               // no rotation
      G4ThreeVector(), // at (0,0,0)
      worldLV,         // its logical volume
      "World",         // its name
      0,               // its mother  volume
      false,           // no boolean operations
      0,               // copy number
      fCheckOverlaps); // checking overlaps

    // TPCの立体作り
    G4double phiStart = 0 * CLHEP::pi;
    G4double phiTotal = 2 * CLHEP::pi;
    G4int numSide = 6;
    G4int numZPlanes = 2;
    G4double rInner[] = {(sqrt(3) / 2)* TPCpRmin* CLHEP::mm, (sqrt(3) / 2)* TPCpRmin* CLHEP::mm};
    G4double rOuter[] = {(sqrt(3) / 2)* TPCpRmax* CLHEP::mm, (sqrt(3) / 2)* TPCpRmax* CLHEP::mm};
    G4double zPlane[] = {-TPCpDepth / 2., TPCpDepth / 2.};

    G4Polyhedra* tpcConeS = new G4Polyhedra("TPCCone", phiStart, phiTotal, numSide, numZPlanes, zPlane, rInner, rOuter);

    G4LogicalVolume* tpcConeLV
      = new G4LogicalVolume(tpcConeS, P10gas, "TPCCone", localFieldMgr, 0, 0);

    tpcConeLV->SetVisAttributes(G4VisAttributes(G4Colour(239 / 255., 83 / 255., 80 / 255.)));

    new G4PVPlacement(0,               // no rotation
                      G4ThreeVector(), // at (x,y,z)
                      tpcConeLV,       // its logical volume
                      "TPCCone",       // its name
                      worldLV,         // its mother  volume
                      false,           // no boolean operations
                      0,               // copy number
                      fCheckOverlaps); // checking overlaps


    // パッド中央を通る有感領域を設定
    G4double hPad = 14.5 * CLHEP::mm; // Hight of Cathode Pad
    G4double wPad = 5.1 * CLHEP::mm; // Wide of Cathode Pad;
    G4double dPad = 0.1 * CLHEP::mm; // Distance of Pad on hight direction
    G4int numPad = 9;
    G4double frameThin = 12 * CLHEP::mm; // Thin of sensitive detectors
    G4double thinSD = 0.10 * CLHEP::mm; // Hit しないパッドが多かったため0.1mm->1.0mmに変更
    G4int numSideSD = 6;
    G4int numZPlanesSD = 2;

    G4double rInnerSD[2];
    G4double rOuterSD[2];
    G4double zPlaneSD[] = {-TPCpDepth / 2., TPCpDepth / 2.};

    for (G4int padNo = 0; padNo < numPad; padNo++) {
      for (G4int i = 0; i < 2; i++) {
        if (!padNo)
          rInnerSD[i] = ((sqrt(3) / 2) * TPCpRmin) + frameThin + dPad + (hPad / 2.) - (thinSD / 2.);
        else
          rInnerSD[i] = rInnerSD[i] + hPad + dPad;

        rOuterSD[i] = rInnerSD[i] + thinSD;
      }

      G4Polyhedra* tpcPadSD = new G4Polyhedra("TPCPad_solid", phiStart, phiTotal, numSide, numZPlanesSD, zPlane, rInnerSD, rOuterSD);

      tpcPadLV =
        new G4LogicalVolume(tpcPadSD, P10gas, "TPCPad_LV", localFieldMgr, 0, 0);

      tpcPadLV->SetVisAttributes(G4VisAttributes(G4Colour(38 / 255., 198 / 255., 218 / 255.)));
      // Step Sizeを変更
      G4double maxStep = 0.040 * CLHEP::mm; // パッド部の厚さthinSDの半分以下程度
      G4UserLimits* fstepLimit = new G4UserLimits(maxStep);
      tpcPadLV->SetUserLimits(fstepLimit);

      new G4PVPlacement(0,                            // no rotation
                        G4ThreeVector(), // at (x,y,z)
                        tpcPadLV,        // its logical volume
                        "TPCPad_PV",                 // its name
                        tpcConeLV,                    // its mother  volume
                        false,                        // no boolean operations
                        padNo + 1,                     // copy number
                        fCheckOverlaps);              // checking overlaps
    }

// サポーター実装前
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// サポーター実装(サポーターの詳しい寸法、図面がわからないので保留)
    /*
    for(G4int j = 0; j < 6 ; j++){

        for(G4int padNo = 0; padNo< numPad; padNo++){
              for(G4int i = 0; i < 2; i++){
              if(!padNo)
                  rInnerSD[i] = ( (sqrt(3)/ 2 ) *TPCpRmin ) + frameThin + dPad + (hPad /2.) - (thinSD/2.);
              else
                  rInnerSD[i] = rInnerSD[i] + hPad + dPad;

              rOuterSD[i] = rInnerSD[i] + thinSD;
              }

        G4double angleSupporter = (2.906/360.0)*2*CLHEP::pi; // サポーターの広がり角
        G4double phiStart1 = angleSupporter/2. + j*(CLHEP::pi/3.);
        G4double phiTotal1 = (CLHEP::pi/3.) -angleSupporter;
        G4int numSide1 = 1;

        G4Polyhedra* tpcPadSD = new G4Polyhedra("TPCPad_solid", phiStart1, phiTotal1, numSide1, numZPlanesSD, zPlane, rInnerSD, rOuterSD);

         tpcPadLV =
              new G4LogicalVolume(tpcPadSD, P10gas, "TPCPad_LV",0,0,0);

          tpcPadLV->SetVisAttributes(G4VisAttributes(G4Colour(38/255.,198/255.,218/255.)));
          // Step Sizeを変更
          G4double maxStep = 0.040 *mm; // パッド部の厚さthinSDの半分以下程度
          G4UserLimits *fstepLimit = new G4UserLimits(maxStep);
          tpcPadLV->SetUserLimits(fstepLimit);


          new G4PVPlacement(0,                            // no rotation
                            G4ThreeVector(), // at (x,y,z)
                            tpcPadLV,        // its logical volume
                            "TPCPad_PV",                 // its name
                            tpcConeLV,                    // its mother  volume
                            false,                        // no boolean operations
                            padNo+1,                       // copy number
                            fCheckOverlaps);              // checking overlaps

        }
    }*/

    // 7Li標的
    G4double targetLiWidth = 70.0 * CLHEP::mm; // x方向
    G4double targetLiHight = 40.0 * CLHEP::mm; // y方向
    G4double targetLiLength = 200.0 * CLHEP::mm; // z方向
    G4double targetLiPx = targetLiWidth / 2.0;
    G4double targetLiPy = targetLiHight / 2.0;
    G4double targetLiPz = targetLiLength / 2.0;

    G4Box* targetLiS = new G4Box("targetLi", targetLiPx, targetLiPy, targetLiPz);
    G4LogicalVolume* targetLiLV =
      new G4LogicalVolume(targetLiS, Li7, "targetLi_LV", localFieldMgr, 0, 0); // 標的内の磁場は？
    targetLiLV->SetVisAttributes(G4VisAttributes(G4Colour(92 / 255., 107 / 255., 192 / 255.)));

    // 標的On / Off
    // コマンドライン引数argv[2] を1としたときのみ標的出現
    if (false) {
      new G4PVPlacement(0,                            // no rotation
                        G4ThreeVector(), // at (x,y,z)
                        targetLiLV,        // its logical volume
                        "targetLi_PV",                 // its name
                        worldLV,                    // its mother  volume
                        false,                        // no boolean operations
                        0,                       // copy number
                        fCheckOverlaps);              // checking overlaps

      std::cout << "->->->----------> Taget ON !! <-----------<-<-<- " << std::endl;
    }

    // Target(サンプルファイルのもの、今は使っていない)


    G4ThreeVector positionTarget = G4ThreeVector(0, 0, -(targetLength + trackerSize));

    G4Tubs* targetS
      = new G4Tubs("target", 0., targetRadius, targetLength, 0.*CLHEP::degree, 360.*CLHEP::degree);
    fLogicTarget
      = new G4LogicalVolume(targetS, fTargetMaterial, "Target", 0, 0, 0);

// PhysicalVolume部分のみコメント
    /*
     new G4PVPlacement(0,               // no rotation
                       positionTarget,  // at (x,y,z)
                       fLogicTarget,    // its logical volume
                       "Target",        // its name
                       worldLV,         // its mother volume
                       false,           // no boolean operations
                       0,               // copy number
                       fCheckOverlaps); // checking overlaps
    */

    G4cout << "Target is " << 2 * targetLength / CLHEP::cm << " cm of "
           << fTargetMaterial->GetName() << G4endl;



    // Tracker


    G4ThreeVector positionTracker = G4ThreeVector(0, 0, 0);

    G4Tubs* trackerS
      = new G4Tubs("tracker", 0, trackerSize, trackerSize, 0.*CLHEP::degree, 360.*CLHEP::degree);
    G4LogicalVolume* trackerLV
      = new G4LogicalVolume(trackerS, air, "Tracker", 0, 0, 0);

// PhysicalVolume部分のみコメント
    /*
     new G4PVPlacement(0,               // no rotation
                       positionTracker, // at (x,y,z)
                       trackerLV,       // its logical volume
                       "Tracker",       // its name
                       worldLV,         // its mother  volume
                       false,           // no boolean operations
                       0,               // copy number
                       fCheckOverlaps); // checking overlaps
    */

    // Visualization attributes


    G4VisAttributes* boxVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
    G4VisAttributes* chamberVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0));

    worldLV      ->SetVisAttributes(boxVisAtt);
    fLogicTarget ->SetVisAttributes(boxVisAtt);
    trackerLV    ->SetVisAttributes(boxVisAtt);



    // Tracker segments


    G4cout << "There are " << fNbOfChambers << " chambers in the tracker region. "
           << G4endl
           << "The chambers are " << chamberWidth / CLHEP::cm << " cm of "
           << fChamberMaterial->GetName() << G4endl
           << "The distance between chamber is " << chamberSpacing / CLHEP::cm << " cm"
           << G4endl;

// G4double firstPosition = -trackerSize + chamberSpacing;
    G4double firstLength   = trackerLength / 10;
    G4double lastLength    = trackerLength;

    G4double halfWidth = 0.5 * chamberWidth;
    G4double rmaxFirst = 0.5 * firstLength;

    G4double rmaxIncr = 0.0;
    if (fNbOfChambers > 0) {
      rmaxIncr =  0.5 * (lastLength - firstLength) / (fNbOfChambers - 1);
      if (chamberSpacing  < chamberWidth) {
        G4Exception("TPCDetectorConstruction::DefineVolumes()",
                    "InvalidSetup", FatalException,
                    "Width>Spacing");
      }
    }


    for (G4int copyNo = 0; copyNo < fNbOfChambers; copyNo++) {

      //   G4double Zposition = firstPosition + copyNo * chamberSpacing;
      G4double rmax =  rmaxFirst + copyNo * rmaxIncr;

      G4Tubs* chamberS
        = new G4Tubs("Chamber_solid", 0, rmax, halfWidth, 0.*CLHEP::degree, 360.*CLHEP::degree);

      fLogicChamber[copyNo] =
        new G4LogicalVolume(chamberS, fChamberMaterial, "Chamber_LV", 0, 0, 0);

      fLogicChamber[copyNo]->SetVisAttributes(chamberVisAtt);

// PhysicalVolume部分のみコメント
      /*
            new G4PVPlacement(0,                            // no rotation
                              G4ThreeVector(0,0,Zposition), // at (x,y,z)
                              fLogicChamber[copyNo],        // its logical volume
                              "Chamber_PV",                 // its name
                              trackerLV,                    // its mother  volume
                              false,                        // no boolean operations
                              copyNo,                       // copy number
                              fCheckOverlaps);              // checking overlaps
      */

    }


    // Example of User Limits
    //
    // Below is an example of how to set tracking constraints in a given
    // logical volume
    //
    // Sets a max step length in the tracker region, with G4StepLimiter


    G4double maxStep = 0.5 * chamberWidth;
    fStepLimit = new G4UserLimits(maxStep);
    trackerLV->SetUserLimits(fStepLimit);


    /// Set additional contraints on the track, with G4UserSpecialCuts
    ///
    /// G4double maxLength = 2*trackerLength, maxTime = 0.1*ns, minEkin = 10*MeV;
    /// trackerLV->SetUserLimits(new G4UserLimits(maxStep,
    ///                                           maxLength,
    ///                                           maxTime,
    ///                                           minEkin));

    // Always return the physical world

    return worldPV;
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//ここから********************************************************************


  void TPCDetectorConstruction::ConstructSDandField()
  {
    // Sensitive detectors

    G4String trackerChamberSDname = "TPC/TrackerChamberSD";
    TPCTrackerSD* aTrackerSD = new TPCTrackerSD(trackerChamberSDname,
                                                "TrackerHitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(aTrackerSD);
    // Setting aTrackerSD to all logical volumes with the same name
    // of "Chamber_LV".
    // SetSensitiveDetector("Chamber_LV", aTrackerSD, true);
    // SetSensitiveDetector("TPCCone", aTrackerSD, true);  // TPCをSensitiveDetectorに設定
    SetSensitiveDetector("TPCPad_LV", aTrackerSD, true);

    // Create global magnetic field messenger.
    // Uniform magnetic field is then created automatically if
    // the field value is not zero.
    G4ThreeVector fieldValue = G4ThreeVector();
    fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
    fMagFieldMessenger->SetVerboseLevel(1);

    // Register the field messenger for deleting
    G4AutoDelete::Register(fMagFieldMessenger);
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void TPCDetectorConstruction::SetTargetMaterial(G4String materialName)
  {
    G4NistManager* nistManager = G4NistManager::Instance();

    G4Material* pttoMaterial =
      nistManager->FindOrBuildMaterial(materialName);

    if (fTargetMaterial != pttoMaterial) {
      if (pttoMaterial) {
        fTargetMaterial = pttoMaterial;
        if (fLogicTarget) fLogicTarget->SetMaterial(fTargetMaterial);
        G4cout
            << G4endl
            << "----> The target is made of " << materialName << G4endl;
      } else {
        G4cout
            << G4endl
            << "-->  WARNING from SetTargetMaterial : "
            << materialName << " not found" << G4endl;
      }
    }
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void TPCDetectorConstruction::SetChamberMaterial(G4String materialName)
  {
    G4NistManager* nistManager = G4NistManager::Instance();

    G4Material* pttoMaterial =
      nistManager->FindOrBuildMaterial(materialName);

    if (fChamberMaterial != pttoMaterial) {
      if (pttoMaterial) {
        fChamberMaterial = pttoMaterial;
        for (G4int copyNo = 0; copyNo < fNbOfChambers; copyNo++) {
          if (fLogicChamber[copyNo]) fLogicChamber[copyNo]->
            SetMaterial(fChamberMaterial);
        }
        G4cout
            << G4endl
            << "----> The chambers are made of " << materialName << G4endl;
      } else {
        G4cout
            << G4endl
            << "-->  WARNING from SetChamberMaterial : "
            << materialName << " not found" << G4endl;
      }
    }
  }

//ここまで********************************************************************

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void TPCDetectorConstruction::SetMaxStep(G4double maxStep)
  {
    if ((fStepLimit) && (maxStep > 0.)) fStepLimit->SetMaxAllowedStep(maxStep);
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void TPCDetectorConstruction::SetCheckOverlaps(G4bool checkOverlaps)
  {
    fCheckOverlaps = checkOverlaps;
  }
}
