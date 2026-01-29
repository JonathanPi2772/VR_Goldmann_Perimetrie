// "WaveVR SDK
// © 2017 HTC Corporation. All Rights Reserved.
//
// Unless otherwise required by copyright law and practice,
// upon the execution of HTC SDK license agreement,
// HTC grants you access to and use of the WaveVR SDK(s).
// You shall fully comply with all of HTC’s SDK license agreement terms and
// conditions signed by you and all SDK and API requirements,
// specifications, and documentation provided by HTC to You."
#include <iostream>
#include <log.h>
#include <string.h>
#include <unistd.h>
#include <limits>
#include <sys/types.h>
#include <sys/time.h>
#include <cstdlib>
#include <math.h>
//#include <Texture.h>
//#include <Picture.h>
//#include <SkyBox.h>
#include <Sky.h>
#include <Settings.h>
#include <Meteoroid.h>
#include <Stars.h>
#include <Terrain.h>
//#include <ControllerAxes.h>
//#include <ControllerCube.h>
#include <Controller.h>
//#include <ReticlePointer.h>
#include <FrameBufferObject.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3.h>

#include <wvr/wvr.h>
#include <wvr/wvr_hand_render_model.h>
#include <wvr/wvr_ctrller_render_model.h>
#include <wvr/wvr_render.h>
#include <wvr/wvr_device.h>
#include <wvr/wvr_projection.h>
#include <wvr/wvr_overlay.h>
#include <wvr/wvr_system.h>
#include <wvr/wvr_events.h>
//#include <RaySphereIntersection.h>

//#include "scene/HandManager.h"

#include "hellovr.h"

// Return micro second.  Should always positive because now is bigger.
#define timeval_subtract(now, last) \
    ((now.tv_sec - last.tv_sec) * 1000000LL + now.tv_usec - last.tv_usec)

#undef LOGENTRY
#define LOGENTRY(...)

using namespace std;

bool gDebug = true;
bool gDebugOld = gDebug;
bool gMsaa = true;
bool gScene = false;
bool gSceneOld = gScene;
bool gUseScale = true;
float gScale = 1;

#define LOGDIF(args...) if (gDebug) LOGD(args)

#define VR_MAX_CLOCKS 200

// To demonstrate how to use heavy-effect foveated rendering
#define ENABLE_LOW_FOVEATED_RENDERING 0

// To demonstrate how to use WaveVR AdaptiveQuality
#define DISABLE_ADAPTIVE_QUALITY 0

static void dumpMatrix(const char * name, const Matrix4& mat) {
    const float * ptr = mat.get();
    LOGD("%s =\n"
        " ⎡%+6f  %+6f  %+6f  %+6f⎤\n"
        " ⎢%+6f  %+6f  %+6f  %+6f⎥\n"
        " ⎢%+6f  %+6f  %+6f  %+6f⎥\n"
        " ⎣%+6f  %+6f  %+6f  %+6f⎦\n",
        name,
        ptr[0], ptr[4], ptr[8],  ptr[12],
        ptr[1], ptr[5], ptr[9],  ptr[13],
        ptr[2], ptr[6], ptr[10], ptr[14],
        ptr[3], ptr[7], ptr[11], ptr[15]);
}

MainApplication::MainApplication()
        : mControllerCount(0)
        , mControllerCount_Last(-1)
        , mValidPoseCount(0)
        , mValidPoseCount_Last(-1)
        , mPoseClasses("")
        , m3DOF(true)
        , mMove(true)
        , mLight(true)
        , mTimeDiff(0.0f)
        , mDriveSpeed(0.0f)
        , mDriveAngle(0.0f)
        , mScene(-1)
        , mIndexLeft(0)
        , mIndexRight(0)
        , mLeftEyeQ(NULL)
        , mRightEyeQ(NULL)
        , mShouldQuit(false)
        , mCurFocusController(WVR_DeviceType_HMD){
    // other initialization tasks are done in init
    memset(mDevClassChar, 0, sizeof(mDevClassChar));
    mStars = NULL;
    mSky = NULL;
    mMeteoroid = NULL;
    mSphere=NULL;
    // mFloor=NULL;
    mGridPicture = NULL;
    //mReticlePointer = NULL;
    mActiveEye=0; //0 for not started 1 for right, 2 for left
    // Initialize Menu Panel
    // Position: x=0, y=1.5 (eye level), z=-2.0 (2 meters in front)
    // Size: Width=1.5m, Height=1.0m
    mMenuHeight = 8.0;
    mMenuWidth = 10.0;
    mMenuPosition = Vector3(0.0f, 0.0f, -15.0f);
    mStartMenu = NULL;
    mLeftEyeMenu = NULL;
    mRightEyeMenu = NULL;
    mEndMenu = NULL;
    mShowStartMenu = true;
    mShowRightEyeMenu= false;
    mShowLeftEyeMenu = false;
    mShowEndMenu = false;
    allDataSaved = false;

    //paused Menu
    mPauseMenu = NULL;
    mShowPauseMenu = false;
    realPausedReleased = false;
    mPausedReleased = std::chrono::high_resolution_clock::now();



    std::random_device rd;
    m_rng.seed(rd());


#if defined(USE_CONTROLLER) || defined(USE_CUSTOM_CONTROLLER)
    mControllerObjs[0] = nullptr;
    mControllerObjs[1] = nullptr;
#else
    mControllerAxes = NULL;
    memset(mControllerCubeTableById, 0, sizeof(mControllerCubeTableById));
#endif
    LOGI("MainApplication::MainApplication()");
}

MainApplication::~MainApplication() {
    // work is done in Shutdown
    LOGI("Shutdown");
}

static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

bool MainApplication::initVR() {
    LOGENTRY();
    LOGI("MainApplication::initVR()");
    // Loading the WVR Runtime
    WVR_InitError eError = WVR_InitError_None;
    LOGI("initVR():start call WVR_Init");
    eError = WVR_Init(WVR_AppType_VRContent);
    LOGI("initVR():start call WVR_Init end");
    if (eError != WVR_InitError_None) {
        LOGE("Unable to init VR runtime: %s", WVR_GetInitErrorString(eError));
        return false;
    }

//---------- Key Mapping Sample --------
/*
    WVR_InputAttribute inputIdAndTypes[] = {
        {WVR_InputId_Alias1_Menu, WVR_InputType_Button, WVR_AnalogType_None},
        {WVR_InputId_Alias1_Touchpad, WVR_InputType_Button | WVR_InputType_Touch | WVR_InputType_Analog, WVR_AnalogType_2D},
        {WVR_InputId_Alias1_Trigger, WVR_InputType_Button , WVR_AnalogType_None},
        {WVR_InputId_Alias1_Bumper, WVR_InputType_Button , WVR_AnalogType_None}
    };
    WVR_SetInputRequest(WVR_DeviceType_HMD, inputIdAndTypes, sizeof(inputIdAndTypes) / sizeof(*inputIdAndTypes));
    WVR_SetInputRequest(WVR_DeviceType_Controller_Right, inputIdAndTypes, sizeof(inputIdAndTypes) / sizeof(*inputIdAndTypes));
    WVR_SetInputRequest(WVR_DeviceType_Controller_Left, inputIdAndTypes, sizeof(inputIdAndTypes) / sizeof(*inputIdAndTypes));
*/
//---------- Full key mapping Sample (you can receive all keyevent)--------
    WVR_InputAttribute array[31];
    for (int i = 0; i < sizeof(array) / sizeof(*array); i++) {
        array[i].id = (WVR_InputId)(i + 1);
        array[i].capability = WVR_InputType_Button | WVR_InputType_Touch | WVR_InputType_Analog;
        array[i].axis_type = WVR_AnalogType_1D;
    }
    WVR_SetInputRequest(WVR_DeviceType_HMD, array, sizeof(array) / sizeof(*array));
    WVR_SetInputRequest(WVR_DeviceType_Controller_Right, array, sizeof(array) / sizeof(*array));
    WVR_SetInputRequest(WVR_DeviceType_Controller_Left, array, sizeof(array) / sizeof(*array));

    WVR_SetArmModel(WVR_SimulationType_Auto); //add for use arm mode.

    // Must initialize render runtime before all OpenGL code.
    WVR_RenderInitParams_t param;
    param = { WVR_GraphicsApiType_OpenGL, WVR_RenderConfig_Default };

    WVR_RenderError pError = WVR_RenderInit(&param);
    if (pError != WVR_RenderError_None) {
        LOGE("Present init failed - Error[%d]", pError);
    }

    mInteractionMode = WVR_GetInteractionMode();
    mGazeTriggerType = WVR_GetGazeTriggerType();
    LOGI("initVR() mInteractionMode: %d, mGazeTriggerType: %d", mInteractionMode, mGazeTriggerType);
    return true;
}

