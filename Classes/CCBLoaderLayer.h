//
//  CCBLoaderLayer.h
//  ccs2dx_v3tutorial001
//
//  Created by noguchika on 2014/02/25.
//
//

#ifndef __ccs2dx_v3tutorial001__CCBLoaderLayer__
#define __ccs2dx_v3tutorial001__CCBLoaderLayer__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocosbuilder/CocosBuilder.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocosbuilder;

class CCBLoaderLayer : public Layer
{
public:
    void CCBChangeScene(const char * pClassName, NodeLoader * pNodeLoader, const char *pCCBFileName);
    virtual void dispose() = 0;
};

#endif /* defined(__ccs2dx_v3tutorial001__CCBLoaderLayer__) */
