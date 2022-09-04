// This file is part of the OGRE project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at https://www.ogre3d.org/licensing.
// SPDX-License-Identifier: MIT

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreWindowEventUtilities.h"

class MyTestApp : public OgreBites::ApplicationContextBase, public OgreBites::InputListener
{
  public:
    MyTestApp();
    void setup() override;
    bool keyPressed(const OgreBites::KeyboardEvent &evt) override;
    bool frameRenderingQueued(const Ogre::FrameEvent &evt) override;
    const void *getNextFrame();

  private:
    Ogre::TexturePtr renderTexturePtr;
    Ogre::RenderTexture *renderTexture;
};