bool MainApplication::initGL() {
    LOGENTRY();
    mNearClip = 0.1f;
    mFarClip = 350.0f; // Was initial at 30.0f
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(true);
    mLUV[0] = 0.0f;
    mLUV[1] = 0.0f;
    mUUV[0] = gScale;
    mUUV[1] = gScale;
    GLenum glerr = glGetError();
    if (glerr != GL_NO_ERROR) {
        LOGE("glGetError() before initGL: %d", glerr);
    }

#define OBJ_ERROR_CHECK(obj) if (obj->hasError() || obj->hasGLError()) return false

    // Floor not necessary any more
    // mFloor = new Floor();
    // OBJ_ERROR_CHECK(mFloor);

    oriSpherePos=GENERAL_THALES_POINT_VEC; // Focuspoinht Start pos
    mSphere = new Sphere(oriSpherePos);
    OBJ_ERROR_CHECK(mSphere);

    // Textboxes

    // Start Menu
    mStartMenu = new Panel(mMenuPosition, mMenuWidth, mMenuHeight);
    mStartMenu->setTexture("textures/StartBox.png"); // Make sure this image exists!
    mShowStartMenu = true; // Start with menu showing
    // Right Eye
    mRightEyeMenu = new Panel(mMenuPosition, mMenuWidth, mMenuHeight);
    mRightEyeMenu->setTexture("textures/RightEyeBox.png");
    mShowRightEyeMenu = false;
    // Left Eye
    mLeftEyeMenu = new Panel(mMenuPosition, mMenuWidth, mMenuHeight);
    mLeftEyeMenu->setTexture("textures/LeftEyeBox.png");
    mShowLeftEyeMenu = false;
    // EndMenu
    mEndMenu = new Panel(mMenuPosition, mMenuWidth, mMenuHeight);
    mEndMenu->setTexture("textures/EndBox.png");
    mShowEndMenu = false;

    // Pause Menu
    mPauseMenu = new SkySphere();
    mPauseMenu->setTexture("textures/PauseMenu.png");
    mShowPauseMenu = false;
    realPausedReleased = false;
    mPausedReleased = std::chrono::high_resolution_clock::now();



    // Setup Scenes
    mStars = new Stars(gDebug);
    OBJ_ERROR_CHECK(mStars);
    mSky = new Sky(gDebug);
    OBJ_ERROR_CHECK(mSky);
    mMeteoroid = new Meteoroid();
    OBJ_ERROR_CHECK(mMeteoroid);
    mTerrain = new Terrain(gDebug);
    OBJ_ERROR_CHECK(mTerrain);
    mLightDir = Vector4(0, 1, 0, 0);

    // Set Balancing Eye
    std::bernoulli_distribution coin_flip(0.5);
    mFirstEye = coin_flip(m_rng) ? 1 : 2;
    /*
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    float rndNumber = dist(m_rng);
    if (rndNumber < 0.5f) {
        mFirstEye = 1;
    } else {
        mFirstEye = 2;
    }
    */


#if defined(USE_CONTROLLER)
    mControllerObjs[0] = new Controller(WVR_DeviceType_Controller_Right);
    mControllerObjs[1] = new Controller(WVR_DeviceType_Controller_Left);
#elif defined(USE_CUSTOM_CONTROLLER)
    mControllerObjs[0] = new CustomController(WVR_DeviceType_Controller_Right);
    mControllerObjs[1] = new CustomController(WVR_DeviceType_Controller_Left);
#else
    mControllerAxes = new ControllerAxes();
    OBJ_ERROR_CHECK(mControllerAxes);
#endif

    //mReticlePointer = new ReticlePointer();
    //OBJ_ERROR_CHECK(mReticlePointer);

    setupCameras();

    // Setup stereo render targets
    WVR_GetRenderTargetSize(&mRenderWidth, &mRenderHeight);
    LOGD("Recommended size is %ux%u", mRenderWidth, mRenderHeight);
    if (mRenderWidth == 0 || mRenderHeight == 0) {
        LOGE("Please check server configure");
        return false;
    }

    mIndexLeft = 0;
    mIndexRight = 0;

    mLeftEyeQ = WVR_ObtainTextureQueue(WVR_TextureTarget_2D, WVR_TextureFormat_RGBA, WVR_TextureType_UnsignedByte, mRenderWidth, mRenderHeight, 0);
    for (int i = 0; i < WVR_GetTextureQueueLength(mLeftEyeQ); i++) {
        FrameBufferObject* fbo;

        fbo = new FrameBufferObject((int)(long)WVR_GetTexture(mLeftEyeQ, i).id, mRenderWidth, mRenderHeight, true);
        if (!fbo) return false;
        if (fbo->hasError())  {
            delete fbo;
            return false;
        }
        mLeftEyeFBOMSAA.push_back(fbo);

        fbo = new FrameBufferObject((int)(long)WVR_GetTexture(mLeftEyeQ, i).id, mRenderWidth, mRenderHeight);
        if (!fbo) return false;
        if (fbo->hasError())  {
            delete fbo;
            return false;
        }
        mLeftEyeFBO.push_back(fbo);
    }
    mRightEyeQ = WVR_ObtainTextureQueue(WVR_TextureTarget_2D, WVR_TextureFormat_RGBA, WVR_TextureType_UnsignedByte, mRenderWidth, mRenderHeight, 0);
    for (int i = 0; i < WVR_GetTextureQueueLength(mRightEyeQ); i++) {
        FrameBufferObject* fbo;

        fbo = new FrameBufferObject((int)(long)WVR_GetTexture(mRightEyeQ, i).id, mRenderWidth, mRenderHeight, true);
        if (!fbo) return false;
        if (fbo->hasError())  {
            delete fbo;
            return false;
        }
        mRightEyeFBOMSAA.push_back(fbo);

        fbo = new FrameBufferObject((int)(long)WVR_GetTexture(mRightEyeQ, i).id, mRenderWidth, mRenderHeight);
        if (!fbo) return false;
        if (fbo->hasError())  {
            delete fbo;
            return false;
        }
        mRightEyeFBO.push_back(fbo);
	}

#if defined(USE_CONTROLLER) || defined(USE_CUSTOM_CONTROLLER)
    setupControllers();
#else
    setupControllerCubes();
#endif
#undef OBJ_ERROR_CHECK

    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

#if ENABLE_LOW_FOVEATED_RENDERING
    WVR_RenderFoveationMode(WVR_FoveationMode_Enable);
#endif

#if defined(DISABLE_ADAPTIVE_QUALITY) && DISABLE_ADAPTIVE_QUALITY
    // 1. WaveVR AQ is enabled with WVR_QualityStrategy_SendQualityEvent by default from WaveVR SDK 3.2.0
    // 2. WVR_EnableAdaptiveQuality must be invoked after WVR_RenderInit().
    // 3. If you don't want to use AQ, you need to disable it.
    WVR_EnableAdaptiveQuality(false);

    /* There are some strategies in WaveVR AQ can to choose.
     *
     * EX: Use auto foveated rendering to improve performance while it is not good enough.
     * WVR_EnableAdaptiveQuality(true, WVR_QualityStrategy_AutoFoveation);
     *
     * EX: Receive recommended quailty chagned event and auto foveated rendering to improve performance while
     *     it is not good enough.
     * WVR_EnableAdaptiveQuality(true, WVR_QualityStrategy_SendQualityEvent | WVR_QualityStrategy_AutoFoveation);
     */
#endif
    /*mHandManager = new HandManager(WVR_HandTrackerType_Natural);
    mHandManager->onCreate();
    mHandManager->registerPinchReleasedCallback(
        std::bind(&MainApplication::handleHandInput, this, std::placeholders::_1));
    */
    // Initialize eye tracking
    if (!initEyeTracking()) {
        LOGW("Eye tracking not available, continuing without it");
    }
    return true;
}

