/*
 *      Copyright (C) 2005-2010 Team XBMC
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include "MythXmlParameters.h"

void MythXmlParameters::SetParameter(const CStdString &name, const CStdString &value) {
  m_parameters[name] = value;
}

void MythXmlParameters::SetParameter(const CStdString &name, const int &value) {
  CStdString str;
  str.Format("%i", value);
  SetParameter(name, str);
}

void MythXmlParameters::SetParameter(const CStdString &name, const time_t &value) {
  struct tm* time;
  time = localtime(&value);
  CStdString str;
  str.Format("%i-%02.2i-%02.2iT%02.2i:%02.2i:%02.2i", time->tm_year + 1900, time->tm_mon + 1, time->tm_mday,
             time->tm_hour, time->tm_min, time->tm_sec);
  // TODO: Change to use strftime(str.GetBuffer(19), 19, "%Y-%m-%dT%H:%M:%S", time); // YYYY-MM-DDTHH:mm:ss
  SetParameter(name, str);
}

CStdString MythXmlParameters::GetParameterString() {
  CStdString str;
  for (PARAMETERS::const_iterator it = m_parameters.begin(); it != m_parameters.end(); it++)
  {
    if (it != m_parameters.begin())
      str += "&";
    str += it->first + "=" + it->second;
  }
  return str;
}
