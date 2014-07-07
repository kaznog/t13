//
//  TitleSceneLoader.h
//  ccs2dx_v3tutorial001
//
//  Created by noguchika on 2014/02/20.
//
//

#ifndef ccs2dx_v3tutorial001_TitleSceneLoader_h
#define ccs2dx_v3tutorial001_TitleSceneLoader_h

#include "cocos2d.h"
#include "cocosbuilder/CocosBuilder.h"
#include "TitleScene.h"

class TitleSceneLoader : public cocosbuilder::LayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(TitleSceneLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(TitleScene);
};

#endif