void MainApplication::shutdownGL() {
    LOGENTRY();

    shutdownEyeTracking();
    /*mHandManager->onDestroy();
    delete mHandManager;
    mHandManager = nullptr;
     */
    /*
    if (mFloor != NULL )
        delete mFloor;
    mFloor = NULL;
     */

    if (mSphere != NULL )
        delete mSphere;
    mSphere = NULL;

    if (mSky != NULL)
        delete mSky;
    mSky = NULL;

    if (mMeteoroid != NULL)
        delete mMeteoroid;
    mMeteoroid = NULL;

    if (mStars != NULL)
        delete mStars;
    mStars = NULL;

    if (mTerrain != NULL)
        delete mTerrain;
    mTerrain = NULL;

    if (mGridPicture != NULL)
        delete mGridPicture;
    mGridPicture = NULL;


#if defined(USE_CONTROLLER) || defined(USE_CUSTOM_CONTROLLER)
    for (uint32_t cID = 0; cID < 2; ++cID) {
        if (mControllerObjs[cID] != nullptr) {
            delete mControllerObjs[cID];
        }
        mControllerObjs[cID] = nullptr;
    }
#else
    if (mControllerAxes != NULL)
        delete mControllerAxes;
    mControllerAxes = NULL;

    for (int i = 0; i < WVR_DEVICE_COUNT_LEVEL_1; i++) {
        if (mControllerCubeTableById[i] != NULL) {
            delete mControllerCubeTableById[i];
            mControllerCubeTableById[i] = NULL;
        }
    }
    mControllerCubes.clear();
#endif
    /*
    if (mReticlePointer != NULL)
        delete mReticlePointer;
    mReticlePointer = NULL;
     */

    if (mLeftEyeQ != 0) {
        for (int i = 0; i < WVR_GetTextureQueueLength(mLeftEyeQ); i++) {
            delete mLeftEyeFBOMSAA.at(i);
            delete mLeftEyeFBO.at(i);
        }
        WVR_ReleaseTextureQueue(mLeftEyeQ);
    }

    if (mRightEyeQ != 0) {
        for (int i = 0; i < WVR_GetTextureQueueLength(mRightEyeQ); i++) {
            delete mRightEyeFBOMSAA.at(i);
            delete mRightEyeFBO.at(i);
        }
        WVR_ReleaseTextureQueue(mRightEyeQ);
    }
}

void MainApplication::shutdownVR() {
    WVR_Quit();
}



class ControllerState {
private:
    bool mLastTouchpadPressed;
    bool mTouchpadClicked;
    bool mMenuClicked;
    bool mLastMenuPressed;

public:

    ControllerState() : mLastTouchpadPressed(false), mTouchpadClicked(false), mLastMenuPressed(false), mMenuClicked(false) {
    }

    void updateState(WVR_DeviceType deviceType) {
        bool bTouchpadPressed = WVR_GetInputButtonState(deviceType, WVR_InputId_Alias1_Touchpad);
        if (mLastTouchpadPressed && (bTouchpadPressed) == 0) {
            mTouchpadClicked = true;
        } else {
            mTouchpadClicked = false;
        }
        mLastTouchpadPressed = bTouchpadPressed;

        bool bMenuPressed = WVR_GetInputButtonState(deviceType, WVR_InputId_Alias1_Menu);
        if (mLastMenuPressed && (bMenuPressed) == 0) {
            mMenuClicked = true;
        } else {
            mMenuClicked = false;
        }
        mLastMenuPressed = bMenuPressed;
    }

    inline bool touchpadClicked() const {
        return mTouchpadClicked;
    }

    inline bool menuClicked() const {
        return mMenuClicked;
    }
};

//-----------------------------------------------------------------------------
// Purpose: Poll events.  Quit application if return true.
//-----------------------------------------------------------------------------
bool MainApplication::handleInput() {
    LOGENTRY();
    if (mShouldQuit) {
        return true; // This breaks the loop in jni.cpp
    }
    static ControllerState states[WVR_DEVICE_COUNT_LEVEL_1];

    bool resolutionChange = false;

    if (gScene != gSceneOld) {
        gSceneOld = gScene;
    }

    if (gDebug != gDebugOld) {
        gDebugOld = gDebug;
        // mSkyBox->setDebug(gDebug);
        // mLightDir = mSkyBox->getLightDir();
        setupCameras();
    }

    // Process WVR events
    bool isCtrlerStatusChange = false;
    WVR_Event_t event;
    while(WVR_PollEventQueue(&event)) {
        if (event.common.type == WVR_EventType_Quit) {
            return true;
        }

        if (event.common.type == WVR_EventType_DeviceConnected ||
            event.common.type == WVR_EventType_DeviceDisconnected) {
            isCtrlerStatusChange = true;
            if (event.common.type == WVR_EventType_DeviceConnected) {
                LOGI("WVR_EventType_DeviceConnected");
            } else {
                LOGI("WVR_EventType_DeviceDisconnected");
            }
        }
        processVREvent(event);

#if defined(USE_CONTROLLER) || defined(USE_CUSTOM_CONTROLLER)
            handleControllerConnectEvent(isCtrlerStatusChange);
#endif

        //mHandManager->handleHandTrackingMechanism();

        if (event.common.type == WVR_EventType_ButtonPressed) {
            if (
                    (event.device.deviceType == WVR_DeviceType_Controller_Right) or
                    (event.device.deviceType == WVR_DeviceType_Controller_Left)) {
                if (
                        (event.input.inputId == WVR_InputId_Alias1_Bumper) or
                        (event.input.inputId == WVR_InputId_Alias1_Trigger) or
                        (event.input.inputId == WVR_InputId_Alias1_Touchpad)
                ) {
                    if (mMeteoroid->m_perimetry_status == "running")
                        mMeteoroid->point_detected();

                    if (mShowRightEyeMenu) {
                        mShowRightEyeMenu = false;
                        mMeteoroid->start_animation();
                        mActiveEye = 1;
                        mMeteoroid->mActiveEye = 1;
                    }

                    if (mShowLeftEyeMenu) {
                        mShowLeftEyeMenu = false;
                        mMeteoroid->start_animation();
                        mActiveEye = 2;
                        mMeteoroid->mActiveEye = 2;
                    }

                    if (mShowStartMenu) {
                        mShowStartMenu = false;
                        if (mFirstEye == 1) {
                            mShowRightEyeMenu = true;
                        } else if (mFirstEye == 2) {
                            mShowLeftEyeMenu = true;
                        }
                    }

                    if (mShowEndMenu) {
                        mShowEndMenu = false;
                        mActiveEye = 0;
                        mMeteoroid->mActiveEye = 0;
                        CloseApplication();
                    }
                // If A or X Button is Pressed
                } else if ((event.input.inputId == WVR_InputId_Alias1_A) or
                           (event.input.inputId == WVR_InputId_Alias1_X)) {
                    if (mShowPauseMenu) {
                        realPausedReleased = true;
                        mShowPauseMenu = false;
                        mPausedReleased = std::chrono::high_resolution_clock::now();
                    } else {
                        mShowPauseMenu = true;
                        mMeteoroid->pause_animation(false);
                    }
                }
            }
        }
    }
    if (resolutionChange) {
        switchResolution();
    }
    return false;
}

