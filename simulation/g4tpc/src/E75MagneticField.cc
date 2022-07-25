/// \file E75MagneticField.cc
/// \brief Implementation of the E75MagneticField class

#include "E75MagneticField.h"
#include "G4UnitsTable.hh"

E75MagneticField::E75MagneticField(): G4MagneticField()
{

  //This is a thread-local class and we have to avoid that all workers open the
  //file at the same time
  // G4AutoLock lock(&myTabulatedField3DLock);
  //

  const char* filename = "magneticField.txt";

  // double lenUnit= cm;
  // double magUnit = kilogauss; // inputファイルにおけるUnit

  G4cout << "\n ---> " "Reading the field grid from " << filename << " ... " << G4endl;
  std::ifstream file(filename); // Open the file for reading.
  if (!file) {
    std::cerr << " 'magneticField.txt' does not exit ! " << std::endl;
  }

  // Read table dimensions
  file >>  fNz >> fNy; // Note dodgy order

  G4cout << "  [ Number of values x,y,z: "
         << fNz << " " << fNy << " ] "
         << G4endl;

  // Set up storage space for table
  fZField.resize(fNz);
  fYField.resize(fNz);
  G4int iy, iz;

  for (iz = 0; iz < fNz; iz++) {
    fZField[iz].resize(fNy);
    fYField[iz].resize(fNy);
  }

  // Read in the data
  G4double yval, zval, by, bz;
  // double permeability; // Not used in this example.

  for (iz = 0; iz < fNz; iz++) {
    for (iy = 0; iy < fNy; iy++) {
      file >> zval >> yval >> by >> bz ;
      if (iy == 0 && iz == 0) {
        G4double fMiniyCM = yval * cm;
        fMiniy = fMiniyCM;
        G4double fMinizCM = zval * cm;
        fMiniz = fMinizCM;
      }
      fYField[iz][iy] = by;
      fZField[iz][iy] = bz;
    }
  }
  file.close();

  // おそらくいらない
  // lock.unlock();
  //

  fMaxiy = yval * cm;
  fMaxiz = zval * cm;

  G4cout << "\n ---> ... done reading " << G4endl;

  // G4cout << " Read values of field from file " << filename << endl;

  G4cout << " ---> assumed the order:  x, y, z, Bx, By, Bz "
         << "\n ---> Min values x,y,z: "
         << fMiniy << " " << fMiniz << " cm "
         << "\n ---> Max values y,z: "
         << fMaxiy << " " << fMaxiz << " cm " << G4endl;

  fDy = fMaxiy - fMiniy;
  fDz = fMaxiz - fMiniz;

  G4cout << "\n ---> Dif values y,z (range): "
         << fDy << " " << fDz << " cm in z "
         << "\n-----------------------------------------------------------" << G4endl;

}

// E75MagneticField::~E75MagneticField(){}

