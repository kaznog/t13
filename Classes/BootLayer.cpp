//
//  BootLayer.cpp
//  ccs2dx_v3tutorial001
//
//  Created by noguchika on 2014/02/20.
//
//

#include "BootLayer.h"
#include "TitleScene.h"
#include "TitleSceneLoader.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace cocosbuilder;

Scene* BootLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = BootLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool BootLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    auto pUserDefault = UserDefault::getInstance();
    auto music_flag = pUserDefault->getBoolForKey("music", true);
    if (music_flag) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(S_GAME_MUSIC_MP3);
    }
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile(S_COIN_PLIST);
    return true;
}

void BootLayer::onEnter()
{
    this->CCBChangeScene(S_TITLE_CLASSNAME, TitleSceneLoader::loader(), S_CCB_MAINMENU);
}
