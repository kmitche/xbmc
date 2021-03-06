/*
*      Copyright (C) 2005-2008 Team XBMC
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

#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#pragma once

#include "guilib/Geometry.h"
#include "guilib/TransformMatrix.h"
#include "guilib/DirtyRegion.h"
#include "utils/StdString.h"
#include <stdint.h>

typedef enum _RenderingSystemType
{
  RENDERING_SYSTEM_OPENGL,
  RENDERING_SYSTEM_DIRECTX,
  RENDERING_SYSTEM_OPENGLES
} RenderingSystemType;

/*
*   CRenderSystemBase interface allows us to create the rendering engine we use.
*   We currently have two engines: OpenGL and DirectX
*   This interface is very basic since a lot of the actual details will go in to the derived classes
*/

typedef uint32_t color_t;

enum
{
  RENDER_CAPS_DXT      = (1 << 0),
  RENDER_CAPS_NPOT     = (1 << 1),
  RENDER_CAPS_DXT_NPOT = (1 << 2),
  RENDER_CAPS_BGRA     = (1 << 3),
  RENDER_CAPS_BGRA_APPLE = (1 << 4)
};

enum
{
  RENDER_QUIRKS_MAJORMEMLEAK_OVERLAYRENDERER = 1 << 0,
  RENDER_QUIRKS_YV12_PREFERED                = 1 << 1,
};

class CRenderSystemBase
{
public:
  CRenderSystemBase();
  virtual ~CRenderSystemBase();

  // Retrieve
  RenderingSystemType GetRenderingSystemType() { return m_enumRenderingSystem; }

  virtual bool InitRenderSystem() = 0;
  virtual bool DestroyRenderSystem() = 0;
  virtual bool ResetRenderSystem(int width, int height, bool fullScreen, float refreshRate) = 0;

  virtual bool BeginRender() = 0;
  virtual bool EndRender() = 0;
  virtual bool PresentRender(const CDirtyRegionList& dirty) = 0;
  virtual bool ClearBuffers(color_t color) = 0;
  virtual bool IsExtSupported(const char* extension) = 0;

  virtual void SetVSync(bool vsync) = 0;
  bool GetVSync() { return m_bVSync; }

  virtual void SetViewPort(CRect& viewPort) = 0;
  virtual void GetViewPort(CRect& viewPort) = 0;

  virtual void SetScissors(const CRect &rect) = 0;
  virtual void ResetScissors() = 0;

  virtual void CaptureStateBlock() = 0;
  virtual void ApplyStateBlock() = 0;

  virtual void SetCameraPosition(const CPoint &camera, int screenWidth, int screenHeight) = 0;
  virtual void ApplyHardwareTransform(const TransformMatrix &matrix) = 0;
  virtual void RestoreHardwareTransform() = 0;

  virtual bool TestRender() = 0;

  void GetRenderVersion(unsigned int& major, unsigned int& minor) const;
  const CStdString& GetRenderVendor() const { return m_RenderVendor; }
  const CStdString& GetRenderRenderer() const { return m_RenderRenderer; }
  const CStdString& GetRenderVersionString() const { return m_RenderVersion; }
  bool SupportsDXT() const;
  bool SupportsBGRA() const;
  bool SupportsBGRAApple() const;
  bool SupportsNPOT(bool dxt) const;
  unsigned int GetMaxTextureSize() const { return m_maxTextureSize; }
  unsigned int GetMinDXTPitch() const { return m_minDXTPitch; }
  unsigned int GetRenderQuirks() const { return m_renderQuirks; }

protected:
  bool                m_bRenderCreated;
  RenderingSystemType m_enumRenderingSystem;
  bool                m_bVSync;
  unsigned int        m_maxTextureSize;
  unsigned int        m_minDXTPitch;

  CStdString   m_RenderRenderer;
  CStdString   m_RenderVendor;
  CStdString   m_RenderVersion;
  int          m_RenderVersionMinor;
  int          m_RenderVersionMajor;
  unsigned int m_renderCaps;
  unsigned int m_renderQuirks;
};

#endif // RENDER_SYSTEM_H