/*void MainApplication::handleHandInput(HandTypeEnum handType) {
    if (mSphere != nullptr && mHandManager != nullptr) {
        if (mPointToSphere == true) {
            Vector3 pos;
            if (handType == Hand_Right) {
                if(mSphere->getCenter() == oriSpherePos){
                    pos = Vector3(1,0,0)+mSphere->getCenter();
                }else{
                    pos = oriSpherePos;
                }
                mSphere->setSpherePos(pos);
            } else if (handType == Hand_Left) {
                if (mSphere->getCenter() == oriSpherePos) {
                    pos = Vector3(-1,0,0) + mSphere->getCenter();
                } else {
                    pos = oriSpherePos;
                }
                mSphere->setSpherePos(pos);
            }
        }
    }
}*/

void MainApplication::switchResolution() {
#if ENABLE_LOW_FOVEATED_RENDERING
    LOGI("menu key pressed");
#else
    if (gUseScale == true) {
            if (std::abs(gScale - 1.0) <= std::numeric_limits<float>::epsilon()) {
                gScale = 0.5;
            } else {
                gScale += 0.1;
            }
            mUUV[0] = gScale;
            mUUV[1] = gScale;

            FrameBufferObject* fbo=NULL;

            for (int i = 0; i < WVR_GetTextureQueueLength(mLeftEyeQ); i++) {
                fbo = gMsaa ? mLeftEyeFBOMSAA.at(i) : mLeftEyeFBO.at(i);
                fbo->resizeFrameBuffer(gScale);
            }

            for (int i = 0; i < WVR_GetTextureQueueLength(mRightEyeQ); i++) {
                fbo = gMsaa ? mRightEyeFBOMSAA.at(i) : mRightEyeFBO.at(i);
                fbo->resizeFrameBuffer(gScale);
            }
    }
#endif
}

//-----------------------------------------------------------------------------
// Purpose: Processes a single VR event
//-----------------------------------------------------------------------------
void MainApplication::processVREvent(const WVR_Event_t & event) {
    switch(event.common.type) {
    case WVR_EventType_DeviceConnected:
        {
#if defined(USE_CONTROLLER) || defined(USE_CUSTOM_CONTROLLER)
#else
            setupControllerCubeForDevice(event.device.deviceType);
            LOGD("Device %u attached. Setting up controller cube.\n", event.device.deviceType);
#endif
        }
        break;
    case WVR_EventType_ButtonPressed:
    case WVR_EventType_ButtonUnpressed:
    case WVR_EventType_TouchTapped:
    case WVR_EventType_TouchUntapped:
        {
#if defined(USE_CONTROLLER)
            WVR_DeviceType ctrlType = event.device.deviceType;
            LOGI("Device Btn Event:C(%d)EType(%d)", ctrlType, event.common.type);
            for (uint32_t cID = 0; cID < 2; ++cID) {
                if (mControllerObjs[cID] != nullptr) {
                    if (mControllerObjs[cID]->isThisCtrlerType(ctrlType) == true) {
                        mControllerObjs[cID]->refreshButtonStatus(event);
                    }
                }
            }
#else
#endif
        }
        break;
    case WVR_EventType_DeviceRoleChanged:
        {
#if defined(USE_CONTROLLER) || defined(USE_CUSTOM_CONTROLLER)
            LOGI("DeviceRoleChanged.");
            for (uint32_t cID = 0; cID < 2; ++cID) {
                if (mControllerObjs[cID] != nullptr) {
                    mControllerObjs[cID]->switchCtrlerType();
                }
            }
#else
            LOGD("Device %u role change.\n", event.device.deviceType);
#endif
        }
        break;
    case WVR_EventType_DeviceStatusUpdate:
        {
            LOGD("Device %u updated.\n", event.device.deviceType);
#if defined(USE_CONTROLLER) || defined(USE_CUSTOM_CONTROLLER)
#else
            // Delete if the controller cube of device id already exists and is not real.
            int index = event.device.deviceType - WVR_DeviceType_HMD;  // Shift the HMD base as index
            if (index > WVR_DEVICE_HMD && index < WVR_DEVICE_COUNT_LEVEL_1) {
                if (mControllerCubeTableById[index] != NULL) {
                    ControllerCube *pControllerCube = mControllerCubeTableById[index];
                    for (std::vector<ControllerCube *>::iterator iter = mControllerCubes.begin(); iter != mControllerCubes.end(); iter++) {
                        if ((*iter) == pControllerCube) {
                            LOGD("Find empty controller cube, delete it.");
                            mControllerCubes.erase(iter);
                            break;
                        }
                    }
                    LOGI("Clear the controller cube of device[%d]", index);
                    delete pControllerCube;
                    mControllerCubeTableById[index] = NULL;
                }
                setupControllerCubeForDevice(event.device.deviceType);
            }
#endif
        }
        break;
    case WVR_EventType_IpdChanged:
        {
            WVR_RenderProps_t props;
            bool ret = WVR_GetRenderProps(&props);
            float ipd = 0;
            if (ret) {
                ipd = props.ipdMeter;
            }
            LOGI("IPD is changed (%.4f) and renew the transform from eye to head.", ipd);
            mProjectionLeft = wvrmatrixConverter(
                WVR_GetProjection(WVR_Eye_Left, mNearClip, mFarClip));
            mProjectionRight = wvrmatrixConverter(
                WVR_GetProjection(WVR_Eye_Right, mNearClip, mFarClip));

            mEyePosLeft = wvrmatrixConverter(
                WVR_GetTransformFromEyeToHead(WVR_Eye_Left)).invert();
            mEyePosRight = wvrmatrixConverter(
                WVR_GetTransformFromEyeToHead(WVR_Eye_Right)).invert();

            dumpMatrix("ProjectionLeft", mProjectionLeft);
            dumpMatrix("ProjectionRight", mProjectionRight);
            dumpMatrix("EyePosLeft", mEyePosLeft);
            dumpMatrix("EyePosRight", mEyePosRight);
        }
        break;
    case WVR_EventType_InteractionModeChanged:
        {
            mInteractionMode = WVR_GetInteractionMode();
            LOGI("Receive WVR_EventType_InteractionModeChanged mode = %d", mInteractionMode);
        }
        break;
    case WVR_EventType_GazeTriggerTypeChanged:
        {
            mGazeTriggerType = WVR_GetGazeTriggerType();
            LOGI("Receive WVR_EventType_GazeTriggerTypeChanged type = %d", mGazeTriggerType);
        }
        break;
    // If WaveVR AQ have enabled with WVR_QualityStrategy_SendQualityEvent, you will receive
    // the wvr recommendedQuality_Lower/Higher event based on rendering performance.
    case WVR_EventType_RecommendedQuality_Lower:
        {
            /* Once you got this recommend event:
             * 1. If your conetent rendering quality has already in the worst quality, you can ignore this event.
             * 2. Or, you can adjust the rendering resolution lower gradually by re-create texture queue, disable MSAA, etc.
             */
            LOGI("[Sample] Get WVR_EventType_RecommendedQuality_Lower");
        }
        break;
    case WVR_EventType_RecommendedQuality_Higher:
        {
            /* 1. Once you got this recommend event:
             * 1. If your conetent rendering quality has already in the best quality, you can ignore this event.
             * 2. Or, you can adjust the rendering resolution higher ASAP by re-create texture queue, enable MSAA, etc.
             */
            LOGI("[Sample] Get WVR_EventType_RecommendedQuality_Higher");
        }
        break;
    default:
        break;
    }
}

