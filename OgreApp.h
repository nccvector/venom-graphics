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
    MyTestApp(int externalWindowHandle);
    OgreBites::NativeWindowPair createWindow(const Ogre::String &name, Ogre::uint32 w, Ogre::uint32 h,
                                             Ogre::NameValuePairList miscParams) override;
    void setup() override;
    bool frameRenderingQueued(const Ogre::FrameEvent &evt) override;
    void resize(unsigned int, unsigned int);

  private:
    Ogre::TexturePtr renderTexturePtr;
    Ogre::RenderTexture *renderTexture;
    int pythonWindowHandle;
    Ogre::SceneNode *node;
};
