/**************************************************************************
 * basf2 (Belle II Analysis Software Framework)                           *
 * Author: Makoto Uchida                                                  *
 *                                                                        *
 * See git log for contributors and copyright holders.                    *
 * This file is licensed under LGPL-3.0, see LICENSE.md.                  *
 **************************************************************************/


#include <iostream>
#include "TPCRawHit.h"

using namespace std;
using namespace E75;

TPCRawHit::TPCRawHit() :
  m_port(0), m_node(0), m_triggerCount(0),
  m_triggerFClk(0), m_triggerCClk(0)
{
}

TPCRawHit::TPCRawHit(int port, int node,
                                     int trig, int fclk, int cclk,
                                     unsigned int* data)
{
  m_port = port;
  m_node = node;
  m_triggerCount = trig;
  m_triggerFClk = fclk ;
  m_triggerCClk = cclk;
  std::vector<int> cdata;
  for (int i = 0; i < 16; ++i) {
    const int ch = (*data >> 16) & 0xffff;
    const int eachSize = (*data) & 0xffff;
    cdata.clear();
    data++;
    data++;
    for (int j = 0; j < (eachSize + 1) / 2; ++j) {
      cdata.push_back((*data) & 0xffff);
      cdata.push_back((*data++ >> 16) & 0xffff);
    }
    m_chData.insert(std::make_pair(ch, cdata));
  }
  auto footer =*data++;
  //  std::cout<< std::hex << footer << std::endl;
  if (footer!=0xffff0000) printf("Something wrong! FADC Footer=%08x\n",footer);
  if (footer==0xf0f0f0f0) data++;
}

TPCRawHit::~TPCRawHit()
{
}

void TPCRawHit::import(int port, int node, int trig, int fclk, int cclk,
		       unsigned int* data)
{
  m_port = port;
  m_node = node;
  m_triggerCount = trig;
  m_triggerFClk = fclk ;
  m_triggerCClk = cclk;
  std::vector<int> cdata;
  for (int i = 0; i < 16; ++i) {
    const int ch = (*data >> 16) & 0xffff;
    const int eachSize = (*data) & 0xffff;
    cdata.clear();
    data++;
    data++;
    for (int j = 0; j < (eachSize + 1) / 2; ++j) {
      cdata.push_back((*data) & 0xffff);
      cdata.push_back((*data++ >> 16) & 0xffff);
    }
    m_chData.insert(std::make_pair(ch, cdata));
  }
  auto footer =*data++;
  //  std::cout<< std::hex << footer << std::endl;
  if (footer!=0xffff0000) printf("Something wrong! FADC Footer=%08x\n",footer);
  if (footer==0xf0f0f0f0) data++;
}