#if defined(USE_CONTROLLER) || defined(USE_CUSTOM_CONTROLLER)
void MainApplication::handleControllerConnectEvent(bool iIsCtrlerStatusChanged)
{
    //1. check controller connection state, if controller is connected, we loading. Otherwise, we release.
    //*** reload empty controller won't memory leak because we will release resource before we initialize graphics res.
    //*** release empty controller won't cause crash because we will check validation resource is exist before we release.
    if (iIsCtrlerStatusChanged == true) {
        LOGI("[APCtrler] Trigger connect or disconnect.");
        for (uint32_t cID = 0; cID < 2; ++cID) {
            if (mControllerObjs[cID] != nullptr) {
                //2.1 get ctrler device type.
                WVR_DeviceType ctrlerType = mControllerObjs[cID]->getCtrlerType();
                bool ctrlerConStatus = WVR_IsDeviceConnected(ctrlerType);
                if (ctrlerConStatus == true) {// connect
#if defined(USE_CONTROLLER)
                    mControllerObjs[cID]->loadControllerModelAsync();
#else
                    mControllerObjs[cID]->loadControllerEmitterAsync();
#endif
                } else {
#if defined(USE_CONTROLLER)
                    mControllerObjs[cID]->handleDisconnected();
#endif
                }
            }
        }
    }
}
#endif

