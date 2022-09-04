#include "OgreApp.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

//! [constructor]
MyTestApp::MyTestApp() : OgreBites::ApplicationContextBase("OgreTutorialApp")
{
}
//! [constructor]

//! [key_handler]
bool MyTestApp::keyPressed(const OgreBites::KeyboardEvent &evt)
{
    if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
    {
        getRoot()->queueEndRendering();
    }
    return true;
}
//! [key_handler]

//! [setup]
void MyTestApp::setup(void)
{
    // do not forget to call the base first
    OgreBites::ApplicationContextBase::setup();
    mWindows[0].render->setHidden(true); // IMPORTANT! setting window to hidden

    // Initializing render texture because we will be rendering to this texture
    renderTexturePtr = Ogre::TextureManager::getSingleton().createManual(
        "RenderTexture", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, 640, 480, 0,
        Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET);

    renderTexture = renderTexturePtr->getBuffer()->getRenderTarget();

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

    // and tell it to render into the main window
    // getRenderWindow()->addViewport(cam);
    renderTexture->addViewport(cam);

    // finally something to render
    Ogre::Entity *ent = scnMgr->createEntity("Sinbad.mesh");
    Ogre::SceneNode *node = scnMgr->getRootSceneNode()->createChildSceneNode();
    node->attachObject(ent);
}
//! [setup]

bool MyTestApp::frameRenderingQueued(const Ogre::FrameEvent &evt)
{
    Ogre::LogManager::getSingleton().logMessage("RENDERING");

    return true;
}

const void *MyTestApp::getNextFrame()
{
    // Rendering the frame on top
    getRoot()->renderOneFrame();

    Ogre::Image image(renderTexture->suggestPixelFormat(), renderTexture->getWidth(), renderTexture->getHeight());
    Ogre::PixelBox pb = image.getPixelBox();
    renderTexture->copyContentsToMemory(pb, pb);

    renderTexture->writeContentsToFile("wow.png");

    return image.getData();
}

//! [main]
int main()
{
    MyTestApp app;

    app.initApp();
    while (true)
    {
        if (!app.getRoot()->renderOneFrame())
            break;
    }

    app.closeApp();
    return 0;
}
//! [main]
