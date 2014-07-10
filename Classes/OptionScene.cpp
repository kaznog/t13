//
//  OptionScene.cpp
//
//  Created by kaznog on 2014/02/20.
//
//

#include "OptionScene.h"
#include "TitleScene.h"
#include "TitleSceneLoader.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;
using namespace cocosbuilder;

Scene* OptionScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = OptionScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool OptionScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    auto gradiantLayer = LayerGradient::create(Color4B(0, 0, 0, 255), Color4B(255, 255, 255, 255));
    this->addChild(gradiantLayer);
    
    auto label1 = cocos2d::LabelBMFont::create("MUSIC ON", S_KONQA32_FNT);
    auto item1 = MenuItemLabel::create(label1);

    auto label2 = LabelBMFont::create("MUSIC OFF", S_KONQA32_FNT);
    auto item2 = MenuItemLabel::create(label2);
    auto toggle = MenuItemToggle::createWithCallback(CC_CALLBACK_1(OptionScene::onMusicToggle, this), item1, item2, NULL);
    if (CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() == false) {
        toggle->setSelectedIndex(1);
    }
    auto back = MenuItemFont::create("Back", CC_CALLBACK_1(OptionScene::onBack, this));
    auto pMenu = Menu::create(toggle, back, NULL);
    this->addChild(pMenu);
    pMenu->alignItemsVertically();
    auto director = Director::getInstance();
    Size winSize = director->getWinSize();
    Point centerPos = Point(winSize.width/2, winSize.height/2);
    pMenu->setPosition(centerPos);

    return true;
}

void OptionScene::onMusicToggle(cocos2d::Object * sender)
{
    auto pUserDefault = UserDefault::getInstance();
    auto audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
    if (audioEngine->isBackgroundMusicPlaying()) {
        audioEngine->stopBackgroundMusic();
        pUserDefault->setBoolForKey("music", false);
    } else {
        audioEngine->playBackgroundMusic(S_GAME_MUSIC_MP3);
        pUserDefault->setBoolForKey("music", true);
    }
    pUserDefault->flush();
}

void OptionScene::onBack(cocos2d::Object * sender)
{
    this->CCBChangeScene(S_TITLE_CLASSNAME, TitleSceneLoader::loader(), S_CCB_MAINMENU);
}

