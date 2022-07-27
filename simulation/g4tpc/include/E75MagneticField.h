/// \file E75MagneticField.hh
/// \brief Implementation of the E75MagneticField class

#ifndef E75MagneticField_h
#define E75MagneticField_h 1

#include "G4MagneticField.hh"
#include "vector"
#include <iostream>
#include <fstream>
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
namespace E75 {

  class E75MagneticField : public G4MagneticField {
  public:
    E75MagneticField();

    void  GetFieldValue(const  G4double Point[4],
                        G4double* Bfield) const;

    // メンバー変数・関数などの宣言
  private:

    // vector< vector< vector< double > > > fXField;
    std::vector< std::vector< G4double > > fYField;
    std::vector< std::vector< G4double > > fZField;

    G4int fNx, fNy, fNz;
    G4double fMinix, fMaxix, fMiniy, fMaxiy, fMiniz, fMaxiz;
    G4double fDx, fDy, fDz;

    // G4float fGradient1, fGradient2, fGradient3, fGradient4;

    // G4int fModel;

  };
}
#endif
