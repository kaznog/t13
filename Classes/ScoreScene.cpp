//
//  ScoreScene.cpp
//  ccs2dx_v3tutorial001
//
//  Created by noguchika on 2014/02/24.
//
//

#include "ScoreScene.h"
#include "ScoreManager.h"

#include "TitleScene.h"
#include "TitleSceneLoader.h"

USING_NS_CC;
using namespace cocosbuilder;

Scene* ScoreScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ScoreScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool ScoreScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    auto gradiantLayer = LayerGradient::create(Color4B(0, 0, 0, 255), Color4B(255, 255, 255, 255));
    this->addChild(gradiantLayer);
    
    auto label = LabelBMFont::create("HI SCORES", S_GAS40_FNT);
    Size winSize = Director::getInstance()->getWinSize();
    label->setPosition(Point(winSize.width/2, winSize.height - label->getContentSize().height/2));
    this->addChild(label);

    auto back = MenuItemFont::create("Back", CC_CALLBACK_1(ScoreScene::onBack, this));
    auto menu = Menu::create(back, NULL);
    menu->alignItemsVertically();
    menu->setPosition(winSize.width-50, winSize.height-50);
    this->addChild(menu);
    
    return true;
}

void ScoreScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    std::vector<int>* scores = ScoreManager::getInstance()->getScores();
    std::vector<LabelBMFont*> labels;
    LabelBMFont* label = nullptr;
    Size winSize = Director::getInstance()->getWinSize();
    int i = 0;
    for (std::vector<int>::iterator it = scores->begin(); it != scores->end(); it++, i++) {
        label = LabelBMFont::create(std::to_string((int)*it), S_GAS40_FNT);
        float h = label->getContentSize().height;
        label->setPosition(Point(winSize.width/2, winSize.height/2 +h*2 - i*h));
        this->addChild(label);
        labels.push_back(label);
    }
    i = 0;
    for (std::vector<LabelBMFont*>::iterator it = labels.begin(); it != labels.end(); it++, i++) {
        LabelBMFont* child = (LabelBMFont*)*it;
        Point dstPoint = child->getPosition();
        float offset = (winSize.width / 2 + 50);
        if (i % 2) {
            offset = -offset;
        }
        child->setPosition(Point(dstPoint.x + offset, dstPoint.y));
        child->runAction(
                         Sequence::create(DelayTime::create(i*0.1),
                                          EaseElasticInOut::create(MoveBy::create(2, Point(-offset, 0)), 0.35),
                                          NULL)
        );
    }
}

void ScoreScene::onBack(cocos2d::Object * sender)
{
    this->CCBChangeScene(S_TITLE_CLASSNAME, TitleSceneLoader::loader(), S_CCB_MAINMENU);
}
