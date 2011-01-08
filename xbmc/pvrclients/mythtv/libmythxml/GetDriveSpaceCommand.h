#pragma once
/*
 * GetDriveSpaceResult.h
 *
 *  Created on: Oct 7, 2010
 *      Author: tafypz
 */

#include "MythXmlCommand.h"

class GetDriveSpaceCommand: public MythXmlCommand
{
  public:
    GetDriveSpaceCommand();

    // TODO: int64_t way safer if suppose to be 64 bit
    long long GetTotal();
    long long GetUsed();

    bool ParseResponse(const CStdString& response);

  private:
    long long m_total;
    long long m_used;
};
