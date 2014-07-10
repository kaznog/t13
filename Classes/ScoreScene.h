//
//  ScoreScene.h
//
//  Created by kaznog on 2014/02/24.
//
//

#ifndef __ccs2dx_v3tutorial001__ScoreScene__
#define __ccs2dx_v3tutorial001__ScoreScene__

#include "cocos2d.h"
#include "CCBLoaderLayer.h"
#include "Constant.h"

USING_NS_CC;

class ScoreScene : public CCBLoaderLayer
{
public:
    virtual bool init();
    
    // implement the "static create()" method manually
    static cocos2d::Scene* createScene();
    
    CREATE_FUNC(ScoreScene);
    virtual void dispose(){}
    virtual void onEnterTransitionDidFinish();
    void onBack(cocos2d::Object * sender);
};

#endif /* defined(__ccs2dx_v3tutorial001__ScoreScene__) */
