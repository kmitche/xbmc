#pragma once

/*
 *      Copyright (C) 2005-2009 Team XBMC
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

#include "library.xbmc.addon/libXBMC_addon.h"
#include "library.xbmc.pvr/libXBMC_pvr.h"

#include "utils/StdString.h"

#define DEFAULT_HOST          "127.0.0.1"
#define DEFAULT_MYTHXML_PORT  6544
#define DEFAULT_PIN           0000
#define DEFAULT_MYTHXML_CONNECTION_TIMEOUT 30000
#define DEFAULT_USER "mythtv"
#define DEFAULT_PASS "mythtv"

extern bool         g_bCreated;           ///< Shows that the Create function was successfully called
extern int          g_iClientID;          ///< The PVR client ID used by XBMC for this driver
extern CStdString   g_szUserPath;         ///< The Path to the user directory inside user profile
extern CStdString   g_szClientPath;       ///< The Path where this driver is located

/* Client Settings */
extern CStdString	  g_szUserName;		      ///< The User Name to use to connect to the mythtv server (default is mythtv)
extern CStdString	  g_szPassword;		      ///< The Password to use to connect to the mythtv server (default is mythtv)
extern CStdString   g_szHostname;         ///< The Host name or IP of the mythtv server
extern int          g_iMythXmlPort;       ///< The MYTHXML Port (default is 6544)
extern int          g_iPin;               ///< The Mythtv server PIN (default is 0000)

extern CHelper_libXBMC_addon *XBMC;
extern CHelper_libXBMC_pvr   *PVR;
