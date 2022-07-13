/**************************************************************************
 * basf2 (Belle II Analysis Software Framework)                           *
 * Author: M. Uchida                                                      *
 *                                                                        *
 * See git log for contributors and copyright holders.                    *
 * This file is licensed under LGPL-3.0, see LICENSE.md.                  *
 **************************************************************************/

#pragma once

#include <map>
#include <TObject.h>

namespace E75 {

  /**
   * The TPCRawHit (Full output mode) class.
   *
   */

  class TPCRawHit : public TObject {
  public:

    /**
     * Default constructor.
     */
    TPCRawHit();

    /**
     * Constructor.
     */
    TPCRawHit(int port, int node, int trig, int fclk, int cclk,
                      unsigned int* data);


    /**
     * Destructor.
     */
    virtual ~TPCRawHit();


    /**
     * Import data.
     */
    void import(int port, int node, int trig, int fclk, int cclk, unsigned int* data);
    
    /**
     * Setter for trigger count.
     */
    inline void setTriggerCount(int trgCount)
    {
      m_triggerCount = trgCount;
    }


    /**
     * Getter for trigger number.
     */
    inline int getTriggerCount(void) const
    {
      return m_triggerCount;
    }

    /**
     * Setter for Node ID.
     */
    inline void setNode(unsigned short node)
    {
      m_node = node;
    }

    /**
     * Getter for Node ID.
     */
    inline unsigned short getNode(void) const
    {
      return m_node;
    }

    /**
     * Setter for Port ID.
     */
    inline void setPort(unsigned short port)
    {
      m_port = port;
    }

    /**
     * Getter for Port ID.
     */
    inline unsigned short getPort(void) const
    {
      return m_port;
    }

    /**
     * Getter for channel data.
     */
    inline std::vector<int> getChData(int i) const
    {
      return m_chData.at(i);
    }

    /**
     * Clear dataobject.
     */
    inline void clear()
    {
      m_port = 0;
      m_node = 0;
      m_triggerCount = 0;
      m_triggerFClk = 0;
      m_triggerCClk = 0;
      m_chData.clear();
    }

  private:

    /**
     * Port ID.
     */
    unsigned short m_port;
    /**
     * Node ID.
     */
    unsigned short m_node;

    /**
     * Trigger count.
     */
    int m_triggerCount;

    /**
     * Trigger FClock.
     */
    int m_triggerFClk;

    /**
     * Trigger CClock.
     */
    int m_triggerCClk;

    /**
     * Channel data.
     */
    std::map<int, std::vector<int>> m_chData = {};

    /// To derive from TObject
    ClassDef(TPCRawHit, 1);
  };
}