bool MainApplication::renderFrame() {
    LOGENTRY();

    unsigned int ext = WVR_SubmitExtend_Default;

	mIndexLeft = WVR_GetAvailableTextureIndex(mLeftEyeQ);
    mIndexRight = WVR_GetAvailableTextureIndex(mRightEyeQ);


    updateEyeTracking();
    /*
    //LOGD("renderFrame start");
    // for now as fast as possible
    drawControllers();m

    if (mInteractionMode == WVR_InteractionMode_Gaze) {
        drawReticlePointer();
    }*/
    renderStereoTargets();
    ext |= WVR_SubmitExtend_Default;
#if ENABLE_LOW_FOVEATED_RENDERING
#else
    if (gScale < 1 && gScale > 0)
        ext |= WVR_SubmitExtend_PartialTexture;
#endif

    WVR_TextureParams_t leftEyeTexture = WVR_GetTexture(mLeftEyeQ, mIndexLeft);
    WVR_SubmitError e;

    leftEyeTexture.layout.leftLowUVs.v[0] = 0;
    leftEyeTexture.layout.leftLowUVs.v[1] = 0;
    leftEyeTexture.layout.rightUpUVs.v[0] = 1;
    leftEyeTexture.layout.rightUpUVs.v[1] = 1;
#if ENABLE_LOW_FOVEATED_RENDERING
#else
        if (gScale < 1 && gScale > 0) {
            leftEyeTexture.layout.leftLowUVs.v[0] = mLUV[0];
            leftEyeTexture.layout.leftLowUVs.v[1] = mLUV[1];
            leftEyeTexture.layout.rightUpUVs.v[0] = mUUV[0];
            leftEyeTexture.layout.rightUpUVs.v[1] = mUUV[1];
        }
#endif
        e = WVR_SubmitFrame(WVR_Eye_Left, &leftEyeTexture, &(mVRDevicePairs[WVR_DEVICE_HMD].pose), (WVR_SubmitExtend)ext);
        if (e != WVR_SubmitError_None) return true;

        // Right eye
        WVR_TextureParams_t rightEyeTexture = WVR_GetTexture(mRightEyeQ, mIndexRight);

        rightEyeTexture.layout.leftLowUVs.v[0] = 0;
        rightEyeTexture.layout.leftLowUVs.v[1] = 0;
        rightEyeTexture.layout.rightUpUVs.v[0] = 1;
        rightEyeTexture.layout.rightUpUVs.v[1] = 1;

#if ENABLE_LOW_FOVEATED_RENDERING
#else
        if (gScale < 1 && gScale > 0) {
            rightEyeTexture.layout.leftLowUVs.v[0] = mLUV[0];
            rightEyeTexture.layout.leftLowUVs.v[1] = mLUV[1];
            rightEyeTexture.layout.rightUpUVs.v[0] = mUUV[0];
            rightEyeTexture.layout.rightUpUVs.v[1] = mUUV[1];
        }
#endif
        e = WVR_SubmitFrame(WVR_Eye_Right, &rightEyeTexture, &(mVRDevicePairs[WVR_DEVICE_HMD].pose), (WVR_SubmitExtend)ext);
        if (e != WVR_SubmitError_None) return true;

    updateTime();

    // Clear
    {
        // We want to make sure the glFinish waits for the entire present to complete, not just the submission
        // of the command. So, we do a clear here right here so the glFinish will wait fully for the swap.
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    // Spew out the controller and pose count whenever they change.
    if (mControllerCount != mControllerCount_Last || mValidPoseCount != mValidPoseCount_Last) {
        mValidPoseCount_Last = mValidPoseCount;
        mControllerCount_Last = mControllerCount;

        LOGD("PoseCount:%d(%s) Controllers:%d\n", mValidPoseCount, mPoseClasses.c_str(), mControllerCount);
    }

    usleep(1);
    //LOGD("renderFrame end");

    return false;
}

void MainApplication::setupCameras() {
    mProjectionLeft = wvrmatrixConverter(
        WVR_GetProjection(WVR_Eye_Left, mNearClip, mFarClip));
    mProjectionRight = wvrmatrixConverter(
        WVR_GetProjection(WVR_Eye_Right, mNearClip, mFarClip));

    mEyePosLeft = wvrmatrixConverter(
        WVR_GetTransformFromEyeToHead(WVR_Eye_Left)).invert();
    mEyePosRight = wvrmatrixConverter(
        WVR_GetTransformFromEyeToHead(WVR_Eye_Right)).invert();

    dumpMatrix("ProjectionLeft", mProjectionLeft);
    dumpMatrix("ProjectionRight", mProjectionRight);
    dumpMatrix("EyePosLeft", mEyePosLeft);
    dumpMatrix("EyePosRight", mEyePosRight);

    // Initial position
    mWorldTranslation.identity().setColumn(3, Vector4(0.0f, 0.0f, 0.0f, 1));
    mWorldRotation = 0;
    gettimeofday(&mRtcTime, NULL);
}

void MainApplication::renderStereoTargets() {
    LOGENTRY();
    glClearColor(0.30f, 0.30f, 0.37f, 1.0f); // nice background color, but not black
    FrameBufferObject * fbo = NULL;

    fbo = gMsaa ? mLeftEyeFBOMSAA.at(mIndexLeft) : mLeftEyeFBO.at(mIndexLeft);
    fbo->bindFrameBuffer();

    WVR_TextureParams_t leftEyeTexture = WVR_GetTexture(mLeftEyeQ, mIndexLeft);
#if ENABLE_LOW_FOVEATED_RENDERING
        WVR_RenderFoveationParams_t foveated;
        foveated.focalX = foveated.focalY = 0.0f;
        foveated.fovealFov = 30.0f;
        foveated.periQuality = WVR_PeripheralQuality_Low;
        fbo->glViewportFull();
        WVR_PreRenderEye(WVR_Eye_Left, &leftEyeTexture, &foveated);
#else
        if (gScale < 1 && gScale > 0)
            fbo->glViewportScale(mLUV, mUUV);
        else
            fbo->glViewportFull();
        WVR_PreRenderEye(WVR_Eye_Left, &leftEyeTexture);
#endif
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderScene(WVR_Eye_Left);
        fbo->unbindFrameBuffer();

        // Right Eye
        fbo = gMsaa ? mRightEyeFBOMSAA.at(mIndexRight) : mRightEyeFBO.at(mIndexRight);
        fbo->bindFrameBuffer();
        WVR_TextureParams_t rightEyeTexture = WVR_GetTexture(mRightEyeQ, mIndexRight);
#if ENABLE_LOW_FOVEATED_RENDERING
        foveated.focalX = foveated.focalY = 0.0f;
        foveated.fovealFov = 30.0f;
        foveated.periQuality = WVR_PeripheralQuality_Low;
        fbo->glViewportFull();
        WVR_PreRenderEye(WVR_Eye_Right, &rightEyeTexture, &foveated);
#else
        if (gScale < 1 && gScale > 0)
            fbo->glViewportScale(mLUV, mUUV);
        else
            fbo->glViewportFull();
        WVR_PreRenderEye(WVR_Eye_Right, &rightEyeTexture);
#endif
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderScene(WVR_Eye_Right);
        fbo->unbindFrameBuffer();
}


void MainApplication::renderScene(WVR_Eye nEye) {
    WVR_RenderMask(nEye);

    // Reset for second eye
    if (mMeteoroid and mMeteoroid->m_perimetry_status == "Done") {
        if (mActiveEye == mFirstEye) {
            savePerimetryData(mMeteoroid->m_goldmann_sheet);
            mMeteoroid->reset_animation();
            if (mFirstEye == 1) {
                mActiveEye = 2;
                mMeteoroid->mActiveEye = 2;
                mShowLeftEyeMenu = true;
            } else if (mFirstEye == 2) {
                mActiveEye = 1;
                mMeteoroid->mActiveEye = 1;
                mShowRightEyeMenu = true;
            }
        } else {
            if (!allDataSaved) {
                savePerimetryData(mMeteoroid->m_goldmann_sheet);
                allDataSaved = true;
                mShowEndMenu = true;
            }
        }
    }
    /*
    if (mGridPicture && mGridPicture->isEnabled()) {
        if (nEye == WVR_Eye_Left)
            mGridPicture->draw(mProjectionLeft, mEyePosLeft, mHMDPose, mLightDir);
        else if (nEye == WVR_Eye_Right)
            mGridPicture->draw(mProjectionRight, mEyePosRight, mHMDPose, mLightDir);
        return;
    }
     */
    // Menus
    if (mStartMenu && mShowStartMenu) {
        if (nEye == WVR_Eye_Left)
            mStartMenu->draw(mProjectionLeft, mEyePosLeft, mHMDPose, mLightDir);
        else if (nEye == WVR_Eye_Right)
            mStartMenu->draw(mProjectionRight, mEyePosRight, mHMDPose, mLightDir);
    }
    if (mRightEyeMenu && mShowRightEyeMenu) {
        if (nEye == WVR_Eye_Left)
            mRightEyeMenu->draw(mProjectionLeft, mEyePosLeft, mHMDPose, mLightDir);
        else if (nEye == WVR_Eye_Right)
            mRightEyeMenu->draw(mProjectionRight, mEyePosRight, mHMDPose, mLightDir);
    }
    if (mLeftEyeMenu && mShowLeftEyeMenu) {
        if (nEye == WVR_Eye_Left)
            mLeftEyeMenu->draw(mProjectionLeft, mEyePosLeft, mHMDPose, mLightDir);
        else if (nEye == WVR_Eye_Right)
            mLeftEyeMenu->draw(mProjectionRight, mEyePosRight, mHMDPose, mLightDir);
    }
    if (mEndMenu && mShowEndMenu) {
        if (nEye == WVR_Eye_Left)
            mEndMenu->draw(mProjectionLeft, mEyePosLeft, mHMDPose, mLightDir);
        else if (nEye == WVR_Eye_Right)
            mEndMenu->draw(mProjectionRight, mEyePosRight, mHMDPose, mLightDir);
    }
    if (mPauseMenu && mShowPauseMenu) {
        if (nEye == WVR_Eye_Left)
            mPauseMenu->draw(mProjectionLeft, mEyePosLeft, mHMDPose, mLightDir);
        else if (nEye == WVR_Eye_Right)
            mPauseMenu->draw(mProjectionRight, mEyePosRight, mHMDPose, mLightDir);
    }

    if (realPausedReleased) {
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - mPausedReleased);
        if (duration.count() > 4) {
            realPausedReleased = false;
            mShowPauseMenu = false;
            mPausedReleased = now;
            mMeteoroid->resume_animation();
    }



    // Sky
    if (mSky) {
        if (nEye == WVR_Eye_Left)
            mSky->draw(mProjectionLeft, mEyePosLeft, mHMDPose, mLightDir);
        else if (nEye == WVR_Eye_Right)
            mSky->draw(mProjectionRight, mEyePosRight, mHMDPose, mLightDir);
    }

    // Meteoroid
    if (mMeteoroid and !mShowPauseMenu) {
        if (nEye == WVR_Eye_Left and mActiveEye == 2)
            mMeteoroid->draw(mProjectionLeft, mEyePosLeft, mHMDPose, mLightDir);
        else if (nEye == WVR_Eye_Right and mActiveEye == 1)
            mMeteoroid->draw(mProjectionRight, mEyePosRight, mHMDPose, mLightDir);
    }
    // Stars
    if (mStars and SHOW_STARS) {
        if (nEye == WVR_Eye_Left)
            mStars->draw(mProjectionLeft, mEyePosLeft, mHMDPose, mLightDir);
        else if (nEye == WVR_Eye_Right)
            mStars->draw(mProjectionRight, mEyePosRight, mHMDPose, mLightDir);
    }

    // Terrain
    if (mTerrain and SHOW_TERRAIN) {
        if (nEye == WVR_Eye_Left)
            mTerrain->draw(mProjectionLeft, mEyePosLeft, mHMDPose, mLightDir);
        else if (nEye == WVR_Eye_Right)
            mTerrain->draw(mProjectionRight, mEyePosRight, mHMDPose, mLightDir);
    }

    // Sphere
    if (mSphere and (!mShowStartMenu and !mShowRightEyeMenu and !mShowLeftEyeMenu and !mShowEndMenu and !mShowPauseMenu)) {
    // mSphere->setSphereColor(currColor);
        if (nEye == WVR_Eye_Left)
            mSphere->draw(mProjectionLeft, mEyePosLeft, mHMDPose, mLightDir);
        else if (nEye == WVR_Eye_Right)
            mSphere->draw(mProjectionRight, mEyePosRight, mHMDPose, mLightDir);
    }

    glUseProgram(0);

    GLenum glerr = glGetError();
    if (glerr != GL_NO_ERROR) {
        LOGW("glGetError(): %d", glerr);
    }
}

void MainApplication::updateTime() {
    // Process time variable.
    struct timeval now;
    gettimeofday(&now, NULL);

    mClockCount++;
    if (mRtcTime.tv_usec > now.tv_usec)
        mClockCount = 0;
    if (mClockCount >= VR_MAX_CLOCKS)
        mClockCount--;

    uint32_t timeDiff = timeval_subtract(now, mRtcTime);
    mTimeDiff = timeDiff / 1000000.0f;
    mTimeAccumulator2S += timeDiff;
    mRtcTime = now;
    mFrameCount++;
    if (mTimeAccumulator2S > 2000000) {
        mFPS = mFrameCount / (mTimeAccumulator2S / 1000000.0f);
        LOGI("HelloVR FPS %3.0f", mFPS);

        mFrameCount = 0;
        mTimeAccumulator2S = 0;
    }
}

void MainApplication::updateEyeToHeadMatrix(bool is6DoF) {
    if (is6DoF != mIs6DoFPose) {
        if(is6DoF) {
            mEyePosLeft = wvrmatrixConverter(
                WVR_GetTransformFromEyeToHead(WVR_Eye_Left, WVR_NumDoF_6DoF)).invert();
            mEyePosRight = wvrmatrixConverter(
                WVR_GetTransformFromEyeToHead(WVR_Eye_Right, WVR_NumDoF_6DoF)).invert();
        } else {
            mEyePosLeft = wvrmatrixConverter(
                WVR_GetTransformFromEyeToHead(WVR_Eye_Left, WVR_NumDoF_3DoF)).invert();
            mEyePosRight = wvrmatrixConverter(
                WVR_GetTransformFromEyeToHead(WVR_Eye_Right, WVR_NumDoF_3DoF)).invert();
        }

    }
    mIs6DoFPose = is6DoF;
}

void MainApplication::updateHMDMatrixPose() {
    LOGENTRY();

    WVR_GetSyncPose(WVR_PoseOriginModel_OriginOnHead, mVRDevicePairs, WVR_DEVICE_COUNT_LEVEL_1);
    mValidPoseCount = 0;
    mPoseClasses = "";
    for (int nDevice = 0; nDevice < WVR_DEVICE_COUNT_LEVEL_1; ++nDevice) {
        if (mVRDevicePairs[nDevice].pose.isValidPose) {
            mValidPoseCount++;
            mDevicePoseArray[nDevice] = wvrmatrixConverter(mVRDevicePairs[nDevice].pose.poseMatrix);

            if (mDevClassChar[nDevice]==0) {
                switch (WVR_DeviceType_HMD + nDevice) {
                case WVR_DeviceType_HMD:                       mDevClassChar[nDevice] = 'H'; break;
                case WVR_DeviceType_Controller_Right:          mDevClassChar[nDevice] = 'R'; break;
                case WVR_DeviceType_Controller_Left:           mDevClassChar[nDevice] = 'L'; break;
                default:                                       mDevClassChar[nDevice] = '?'; break;
                }
            }
            mPoseClasses += mDevClassChar[nDevice];
        }
    }

    if (mVRDevicePairs[WVR_DEVICE_HMD].pose.isValidPose) {
        updateEyeToHeadMatrix(mVRDevicePairs[WVR_DEVICE_HMD].pose.is6DoFPose);
        Matrix4 hmd = mDevicePoseArray[WVR_DEVICE_HMD];

        if (!mMove) {
            // The controller make the sample not simple.  If you don't have
            // a controller, we just need invert the hmd pose.

            // When the head turn left, acturally the object turn right.
            // When the head move left, acturally the object move right.
            // So we need invert the hmd matrix.
            mHMDPose = hmd.invert();
        } else {
            // In order to add translation and rotation to HMD. We need seperate
            // the translation and rotation into two matrix from HMD.

            Matrix4 hmdRotation = hmd;
            hmdRotation.setColumn(3, Vector4(0,0,0,1));

            Matrix4 hmdTranslation;
            hmdTranslation.setColumn(3, Vector4(hmd[12], hmd[13], hmd[14], 1));

            // Update world rotation.
            mWorldRotation += -mDriveAngle * mTimeDiff;
            Matrix4 mat4WorldRotation;
            mat4WorldRotation.rotate(mWorldRotation, 0, 1, 0);

            // Update WorldTranslation
            Vector4 direction = (mat4WorldRotation * hmdRotation) * Vector4(0, 0, 1, 0);  // Not apply the tranlsation of hmdpose
            direction *= -mDriveSpeed * mTimeDiff;
            direction.w = 1;

            // Move toward -z
            Matrix4 update;
            update.setColumn(3, direction);
            mWorldTranslation *= update;

            // Check world bound
            if (mWorldTranslation[12] >= mFarClip/2)
                mWorldTranslation[12] = mFarClip/2;
            if (mWorldTranslation[12] <= -mFarClip/2)
                mWorldTranslation[12] = -mFarClip/2;
            if (mWorldTranslation[13] >= mFarClip/2)
                mWorldTranslation[13] = mFarClip/2;
            if (mWorldTranslation[13] <= -mFarClip/2)
                mWorldTranslation[13] = -mFarClip/2;
            if (mWorldTranslation[14] >= mFarClip/2)
                mWorldTranslation[14] = mFarClip/2;
            if (mWorldTranslation[14] <= -mFarClip/2)
                mWorldTranslation[14] = -mFarClip/2;

            // DEFINE: The invert A^-1 is notated A'
            // The invert property: (AB)' = B'A'
            // "WT" means "world translation", "HR" means "hmd rotation", and so on.
            // We can get the model tranform matrix as (WT*HT*WR*HR)' = HR'*WR'*WT'*HT'
            // The tranlation matrix property: TA = AT , then: (TA)' = (AT)'
            // So we can put tranlsation matrix any where.
            // We apply WR' to vertex first, then do HR'.  If not, the world will be weired when look up or down.
            mHMDPose = (mWorldTranslation * hmdTranslation * mat4WorldRotation * hmdRotation).invert();
        }
    }
    if (gDebug) dumpMatrix("hmd", mHMDPose);
}

// For Eye Tracking
// Todo: Change for Singe Eye when Selection is made or Chosen randomly

bool MainApplication::initEyeTracking() {
    LOGENTRY();
    LOGI("MainApplication::initEyeTracking()");

    // Start eye tracking
    WVR_Result result = WVR_StartEyeTracking();
    if (result != WVR_Success) {
        LOGE("Failed to start eye tracking: %d", result);
        mSphere->setSphereColor(Sphere::Color::red);
        mEyeTrackingEnabled = false;
        return false;
    }

    // Check if eye tracking device is connected
    if (!WVR_IsDeviceConnected(WVR_DeviceType_EyeTracking)) {
        LOGW("Eye tracking device not connected");
        mSphere->setSphereColor(Sphere::Color::blue);
        mEyeTrackingEnabled = false;
        return false;
    }

    mEyeTrackingEnabled = true;
    mSphere->setSphereColor(Sphere::Color::okay);
    LOGI("Eye tracking initialized successfully");
    return true;
}

void MainApplication::shutdownEyeTracking() {
    if (mEyeTrackingEnabled) {
        WVR_StopEyeTracking();
        mEyeTrackingEnabled = false;
        LOGI("Eye tracking stopped");
    }
}

void MainApplication::updateEyeTracking() {
    if (!mEyeTrackingEnabled || !mSphere) return;

    // Get eye tracking data
    WVR_Result result = WVR_GetEyeTracking(&mEyeTrackingData, WVR_CoordinateSystem_Local);

    if (result != WVR_Success) {
        return;
    }
    bool isSet = false;
    Vector3 gazeDirLocal;
    if ((mActiveEye == 1) && mEyeTrackingData.right.eyeTrackingValidBitMask /*&& WVR_GazeDirectionNormalizedValid*/) {
        gazeDirLocal = Vector3(
                mEyeTrackingData.right.gazeDirectionNormalized.v[0],
                mEyeTrackingData.right.gazeDirectionNormalized.v[1],
                mEyeTrackingData.right.gazeDirectionNormalized.v[2]
        );
        isSet = true;
    }
    if ((mActiveEye == 2) && mEyeTrackingData.left.eyeTrackingValidBitMask /*&& WVR_GazeDirectionNormalizedValid*/) {
        gazeDirLocal = Vector3(
                mEyeTrackingData.left.gazeDirectionNormalized.v[0],
                mEyeTrackingData.left.gazeDirectionNormalized.v[1],
                mEyeTrackingData.left.gazeDirectionNormalized.v[2]
        );
        isSet = true;
    }

    // Check if combined gaze data is valid
    if (isSet) {
        // --- A. CALCULATE VECTORS ---
        gazeDirLocal = gazeDirLocal.normalize(); // Ensure it's normalized

        // Get Sphere Position (World Space)
        Vector3 spherePosWorld = mSphere->getCenter();

        // Get Head Position (World Space)
        // Extract translation from HMD Pose matrix (Column 3)
        // Note: mDevicePoseArray[WVR_DEVICE_HMD] is the Head-to-World matrix
        Matrix4 headMatrix = mDevicePoseArray[WVR_DEVICE_HMD];

        // IMPORTANT: You apply mWorldTranslation in your scene, so we must account for it.
        // Based on your draw functions, the final HMD World pos is roughly:
        // WorldTranslation * WorldRotation * HMD_Pose
        Matrix4 mat4WorldRotation;
        mat4WorldRotation.rotate(mWorldRotation, 0, 1, 0);
        Matrix4 finalHeadMat = mWorldTranslation * mat4WorldRotation * headMatrix;

        Vector3 headPosWorld(finalHeadMat[12], finalHeadMat[13], finalHeadMat[14]);

        // Calculate Vector from Head to Sphere
        Vector3 targetDirWorld = spherePosWorld - headPosWorld;
        targetDirWorld = targetDirWorld.normalize();

        // Convert Gaze to World Space (Rotate only, do not translate direction vectors)
        // Extract 3x3 rotation from finalHeadMat
        Matrix3 rotMat(
                finalHeadMat[0], finalHeadMat[1], finalHeadMat[2],
                finalHeadMat[4], finalHeadMat[5], finalHeadMat[6],
                finalHeadMat[8], finalHeadMat[9], finalHeadMat[10]
        );
        Vector3 gazeDirWorld = (rotMat * gazeDirLocal).normalize();

        // --- B. CHECK ANGLE (The "Acceptance Radius") ---

        // Dot Product = cos(angle)
        float dotProduct = gazeDirWorld.dot(targetDirWorld);

        // Clamp to -1.0 to 1.0 to avoid math errors
        if(dotProduct > 1.0f) dotProduct = 1.0f;
        if(dotProduct < -1.0f) dotProduct = -1.0f;

        // Calculate Angle in Degrees
        float angle = acos(dotProduct) * 180.0f / M_PI;

        // DEFINE TOLERANCE: How strict are we?
        // 8-10 degrees is usually a comfortable "foveal" view.
        // 15+ degrees is very loose.

        if (angle <= MAX_ACCEPTANCE_ANGLE_DEG) {
            // --- STATE: LOOKING AT SPHERE ---
            mSphere->setSphereColor(Sphere::Color::green); // Renders as Grey/White

            if (mMeteoroid) {
                mMeteoroid->resume_animation();
            }
        } else {
            // --- STATE: LOOKING AWAY ---
            mSphere->setSphereColor(Sphere::Color::red);   // Renders as Red

            if (mMeteoroid) {
                mMeteoroid->pause_animation(false);
            }
        }
    }
}


void MainApplication::savePerimetryData(const GoldmannSheet& sheet) {
    if (mExportPath.empty()) {
        LOGE("Cannot save data: Export path is empty.");
        return;
    }

    // --- 1. Get Current Time ---
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);

    // --- 2. Format the Filename ---
    // Format: perimetry_YYYY-MM-DD_HH-MM-SS.csv
    char buffer[128];
    std::strftime(buffer, sizeof(buffer), "perimetry_%Y-%m-%d_%H-%M-%S.csv", now);
    std::string filename(buffer);

    // --- 3. Construct Full Path ---
    // Ensure mExportPath doesn't already have a trailing slash
    std::string fullPath;
    std::string eyeAppendix;
    map<int, map<MeteoroidSizeID, map<string, SheetEntry>>> sheet_to_save;
    if (mActiveEye == 1) {
        eyeAppendix = "Right_";
        sheet_to_save = sheet.m_sheet_right;
    } else if (mActiveEye == 2) {
        eyeAppendix = "Left_";
        sheet_to_save = sheet.m_sheet_left;
    }
    if (mExportPath.back() == '/') {
        fullPath = mExportPath + eyeAppendix + filename;
    } else {
        fullPath = mExportPath + "/"+ eyeAppendix + filename;
    }

    LOGI("Saving data to: %s", fullPath.c_str());

    // --- 4. Open and Write ---
    std::ofstream outFile;
    outFile.open(fullPath, std::ios::out); // Default to overwrite since filename is unique

    if (!outFile.is_open()) {
        LOGE("Failed to open file for writing: %s", fullPath.c_str());
        return;
    }

    // Write CSV Header
    outFile << "Longitude,SizeIndex,Points[(PHI,THETA)],NormedValue\n";

    //const auto& points = sheet.get_points();
    //const auto& sizes = sheet.get_sizes(); // Assuming you have sizes stored similarly

    auto inn_it = sheet.inner_it;
    auto mid_it = sheet.middle_it;
    auto out_it = sheet.outer_it;


    for (out_it = sheet_to_save.begin(); out_it != sheet_to_save.end(); out_it++) {
        for (mid_it = out_it->second.begin(); mid_it != out_it->second.end(); mid_it++) {

            for (inn_it = mid_it->second.begin(); inn_it != mid_it->second.end(); inn_it++) {
                auto longitude = out_it->first;
                MeteoroidSizeID size_id = mid_it->first;
                string luminance = inn_it->first;
                auto entry = inn_it->second;
                AnyMeteoroidSize size = m_size_map.at(size_id);
                string size_name = std::visit([](auto &&s) {
                    return s.get_name();
                }, size);

                // Write Data

                outFile << longitude << ","
                        << size_name << ","
                        << luminance << ",[";

                for (auto &val: entry.points) {
                    outFile << "(" << val.phi
                            << "|" << val.theta
                            << ");";
                }
                outFile << "]," << entry.normalized_angle << "\n";
            }
        }
    }

    outFile.close();
    LOGI("Data saved successfully with timestamp.");
}

