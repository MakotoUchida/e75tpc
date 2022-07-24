/// \file E75FileName.hh
/// \brief Implementation of the E75FileName class
#include "vector"
#include <iostream>
#include <fstream>
#include <string>

#include "GlobalFileName.hh"

static std::string targetGlo;
static std::string targetOnGlo;
static std::string momGlo;
static std::string thetaGlo;
static std::string phiGlo;
static double momDGlo, thetaDGlo, phiDGlo;
static int targetOnIGlo, magneticFieldOnIGlo;

static std::string magneticFieldGlo;
static std::string magneticFiledOnGlo;

static std::string filenameGlo;
static const std::string head = "geantHits";
static const std::string kaku = "Li.root";
static const std::string underBar = "_";
static const std::string withC = "w";
static const std::string withoutC = "wo";
static const std::string uniformMF = "uniform";
static const std::string mapMF = "map";

static std::string randamGlo;
static int randamIGlo;

void GlobalSetFileName(char* argv2, char* argv3, char* argv4, char* argv5, char* argv6)
{

  targetGlo = std::string(argv2);
  momGlo = std::string(argv3);
  thetaGlo = std::string(argv4);
  phiGlo = std::string(argv5);
  magneticFieldGlo = std::string(argv6);

  targetOnIGlo = atoi(targetGlo.c_str());
  momDGlo = atof(momGlo.c_str());
  thetaDGlo = atof(thetaGlo.c_str());
  phiDGlo = atof(phiGlo.c_str());
  magneticFieldOnIGlo = atoi(magneticFieldGlo.c_str());

  if (targetOnIGlo == 0) {
    targetOnGlo = withoutC;
  } else if (targetOnIGlo == 1) {
    targetOnGlo = withC;
  }

  if (magneticFieldOnIGlo == 0) {
    magneticFiledOnGlo =  uniformMF;
  } else if (magneticFieldOnIGlo == 1) {
    magneticFiledOnGlo =  mapMF;
  }

  filenameGlo = head + momGlo + "_" + thetaGlo + "_" + phiGlo + "_" + magneticFiledOnGlo + "_" + targetOnGlo + kaku;

  std::cout << "output filename is ......." << filenameGlo << std::endl;
}

std::string GetfilenameGlo()
{
  return filenameGlo;
}

int GlobalGettargetOnIGlo()
{
  return targetOnIGlo;
}

double GlobalGetmomDGlo()
{
  return momDGlo;
}

double GlobalGetthetaDGlo()
{
  return thetaDGlo;
}

double GlobalGetphiDGlo()
{
  return phiDGlo;
}

int GlobalGetmagneticFieldOnIGlo()
{
  return magneticFieldOnIGlo;
}
void GlobalSetrandam(char* argv7)
{
  randamGlo = std::string(argv7);
  randamIGlo = atoi(randamGlo.c_str());
}
void GlobalSetrandamEmp()
{
  randamIGlo = 0;
}
int GlobalGetrandamIGlo()
{
  return randamIGlo;
}
