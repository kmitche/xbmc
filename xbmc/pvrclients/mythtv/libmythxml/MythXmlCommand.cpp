/*
 * MythXmlCommand.cpp
 *
 *  Created on: Oct 7, 2010
 *      Author: mythtv
 */

#include "MythXmlCommand.h"

void MythXmlCommand::Init(const CStdString& command, MythXmlParameters& parameters)
{
  m_command = command;
  m_parameters = parameters;
}

CStdString MythXmlCommand::GetPath()
{
  CStdString path(m_command); // TODO: Confirm this is correct.
  if (m_parameters.HasParameter())
    path += "?" + m_parameters.GetParameterString();
  return path;
}
