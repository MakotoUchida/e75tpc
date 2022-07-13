/**************************************************************************
 * basf2 (Belle II Analysis Software Framework)                           *
 * Author: Makoto Uchida                                                  *
 *                                                                        *
 * See git log for contributors and copyright holders.                    *
 * This file is licensed under LGPL-3.0, see LICENSE.md.                  *
 **************************************************************************/

#pragma once


#include "TPCRawHit.h"

#include <TFile.h>
#include <TTree.h>

#include <iostream>
#include <fstream>


namespace E75 {



  /**
   * TPCUnpackerModule: The TPC Raw Hits Decoder.
   */

  class TPCUnpackerModule {

  public:
    /**
     * Constructor of the module.
     */
    TPCUnpackerModule();

    /**
     * Destructor of the module.
     */
    virtual ~TPCUnpackerModule();

    /**
     * Initializes the Module.
     */
    void initialize(std::string ifname, std::string ofname);

    /**
     * Event action (main routine).
     *
     */

    void process();

    /**
     * Termination action.
     */
    void terminate();

  private:
    std::ifstream m_ifs;
    int m_event = 0;
    std::string m_inputFile = "output.dat";
    std::string m_outputFile = "output.root";
    std::string m_tpcRawHitmName = "";
    TPCRawHit m_tpcRawHit;
    TFile *m_tFile;
    TTree *m_tree;
      
  };//end class declaration


} // end namespace E75