void E75MagneticField::GetFieldValue(const double point[4],
                                     double* Bfield) const
{

//******************************************************************

  // MAP


  Bfield[0] = 0.0;
  Bfield[1] = 0.0;
  Bfield[2] = 0.0;
  Bfield[3] = 0.0;
  Bfield[4] = 0.0;
  Bfield[5] = 0.0;

  G4double x = point[0];
  G4double y = point[1];
  G4double z = point[2];

  G4double r_xy = sqrt((x * x) + (y * y));
  // Check that the point is within the defined region

  // Position of given point within region, normalized to the range
  // [0,1]
  G4double yfraction = (r_xy - fMiniy) / fDy;
  G4double zfraction = (abs(z) - fMiniz) / fDz;

  // Need addresses of these to pass to modf below.
  // modf uses its second argument as an OUTPUT argument.
  G4double  ydindex, zdindex;

  // Position of the point within the cuboid defined by the
  // nearest surrounding tabulated points
  G4double ylocal = (std::modf(yfraction * (fNy - 1), &ydindex));
  G4double zlocal = (std::modf(zfraction * (fNz - 1), &zdindex));

  // The indices of the nearest tabulated point whose coordinates
  // are all less than those of the given point

  //int xindex = static_cast<int>(xdindex);
  //int yindex = static_cast<int>(ydindex);
  //int zindex = static_cast<int>(zdindex);

  // SI 15/12/2016: modified according to bugzilla 1879
  G4int yindex = static_cast<int>(std::floor(ydindex));
  G4int zindex = static_cast<int>(std::floor(zdindex));

  G4double deltaZ = fDz / (fNz - 1);
  G4double deltaY = fDy / (fNy - 1);

  if (yindex < fNy - 1 &&
      zindex < fNz - 1) {
    // 線形補間のための変数
    G4double fZ0 = ((deltaZ * (zindex + 1)) - abs(z)) / deltaZ;
    G4double fZ1 = 1. - fZ0;
    G4double fY0 = ((deltaY * (yindex + 1)) - r_xy) / deltaY;
    G4double fY1 = 1. - fY0;

    G4double w00 = fZ0 * fY0;
    G4double w01 = fZ0 * fY1;
    G4double w10 = fZ1 * fY0;
    G4double w11 = fZ1 * fY1;

    Bfield[0] = 0;
    if ((y * z) >= 0) {
      Bfield[1] = (w00 * fYField[zindex][yindex]
                   + w01 * fYField[zindex][yindex + 1]
                   + w10 * fYField[zindex + 1][yindex]
                   + w11 * fYField[zindex + 1][yindex + 1]
                  ) * 0.1 * tesla ;
    } else {
      Bfield[1] = -(w00 * fYField[zindex][yindex]
                    + w01 * fYField[zindex][yindex + 1]
                    + w10 * fYField[zindex + 1][yindex]
                    + w11 * fYField[zindex + 1][yindex + 1]
                   ) * 0.1 * tesla ;
    }
    Bfield[2] = (w00 * fZField[zindex][yindex]
                 + w01 * fZField[zindex][yindex + 1]
                 + w10 * fZField[zindex + 1][yindex]
                 + w11 * fZField[zindex + 1][yindex + 1]
                ) * 0.1 * tesla ;

    /*
    // Byはyz平面上で原点対象
    Bfield[0] = 0;
    if( (y*z) >= 0){
        Bfield[1] = fYField[zindex][yindex]*0.1*tesla;
    }else{
        Bfield[1] = -fYField[zindex][yindex]*0.1*tesla;
    }
    Bfield[2] = fZField[zindex][yindex]*0.1*tesla;
    */

  }  // Worldに収まっているときに括弧内を実行
  else {
    if (yindex >= fNy - 1) {yindex = fNy - 1;}
    if (zindex >= fNz - 1) {zindex = fNz - 2;}

    G4double w0 = ((deltaZ * (zindex + 1)) - abs(z)) / deltaZ;
    G4double w1 = 1. - w0;

    Bfield[0] = 0;
    if ((y * z) >= 0) {
      Bfield[1] = (w0 * fYField[zindex][yindex]
                   + w1 * fYField[zindex + 1][yindex]) * 0.1 * tesla;
    } else {
      Bfield[1] = -(w0 * fYField[zindex][yindex]
                    + w1 * fYField[zindex + 1][yindex]) * 0.1 * tesla;
    }
    Bfield[2] = (w0 * fZField[zindex][yindex]
                 + w1 * fZField[zindex + 1][yindex]) * 0.1 * tesla;

    // G4cout << " position y > 30. cm (defined magnetic fileld) " << G4endl;
    // G4cout << "(x,y,z) = " << "(" << x << ", " << y << ", " << z << ") "
    // <<  "(Bx,By,Bz) = " << "(" << Bfield[0] << ", " << Bfield[1] << ", " << Bfield[2] << ") " << G4endl;
  }

  /*G4cout << "(x,y,z) = " << "(" << x << ", " << y << ", " << z << ") "
          <<  "(Bx,By,Bz) = " << "(" << Bfield[0] << ", " << Bfield[1] << ", " << Bfield[2] << ") " << G4endl; */

}