void MainApplication::CloseApplication() {
    LOGI("CloseApplication called. Exiting main loop...");
    mShouldQuit = true;
}

#if defined(USE_CONTROLLER) || defined(USE_CUSTOM_CONTROLLER)
void MainApplication::setupControllers()
{
    for (uint32_t cID = 0; cID < 2; ++cID) {
        if (mControllerObjs[cID] != nullptr) {
#if defined(USE_CONTROLLER)
            mControllerObjs[cID]->loadControllerModelAsync();
#else
            mControllerObjs[cID]->loadControllerEmitterAsync();
#endif
        } else {
            LOGW("[APCtrlerObj] CtrlerObj(%d) is nullptr", cID);
        }
    }
}
#else
//-----------------------------------------------------------------------------
// Purpose: Finds a controller cube we've already loaded or loads a new one
//-----------------------------------------------------------------------------
ControllerCube *MainApplication::findOrLoadControllerCube(WVR_DeviceType deviceType) {
    LOGDIF("LoadControllerCube %d", deviceType);

    ControllerCube *pControllerCube = NULL;
    for (std::vector<ControllerCube *>::iterator i = mControllerCubes.begin(); i != mControllerCubes.end(); i++) {
        if ((*i)->getDeviceType() == deviceType) {
            pControllerCube = *i;
            break;
        }
    }

    if (!pControllerCube) {
        pControllerCube = new ControllerCube(deviceType);
        if (m3DOF)
            pControllerCube->set3DOF(true);
        mControllerCubes.push_back(pControllerCube);
    }
    return pControllerCube;
}

