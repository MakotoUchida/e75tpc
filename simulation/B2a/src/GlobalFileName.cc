/// \file E75FileName.hh
/// \brief Implementation of the E75FileName class
#include "vector"
#include <iostream>
#include <fstream>
#include <string>
#include <boost/format.hpp>
#include "GlobalFileName.hh"

static std::string globalFileName;

void globalSetFileName(char* exp, char* run, char* st)
{

  int expNum = atoi(exp);
  int runNum = atoi(run);
  int stNum = atoi(st);
  globalFileName = (boost::format("mc.e%04d.r%06d.s%03d.root") % expNum % runNum % stNum).str();
  //  fileName = "test.root";
  std::cout << "output filename is ......." << globalFileName << std::endl;
}

std::string globalGetFileName()
{
  return globalFileName;
}

