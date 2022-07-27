/**************************************************************************
 * basf2 (Belle II Analysis Software Framework)                           *
 * Author: The Belle II Collaboration                                     *
 *                                                                        *
 * See git log for contributors and copyright holders.                    *
 * This file is licensed under LGPL-3.0, see LICENSE.md.                  *
 **************************************************************************/

#include "TPCUnpackerModule.h"

#include <cmath>
#include <algorithm>
#include <utility>
#include <numeric>


using namespace std;
using namespace E75;

//-----------------------------------------------------------------
//                 Implementation
//-----------------------------------------------------------------

TPCUnpackerModule::TPCUnpackerModule() 
{
}

TPCUnpackerModule::~TPCUnpackerModule()
{
}

void TPCUnpackerModule::initialize(std::string ifname, std::string ofname)
{
  m_inputFile = ifname;
  m_ifs.open(m_inputFile.c_str());
  if (!m_ifs) {
    std::cout <<  m_inputFile << " *** failed to open" << std::endl;
    return;
  }

  m_outputFile = ofname;
  m_tFile =  new TFile(m_outputFile.c_str(),"RECREATE");

  m_tree = new TTree("tree","tree");
  m_tree->Branch("TPCRawHit", &m_tpcRawHit);
}

void TPCUnpackerModule::process()
{
  std::cout << "TPCUnpacker: process() started." << std::endl;
  unsigned int totSize;
  unsigned int numFADCHeader0;
  unsigned int numFADCHeader1;

  if (m_ifs.eof()) return;

  //
  // read haeder
  //

  // Total size of an event.
  m_ifs.read((char*)&totSize, 4 * sizeof(char));
  //  std::cout << std::hex << totSize << std::endl;

  // Number of FADC's in each port.
  m_ifs.read((char*)&numFADCHeader0, sizeof(unsigned int));
  m_ifs.read((char*)&numFADCHeader1, sizeof(unsigned int));

  std::vector<std::pair<int, int>> numFADCs;
  numFADCs.push_back(std::pair<int, int>(0, numFADCHeader0 & 0x000000ff));
  numFADCs.push_back(std::pair<int, int>(1, ((numFADCHeader0 & 0x0000ff00) >> 8)));
  numFADCs.push_back(std::pair<int, int>(2, ((numFADCHeader0 & 0x00ff0000) >> 16)));
  numFADCs.push_back(std::pair<int, int>(3, ((numFADCHeader0 & 0xff000000) >> 24)));
  numFADCs.push_back(std::pair<int, int>(4, (numFADCHeader1 & 0x000000ff)));
  numFADCs.push_back(std::pair<int, int>(5, ((numFADCHeader1 & 0x0000ff00) >> 8)));
  numFADCs.push_back(std::pair<int, int>(6, ((numFADCHeader1 & 0x00ff0000) >> 16)));
  numFADCs.push_back(std::pair<int, int>(7, ((numFADCHeader1 & 0xff000000) >> 24)));

  //  for (const auto& numFADC : numFADCs) {
  //    std::cout << "Num fadc " << numFADC.first << " " << numFADC.second << std::endl;
  //  }

  const int numFADCBoards = std::accumulate(numFADCs.begin(), numFADCs.end(), 0,
  [](const int& a, pair<int, int>& iter) {
    return a + iter.second;
  });

  //  std::cout << "Total number of fadc boards : " << numFADCBoards << std::endl;
  unsigned int buf[4 + (7 + 258 * 16)*numFADCBoards];
  unsigned int* p_buf = buf;

  m_ifs.read((char*)buf, totSize - 12);
  for (int i = 0; i < numFADCBoards; ++i) {
    m_tpcRawHit.clear();    
    const int port = (*p_buf >> 16) & 0xffff;
    const int nodeId = (*p_buf++) & 0xffff;
    //    std::cout << "port : " << port << std::endl;
    //    std::cout << "node : " << nodeId << std::endl;
    const int tgcCount = *p_buf++;
    const int tgcFClk = *p_buf++;
    const int tgcCClk = *p_buf++;
    //    std::cout << "trig : " << tgcCount << std::endl;
    p_buf++; p_buf++;
    m_tpcRawHit.import(port, nodeId, tgcCount, tgcFClk, tgcCClk, p_buf);
    //    std::cout << "added tpcraw" << std::endl;
    m_tree->Fill();
  }

  std::cout << "TPCUnpacker: process() end." << std::endl;
}

void TPCUnpackerModule::terminate()
{
  std::cout << "TPCUnpacker : Terminated." << std::endl;
  m_ifs.close();

  m_tree->Write();
  m_tFile->Close();
}