//-----------------------------------------------------------------------------
// Purpose: Create/destroy GL a controller cube for a single device
//-----------------------------------------------------------------------------
void MainApplication::setupControllerCubeForDevice(WVR_DeviceType deviceType) {
    if (deviceType != WVR_DeviceType_Controller_Right && deviceType != WVR_DeviceType_Controller_Left) {
        LOGW("setupControllerCubeForDevice() Ignore unknown device type: %d", deviceType);
        return;
    }

    // try to find a model we've already set up
    ControllerCube *pControllerCube = findOrLoadControllerCube(deviceType);
    if (pControllerCube != NULL) {
        uint32_t index = deviceType - WVR_DeviceType_HMD;
        mControllerCubeTableById[index] = pControllerCube;
        mShowDeviceArray[index] = true;
    }
}

//-----------------------------------------------------------------------------
// Purpose: Create/destroy GL controller cubes
//-----------------------------------------------------------------------------
void MainApplication::setupControllerCubes() {
    for (uint32_t index = WVR_DEVICE_HMD + 1; index <= WVR_DEVICE_COUNT_LEVEL_1; index++) {
        WVR_DeviceType deviceType = (WVR_DeviceType)(index + WVR_DeviceType_HMD);
        if (!WVR_IsDeviceConnected(deviceType))
            continue;

        setupControllerCubeForDevice(deviceType);
    }
}
#endif
