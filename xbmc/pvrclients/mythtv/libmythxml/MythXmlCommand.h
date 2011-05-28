#pragma once
/*
 * MythXmlCommand.h
 *
 *  Created on: Oct 7, 2010
 *      Author: tafypz
 */

#include "MythXmlParameters.h"

#include "utils/StdString.h"
#include "tinyxml.h"

class MythXmlCommand
{
  public:
    CStdString GetPath();
    virtual bool ParseResponse(const TiXmlHandle& handle) = 0;

  protected:
    // TODO: optional parameters argument.
    void Init(const CStdString& command, MythXmlParameters& parameters);

  private:
    CStdString m_command;
    MythXmlParameters m_parameters;

};
