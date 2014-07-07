//
//  CCBLoaderLayer.cpp
//  ccs2dx_v3tutorial001
//
//  Created by noguchika on 2014/02/25.
//
//

#include "CCBLoaderLayer.h"

void CCBLoaderLayer::CCBChangeScene(const char *pClassName, cocosbuilder::NodeLoader *pNodeLoader, const char *pCCBFileName)
{
    /* Create an autorelease NodeLoaderLibrary. */
    auto nodeLoaderLibrary = NodeLoaderLibrary::newDefaultNodeLoaderLibrary();
    nodeLoaderLibrary->unregisterNodeLoader(pClassName);
    nodeLoaderLibrary->registerNodeLoader(pClassName, pNodeLoader);
    
    /* Create an autorelease CCBReader. */
    cocosbuilder::CCBReader * ccbReader = new cocosbuilder::CCBReader(nodeLoaderLibrary);
    /* Read a ccbi file. */
    auto node = ccbReader->readNodeGraphFromFile(pCCBFileName);
    ccbReader->release();
    // シーンを用意し、ゲーム画面を設置する
    if (node == NULL)
    {
        nodeLoaderLibrary->unregisterNodeLoader(pClassName);
        this->dispose();
        auto audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
        audioEngine->stopAllEffects();
        audioEngine->stopBackgroundMusic();
        Director::getInstance()->end();
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
#endif
    }
    auto pScene = Scene::create();
    pScene->addChild(node);
    Director::getInstance()->replaceScene(pScene);
}