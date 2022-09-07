#include "OgreApp.h"

//! [constructor]
MyTestApp::MyTestApp(int externalWindowHandle) : OgreBites::ApplicationContextBase("OgreTutorialApp")
{
    pythonWindowHandle = externalWindowHandle;
}

//! [constructor]
OgreBites::NativeWindowPair MyTestApp::createWindow(const Ogre::String &name, Ogre::uint32 w, Ogre::uint32 h,
                                                    Ogre::NameValuePairList miscParams)
{
    OgreBites::NativeWindowPair ret = {NULL, NULL};

    // additional windows should reuse the context
    miscParams["currentGLContext"] = "false";
    miscParams["externalWindowHandle"] = Ogre::StringConverter::toString((size_t)(pythonWindowHandle));
    miscParams["parentWindowHandle"] = Ogre::StringConverter::toString((size_t)(pythonWindowHandle));

    auto p = mRoot->getRenderSystem()->getRenderWindowDescription();
    miscParams.insert(p.miscParams.begin(), p.miscParams.end());
    p.miscParams = miscParams;
    p.name = name;

    if (w > 0 && h > 0)
    {
        p.width = w;
        p.height = h;
    }

    ret.render = mRoot->createRenderWindow(p);
    ret.render->setVisible(true);
    ret.render->setVSyncEnabled(false);

    mWindows.push_back(ret);

    return ret;
}

//! [setup]
void MyTestApp::setup()
{
    bool hideWindow = false;

    // do not forget to call the base first
    OgreBites::ApplicationContextBase::setup();
    if (hideWindow)
    {
        mWindows[0].render->setHidden(true); // IMPORTANT! setting window to hidden

        // Initializing render texture because we will be rendering to this texture
        renderTexturePtr = Ogre::TextureManager::getSingleton().createManual(
            "RenderTexture", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, 640, 480, 0,
            Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET);

        renderTexture = renderTexturePtr->getBuffer()->getRenderTarget();
    }

    // register for input events
    addInputListener(this);

    // get a pointer to the already created root
    Ogre::Root *root = getRoot();
    Ogre::SceneManager *scnMgr = root->createSceneManager();

    // register our scene with the RTSS
    Ogre::RTShader::ShaderGenerator *shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);

    // without light we would just get a black screen
    Ogre::Light *light = scnMgr->createLight("MainLight");
    Ogre::SceneNode *lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->setPosition(0, 10, 15);
    lightNode->attachObject(light);

    // also need to tell where we are
    Ogre::SceneNode *camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    camNode->setPosition(0, 0, 15);
    camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);

    // create the camera
    Ogre::Camera *cam = scnMgr->createCamera("myCam");
    cam->setNearClipDistance(5); // specific to this sample
    cam->setAutoAspectRatio(true);
    camNode->attachObject(cam);

    Ogre::Viewport *vp;

    // and tell it to render into the main window or texture
    if (hideWindow)
        vp = renderTexture->addViewport(cam);
    else
        vp = getRenderWindow()->addViewport(cam);

    vp->setBackgroundColour(Ogre::ColourValue(1, 1, 1));

    // finally something to render
    Ogre::Entity *ent = scnMgr->createEntity("Sinbad.mesh");
    node = scnMgr->getRootSceneNode()->createChildSceneNode();
    node->attachObject(ent);
}
//! [setup]

Ogre::Quaternion ToQuaternion(double yaw, double pitch, double roll) // yaw (Z), pitch (Y), roll (X)
{
    // Abbreviations for the various angular functions
    double cy = cos(yaw * 0.5);
    double sy = sin(yaw * 0.5);
    double cp = cos(pitch * 0.5);
    double sp = sin(pitch * 0.5);
    double cr = cos(roll * 0.5);
    double sr = sin(roll * 0.5);

    Ogre::Quaternion q;
    q.w = cr * cp * cy + sr * sp * sy;
    q.x = sr * cp * cy - cr * sp * sy;
    q.y = cr * sp * cy + sr * cp * sy;
    q.z = cr * cp * sy - sr * sp * cy;

    return q;
}

bool MyTestApp::frameRenderingQueued(const Ogre::FrameEvent &evt)
{
    // Ogre::LogManager::getSingleton().logMessage("RENDERING ++");

    static float ry = 0;
    ry += 0.5 * evt.timeSinceLastFrame;
    Ogre::Quaternion q = ToQuaternion(0, ry, 0);
    node->setOrientation(q.w, q.x, q.y, q.z);

    return true;
}

void MyTestApp::resize(unsigned int x, unsigned int y)
{
    getRenderWindow()->resize(x, y);
}