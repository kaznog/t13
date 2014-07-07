//
//  AboutSceneLoader.h
//  ccs2dx_v3tutorial001
//
//  Created by noguchika on 2014/02/21.
//
//

#ifndef ccs2dx_v3tutorial001_AboutSceneLoader_h
#define ccs2dx_v3tutorial001_AboutSceneLoader_h

#include "cocos2d.h"
#include "cocosbuilder/CocosBuilder.h"
#include "AboutScene.h"

class AboutSceneLoader : public cocosbuilder::LayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(AboutSceneLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(AboutScene);
};


#endif
