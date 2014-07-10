#include "HelloWorldScene.h"

#include "picojson.h"
#include <math.h>
#include "TitleScene.h"
#include "TitleSceneLoader.h"
#include "ScoreManager.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace cocosbuilder;

Scene* HelloWorld::createScene(GameState* gameState)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create(gameState);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

HelloWorld* HelloWorld::create(GameState *gameState)
{
    HelloWorld* pRet = new HelloWorld();
    if (pRet && pRet->init(gameState)) {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

HelloWorld::HelloWorld()
: m_level(0)
, m_maxLevel(0)
, _gameState(nullptr)
, _deferredState(STATE_UNKNOWN)
, _batch(nullptr)
, _space(nullptr)
, _debugNode(nullptr)
, _carSprite(nullptr)
, _motor(nullptr)
, _scrollNode(nullptr)
, _carSmoke(nullptr)
, _scoreLabel(nullptr)
, _timeLabel(nullptr)
, _shapesToRemove(nullptr)
{
}

HelloWorld::~HelloWorld()
{
    this->dispose();
}

void HelloWorld::dispose()
{
    if (this->_shapesToRemove != nullptr) {
        if (this->_shapesToRemove->size() != 0) {
            this->_shapesToRemove->clear();
        }
        delete this->_shapesToRemove;
        this->_shapesToRemove = nullptr;
    }
}

// on "init" you need to initialize your instance
bool HelloWorld::init(GameState* gameState)
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    Size screenSize = Director::getInstance()->getWinSize();
    this->_scaleFactor = 800/screenSize.width;
    this->setPosition(Vec2(0, 0));
    this->_gameState = gameState;
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("coins.plist");
    
    auto coin = Sprite::createWithSpriteFrameName("coin01.png");
    this->_batch = SpriteBatchNode::createWithTexture(coin->getTexture(), 100);
    
    auto animCache = AnimationCache::getInstance();
    animCache->addAnimationsWithFile("coins_animation.plist");
    
    auto scroll = ParallaxNode::create();
    this->addChild(scroll, Z_SCROLL);
    this->_scrollNode = scroll;
    
    scroll->addChild(this->_batch, Z_SPRITES, Vec2(1, 1), Vec2(0, 0));
    
    auto background = Sprite::create("Parallax.png", Rect(0, 0, 4096, 512));
    scroll->addChild(background, Z_MOUNTAINS, Vec2(0.2, 0.2), Vec2(0, -150));
    background->setAnchorPoint(Vec2::ZERO);
    Texture2D::TexParams tex_params = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_CLAMP_TO_EDGE};
    background->getTexture()->setTexParameters(tex_params);
    
    this->_terrain = DrawNode::create();
    scroll->addChild(this->_terrain, Z_TERRAIN, Vec2(1, 1), Vec2(0, 0));
    this->_terrain->setVisible(true);
    
    this->_carSmoke = ParticleSystemQuad::create("car_smoke.plist");
    this->_carSmoke->setPosition(0, 0);
    this->addChild(this->_carSmoke, Z_SMOKE);
    this->_carSmoke->setPositionType(ParticleSystem::PositionType::FREE);
    
    this->_shapesToRemove = new std::vector<cpShape*>();
    
    this->initHUD();
    this->scheduleUpdate();
    return true;
}

void HelloWorld::initHUD()
{
    /* Create an autorelease CCBReader. */
    NodeLoaderLibrary * nodeLoaderLibrary = NodeLoaderLibrary::newDefaultNodeLoaderLibrary();
    cocosbuilder::CCBReader * ccbReader = new cocosbuilder::CCBReader(nodeLoaderLibrary);
    /* Read a ccbi file. */
    auto node = ccbReader->readNodeGraphFromFile(S_CCB_HUD, this);
    ccbReader->release();
    this->_scoreLabel = (Label*)node->getChildByTag(SCORE_LABEL_TAG);
    this->_timeLabel  = (Label*)node->getChildByTag(TIME_LABEL_TAG);
    
    // bug in cocosbuilder
//    this->_scoreLabel->setAnchorPoint(Vec2(1.0f, 0.5f));
//    this->_timeLabel->setAnchorPoint(Vec2(0.0f, 0.5f));
    node->setTag(Z_HUD);
    this->addChild(node, Z_HUD);
    
    // update with score
    this->_scoreLabel->setString( std::to_string(this->_gameState->score) );
    this->_scoreLabel->updateContent();
}

void HelloWorld::addScore(int value)
{
    this->_gameState->score += value;
    this->_scoreLabel->setString(std::to_string(this->_gameState->score));
    this->_scoreLabel->updateContent();
    this->_scoreLabel->stopAllActions();
    
    auto scaleUpTo = ScaleTo::create(0.05f, 1.2f);
    auto scaleDownTo = ScaleTo::create(0.05f, 1.0f);
    
    auto seq = Sequence::create(scaleUpTo, scaleDownTo, NULL);
    this->_scoreLabel->runAction(seq);
}

void HelloWorld::onEnter()
{
    Layer::onEnter();
}

void HelloWorld::onExit()
{
    this->enableCollisionEvent(false);
    this->enableTouchEvent(false);
    Layer::onExit();
}

void HelloWorld::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    this->enableTouchEvent(true);
    this->initPhysics();
    this->setupLevel(this->_gameState->level);
    
    this->_gameState->state = STATE_PLAYING;
    // Level Label
    auto label = LabelBMFont::create(StringUtils::format("LEVEL %d", this->_gameState->level + 1), S_GAS40_FNT);
    Size winSize = Director::getInstance()->getWinSize();
    label->setPosition(Vec2(winSize.width/2, winSize.height/2));
    this->addChild(label, Z_LABEL);
    
    auto d = DelayTime::create(1);
    auto scale = ScaleBy::create(1.1f, 5.0f);
    auto fade = FadeOut::create(1.1f);
    auto s = Spawn::create(scale, fade, NULL);
    auto seq = Sequence::create(d, s, CallFuncN::create(CC_CALLBACK_1(HelloWorld::onRemoveMe, this)), NULL);
    label->runAction(seq);
}

void HelloWorld::update(float delta)
{
    if( this->_gameState->state == STATE_PLAYING ) {
        // update time
        this->_gameState->time += delta;
        char timerstr[64];
        sprintf(timerstr, "%.1f", this->_gameState->time);
        // 何故かうまく動かない
        this->_timeLabel->setString(StringUtils::format("%.1f", this->_gameState->time));
        this->_timeLabel->updateContent();
    }
    
    // update state if necessary
    if( this->_deferredState != STATE_UNKNOWN)
        this->setGameState(this->_deferredState);
    // Don't update physics on game over
    if( this->_gameState->state != STATE_PAUSE && this->_space != nullptr){
        if(this->_gameState->state == STATE_LEVEL_COMPLETE){
            cpSpaceStep(this->_space, (float)1/1200);
        }
        else if(this->_gameState->state == STATE_GAME_OVER ){
            cpSpaceStep(this->_space, (float)1/300);
        }
        else{
            cpFloat dt = (float)1/60;
            cpSpaceStep(this->_space, dt);
        }
    }
    
    if (this->_carSprite != nullptr && this->_carSmoke != nullptr) {
        Vec2 p = this->_carSprite->convertToWorldSpace(Vec2::ZERO);
        this->_carSmoke->setPosition(p);
    }
    
    for(std::vector<cpShape*>::iterator it = this->_shapesToRemove->begin(); it != this->_shapesToRemove->end(); it++) {
        auto shape = *it;
        cpSpaceRemoveShape(this->_space, shape);
        auto body = cpShapeGetBody(shape);
        auto sprite = (PhysicsSprite*)cpBodyGetUserData(body);
        sprite->removeFromParent();
    }
    this->_shapesToRemove->clear();
    auto audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
    auto pUserDefault = UserDefault::getInstance();
    bool music = pUserDefault->getBoolForKey("music", true);
    if (music) {
        if (audioEngine->isBackgroundMusicPlaying() == false) {
            audioEngine->playBackgroundMusic(S_GAME_MUSIC_MP3);
        }
    } else {
        if (audioEngine->isBackgroundMusicPlaying()) {
            audioEngine->stopBackgroundMusic();
        }
    }
    
}

bool HelloWorld::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    if (this->_gameState->state == STATE_PLAYING)
    {
        this->setThrottle(1);
    }
    return true;
}

void HelloWorld::onTouchMoved(Touch *touch, Event *event)
{
    
}

void HelloWorld::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    if (this->_gameState->state == STATE_PLAYING)
        this->setThrottle(0);
}

void HelloWorld::onTouchCancelled(Touch *touch, Event *event)
{
    
}

void HelloWorld::onNextLevel(cocos2d::Ref * sender)
{
    this->_gameState->state = STATE_PAUSE;
    this->_gameState->level++;
    this->_gameState->time = 0;
    auto scene = HelloWorld::createScene(this->_gameState);
    auto director = Director::getInstance();
    director->replaceScene(TransitionFade::create(0.5, scene));
}

void HelloWorld::onMainMenu(cocos2d::Ref * sender)
{
    this->_gameState->state = STATE_PAUSE;
    this->CCBChangeScene(S_TITLE_CLASSNAME, TitleSceneLoader::loader(), S_CCB_MAINMENU);
}

void HelloWorld::onReStart(cocos2d::Ref * sender)
{
    this->_gameState->state = STATE_PAUSE;
    auto scene = HelloWorld::createScene(new GameState(this->_gameState->level));
    auto director = Director::getInstance();
    director->replaceScene(TransitionFade::create(0.5, scene));
}

void HelloWorld::onRemoveMe(cocos2d::Ref * sender)
{
    ((Node*)sender)->removeFromParent();
}


void HelloWorld::setGameStateDeferred(int state)
{
    this->_deferredState = state;
}

void HelloWorld::setGameState(int state)
{
    if (state != this->_gameState->state) {
        if (state == STATE_GAME_OVER) {
            this->displayGameOver();
        } else if (state == STATE_LEVEL_COMPLETE) {
            this->displayLevelComplete();
        }
        this->_gameState->state = state;
    }
    this->_deferredState = STATE_UNKNOWN;
}

void HelloWorld::displayLevelComplete()
{
    std::string legend = "";
    Menu* menu = nullptr;
    MenuItemFont* item = nullptr;
    Size winSize = Director::getInstance()->getWinSize();
    if (this->_gameState->level + 1 < this->m_maxLevel) {
        item = MenuItemFont::create("Next Level", CC_CALLBACK_1(HelloWorld::onNextLevel, this));
        legend = "LEVEL COMPLETE";
    } else {
        item = MenuItemFont::create("Main Menu", CC_CALLBACK_1(HelloWorld::onMainMenu, this));
        legend = "GAME COMPLETE";
        ScoreManager::getInstance()->addScore(this->_gameState->score);
    }
    
    menu = Menu::create(item, NULL);
    menu->alignItemsVertically();
    menu->setPosition(Vec2(winSize.width/2, winSize.height/3));
    this->addChild(menu, Z_DEBUG_MENU);
    auto label = LabelBMFont::create(legend, S_ABADI40_FNT);
    label->setPosition(Vec2(winSize.width/2, winSize.height/2));
    label->setScale(0.2f);
    auto sa = ScaleTo::create(0.5f, 1.05f);
    auto sb1 = ScaleTo::create(0.5f, 1.0f);
    auto sb2 = ScaleTo::create(0.5f, 1.05f);
    auto seq = Sequence::create(sb1, sb2, NULL);
    auto rep = Repeat::create(seq, 1000);
    auto all = Sequence::create(sa, rep, NULL);
    label->runAction(all);
    this->addChild(label, Z_LABEL);
    this->enableCollisionEvent(false);
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(S_LEVELCOMPLETE_MP3);
}

void HelloWorld::displayGameOver()
{
    Size winSize = Director::getInstance()->getWinSize();
    auto label = LabelBMFont::create("GAME OVER", S_ABADI40_FNT);
    label->setPosition(Vec2(winSize.width/2, winSize.height/2));
    label->setScale(0.2f);
    auto sa = ScaleTo::create(0.5f, 1.05f);
    auto sb1 = ScaleTo::create(0.5f, 1.0f);
    auto sb2 = ScaleTo::create(0.5f, 1.05f);
    auto seq = Sequence::create(sb1, sb2, NULL);
    auto rep = Repeat::create(seq, 1000);
    auto all = Sequence::create(sa, rep, NULL);
    label->runAction(all);
    this->addChild(label, Z_LABEL);
    this->enableCollisionEvent(false);
    auto item1 = MenuItemFont::create("Play Again", CC_CALLBACK_1(HelloWorld::onReStart, this));
    auto item2 = MenuItemFont::create("Main Menu", CC_CALLBACK_1(HelloWorld::onMainMenu, this));
    auto menu = Menu::create(item1, item2, NULL);
    menu->alignItemsVertically();
    menu->setPosition(Vec2(winSize.width/2, winSize.height/3));
    this->addChild(menu, Z_DEBUG_MENU);
    ScoreManager::getInstance()->addScore(this->_gameState->score);
}

void HelloWorld::enableTouchEvent(bool enabled)
{
    if (this->_touchListener != nullptr) {
        Director::getInstance()->getEventDispatcher()->removeEventListener(this->_touchListener);
        this->_touchListener = nullptr;
    }
    if (enabled) {
        this->_touchListener = EventListenerTouchOneByOne::create();
        _touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
        _touchListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
        _touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
        _touchListener->onTouchCancelled = CC_CALLBACK_2(HelloWorld::onTouchCancelled, this);
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchListener, this);
    }
}

void HelloWorld::enableCollisionEvent(bool enabled)
{
    if (enabled) {
        cpSpaceAddCollisionHandler(_space, COLLISION_TYPE_CAR, COLLISION_TYPE_COIN, HelloWorld::CollisionBeginCoin, NULL, NULL, NULL, this);
        cpSpaceAddCollisionHandler(_space, COLLISION_TYPE_CAR, COLLISION_TYPE_FINISH, HelloWorld::CollisionBeginFinish, NULL, NULL, NULL, this);
        cpSpaceAddCollisionHandler(_space, COLLISION_TYPE_FLOOR, COLLISION_TYPE_WATERMELON, HelloWorld::CollisionBeginWatermelon, NULL, NULL, NULL, this);
    } else {
        cpSpaceRemoveCollisionHandler(_space, COLLISION_TYPE_FLOOR, COLLISION_TYPE_WATERMELON);
        cpSpaceRemoveCollisionHandler(_space, COLLISION_TYPE_CAR, COLLISION_TYPE_FINISH);
        cpSpaceRemoveCollisionHandler(_space, COLLISION_TYPE_CAR, COLLISION_TYPE_COIN);
    }
}

cpBool HelloWorld::CollisionBeginCoin(cpArbiter *arb, cpSpace *space, void *data)
{
    CP_ARBITER_GET_SHAPES(arb, a, b);
    cpShape* ans = a->collision_type == COLLISION_TYPE_COIN ? a : b;
    auto classptr = (HelloWorld*)data;
    if (classptr->_shapesToRemove->size() == 0) {
        classptr->_shapesToRemove->push_back(ans);
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(S_PICKUP_COIN_MP3);
        classptr->addScore(1);
    }
    return true;
}

cpBool HelloWorld::CollisionBeginFinish(cpArbiter *arb, cpSpace *space, void *data)
{
    auto classptr = (HelloWorld*)data;
    classptr->setThrottle(0);
    classptr->setGameStateDeferred(STATE_LEVEL_COMPLETE);
    return true;
}

cpBool HelloWorld::CollisionBeginWatermelon(cpArbiter *arb, cpSpace *space, void *data)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(S_GAME_OVER_MP3);
    auto classptr = (HelloWorld*)data;
    classptr->setThrottle(0);
    classptr->setGameStateDeferred(STATE_GAME_OVER);
    return true;
}

void HelloWorld::initPhysics()
{
    this->_space = cpSpaceNew();
    this->_space->iterations = 20;
    this->_space->gravity = cpv(0, -GRAVITY);
    this->enableCollisionEvent(true);
    // debug only
    this->_debugNode = PhysicsDebugNode::create(this->_space);
    this->_debugNode->setVisible(false);
    //    this->addChild(this->_debugNode, Z_DEBUG_PHYSICS);
    // Parallax ratio and offset
    this->_scrollNode->addChild(this->_debugNode, Z_DEBUG_PHYSICS, Vec2(1,1), Vec2(0,0));
}

void HelloWorld::setupLevel(int level)
{
    std::string fullPath = CCFileUtils::getInstance()->fullPathForFilename("levels.json");
    ssize_t size = 0;
    unsigned char* pData = CCFileUtils::getInstance()->getFileData(fullPath, "r", &size);
    
    picojson::value value;
    std::string err;
    
    picojson::parse(value, pData, pData+strlen((const char*)pData), &err);
    if ( err.empty() ) {
        picojson::object& all = value.get<picojson::object>();
        picojson::array& levels = all["levels"].get<picojson::array>();
        m_maxLevel = levels.size();
        log("setupLevel maxLevel: %d", m_maxLevel);
        picojson::object& level_object = levels[level].get<picojson::object>();
        Size winSize = Director::getInstance()->getWinSize();
        float width = winSize.width;
        float height = winSize.height;
        int x, y;
        // Coins
        picojson::array& coins = level_object["coins"].get<picojson::array>();
        for (picojson::array::iterator coinit = coins.begin(); coinit != coins.end(); coinit++) {
            picojson::object& coin = coinit->get<picojson::object>();
            x = (int)coin["x"].get<double>();
            y = (int)coin["y"].get<double>();
            this->createCoin(cpv(x, y));
        }
        
        picojson::object& car_object = level_object["car"].get<picojson::object>();
        x = (int)car_object["x"].get<double>();
        y = (int)car_object["y"].get<double>();
        this->createCar(cpv(x, y));
        
        picojson::object& finish_object = level_object["finish"].get<picojson::object>();
        x = (int)finish_object["x"].get<double>();
        y = (int)finish_object["y"].get<double>();
        this->createFinish(cpv(x, y));
        
        x = 0;
        y = 0;
        std::vector<Vec2> poly;
        Vec2 p = Vec2::ZERO;
        picojson::array& lines = level_object["lines"].get<picojson::array>();
        for (picojson::array::iterator lit = lines.begin(); lit != lines.end(); lit++) {
            picojson::object& line = lit->get<picojson::object>();
            int line_x = (int)line["x"].get<double>();
            int line_y = (int)line["y"].get<double>();
            if (lit != lines.begin()) {
                this->createSegment(cpv(p.x, p.y), cpv(p.x + line_x, p.y + line_y));
                this->_terrain->drawSegment(Vec2(p.x, p.y), Vec2(p.x + line_x, p.y + line_y), 5, Color4F(0.43, 0.39, 0.34, 1));
            }
            p.x += line_x;
            p.y += line_y;
            poly.push_back(p);
            x = MIN(x, p.x);
            y = MIN(y, p.y);
            width = fmaxf(width, p.x);
            height= fmaxf(height, p.y);
        }
        poly.insert(poly.begin(), Vec2(x, y));
        Rect rect = Rect(x, y - 50, width, height + 200);
        auto Action = Follow::create(this->_carSprite, rect);
        this->_scrollNode->runAction(Action);
        this->createWorldBoundary(Rect(x, y, width, height));
    }
}

void HelloWorld::createFinish(cpVect pos)
{
    auto sprite = PhysicsSprite::createWithSpriteFrameName("farmers-market.png");
    Size cs = sprite->getContentSize();
    auto body = cpBodyNewStatic();
    sprite->setCPBody(body);
    cpBodySetPos(body, pos);
    
    auto shape = cpBoxShapeNew(body, cs.width, cs.height);
    cpShapeSetCollisionType(shape, COLLISION_TYPE_FINISH);
    cpShapeSetSensor(shape, true);
    
    cpSpaceAddStaticShape(this->_space, shape);
    this->_batch->addChild( sprite, Z_FINISH);
}

void HelloWorld::createWorldBoundary(Rect rect)
{
    auto staticBody = this->_space->staticBody;
    float x = rect.origin.x;
    float y = rect.origin.y;
    float w = rect.size.width;
    float h = rect.size.height;
    
    // Walls
    auto bottom = cpSegmentShapeNew(staticBody, cpv(x, y), cpv(w, y), 0);
    auto top    = cpSegmentShapeNew(staticBody, cpv(x, h), cpv(w, h), 0);
    auto left   = cpSegmentShapeNew(staticBody, cpv(x, y), cpv(x, h), 0);
    auto right  = cpSegmentShapeNew(staticBody, cpv(w, y), cpv(w, h), 0);
    // initialize Coefficient of restitution. (elasticity)
    cpShapeSetElasticity(bottom, 0);
    cpShapeSetElasticity(top, 0);
    cpShapeSetElasticity(left, 0);
    cpShapeSetElasticity(right, 0);
    // initialize Coefficient of friction.
    cpShapeSetFriction(bottom, 0);
    cpShapeSetFriction(top, 0);
    cpShapeSetFriction(left, 0);
    cpShapeSetFriction(right, 0);
    // initialize collision type
    cpShapeSetCollisionType(bottom, COLLISION_TYPE_FLOOR);
    cpShapeSetCollisionType(top, COLLISION_TYPE_FLOOR);
    cpShapeSetCollisionType(left, COLLISION_TYPE_FLOOR);
    cpShapeSetCollisionType(right, COLLISION_TYPE_FLOOR);
    // add shape
    cpSpaceAddStaticShape(this->_space, bottom);
    cpSpaceAddStaticShape(this->_space, top);
    cpSpaceAddStaticShape(this->_space, left);
    cpSpaceAddStaticShape(this->_space, right);
}

void HelloWorld::setThrottle(int throttle)
{
    if (throttle > 0) {
        // The motor is modeled like an electric motor where the torque decreases inversely as the rate approaches the maximum.
        // It's simple to code up and feels nice.
        
        cpFloat maxForce = cpfclamp01(1.0 - ((cpBodyGetAngVel(this->_chassis) - cpBodyGetAngVel(this->_frontWheel)) / ENGINE_MAX_W)) * ENGINE_MAX_TORQUE;
        cpConstraintSetMaxForce(this->_motor, maxForce);
        
        // Set the brakes to apply the baseline rolling friction torque.
        cpConstraintSetMaxForce(this->_frontBrake, ROLLING_FRICTION);
        cpConstraintSetMaxForce(this->_rearBrake, ROLLING_FRICTION);
    } else if (throttle < 0) {
        // Disable the motor.
        cpConstraintSetMaxForce(this->_motor, 0);
        
        // It would be a pretty good idea to give the front and rear brakes different torques.
        // The buggy as is now has a tendency to tip forward when braking hard.
        cpConstraintSetMaxForce(this->_frontBrake, BRAKING_TORQUE);
        cpConstraintSetMaxForce(this->_rearBrake, BRAKING_TORQUE);
    } else {
        // Disable the motor.
        cpConstraintSetMaxForce(this->_motor, 0);
        
        // Set the brakes to apply the baseline rolling friction torque.
        cpConstraintSetMaxForce(this->_frontBrake, ROLLING_FRICTION);
        cpConstraintSetMaxForce(this->_rearBrake, ROLLING_FRICTION);
    }
}

void HelloWorld::createSegment(cpVect src, cpVect dest)
{
    auto staticBody = this->_space->staticBody;
    auto segment = cpSegmentShapeNew(staticBody, src, dest, 5);
    cpShapeSetElasticity(segment, 1);
    cpShapeSetFriction(segment, 1);
    segment->collision_type = COLLISION_TYPE_FLOOR;
    cpSpaceAddStaticShape(this->_space, segment);
}

void HelloWorld::createCar(cpVect pos)
{
    this->_frontWheel = this->createWheel( cpvadd(pos, cpv(38.5,-25) ), 100);
    this->_chassis = this->createChassis(cpvadd(pos, COG_ADJUSTMENT));
    this->_rearWheel = this->createWheel( cpvadd(pos, cpv(-33, -26.5) ), 200);
    this->createCarJoints(this->_chassis, this->_frontWheel, this->_rearWheel);
    this->createCarFruits(pos);
    
    this->setThrottle(0);
}

cpBody* HelloWorld::createChassis(cpVect pos)
{
    auto sprite = PhysicsSprite::createWithSpriteFrameName("Chassis.png");
    Vec2 anchorPointInPoint = sprite->getAnchorPointInPoints();
    cpVect anchor = cpvadd(cpv(anchorPointInPoint.x, anchorPointInPoint.y), COG_ADJUSTMENT);
    Size cs = sprite->getContentSize();
    sprite->setAnchorPoint(Vec2(anchor.x/cs.width, anchor.y/cs.height));
    
    auto body = cpBodyNew(CHASSIS_MASS, cpMomentForBox(CHASSIS_MASS, cs.width, cs.height));
    cpBodySetPos(body, pos);
    
    sprite->setCPBody(body);
    
    cpSpaceAddBody(_space, body);
    this->_batch->addChild(sprite, Z_CHASSIS);
    this->_carSprite = sprite;
    
    // bottom of chassis
    auto shape = cpBoxShapeNew(body, cs.width, 15);
    cpShapeSetFriction(shape, 0.3f);
    cpShapeSetGroup(shape, GROUP_BUGGY);
    cpShapeSetLayers(shape, COLLISION_LAYERS_BUGGY);
    cpShapeSetCollisionType(shape, COLLISION_TYPE_CAR);
    
    cpSpaceAddShape(this->_space, shape);
    
    // box for fruits (left)
    shape = cpBoxShapeNew2(body, cpBBNew(-50, 0, -46, 30));
    cpShapeSetFriction(shape, 0.3);
    cpShapeSetGroup(shape, GROUP_BUGGY);
    cpShapeSetLayers(shape, COLLISION_LAYERS_BUGGY);
    cpShapeSetCollisionType(shape, COLLISION_TYPE_CAR);
    cpSpaceAddShape(this->_space, shape);
    
    // box for fruits (right)
    shape = cpBoxShapeNew2(body, cpBBNew(8, 0, 25, 30));
    cpShapeSetFriction(shape, 0.3);
    cpShapeSetGroup(shape, GROUP_BUGGY);
    cpShapeSetLayers(shape, COLLISION_LAYERS_BUGGY);
    cpShapeSetCollisionType(shape, COLLISION_TYPE_CAR);
    cpSpaceAddShape(this->_space, shape);
    
    return body;
}

cpBody* HelloWorld::createWheel(cpVect pos, int tag)
{
    auto sprite = PhysicsSprite::createWithSpriteFrameName("Wheel.png");
    float radius = 0.95 * sprite->getContentSize().width / 2;
    
    auto body = cpBodyNew(WHEEL_MASS, cpMomentForCircle(WHEEL_MASS, 0, radius, cpvzero));
    cpBodySetPos(body, pos);
    sprite->setCPBody(body);
    
    auto shape = cpCircleShapeNew(body, radius, cpvzero);
    cpShapeSetFriction(shape, 1);
    cpShapeSetGroup(shape, GROUP_BUGGY);
    cpShapeSetLayers(shape, COLLISION_LAYERS_BUGGY);
    cpShapeSetCollisionType(shape, COLLISION_TYPE_CAR);
    
    cpSpaceAddBody(this->_space, body);
    cpSpaceAddShape(this->_space, shape);
    this->_batch->addChild(sprite, Z_WHEEL, tag);
    
    return body;
}

void HelloWorld::createCarJoints(cpBody* chassis, cpBody* front, cpBody* rear)
{
    // The front wheel strut telescopes, so we'll attach the center of the wheel to a groov joint on the chassis.
    // I created the graphics specifically to have a 45 degree angle. So it's easy to just fudge the numbers.
    cpVect grv_a = cpBodyWorld2Local(chassis, cpBodyGetPos(front));
    cpVect grv_b = cpvadd( grv_a, cpvmult(cpv(-0.8, 1.3), 7));
    auto frontJoint = cpGrooveJointNew(chassis, front, grv_a, grv_b, cpvzero);
    
    // Create the front zero-length spring.
    cpVect front_anchor = cpBodyWorld2Local(chassis, cpBodyGetPos(front));
    auto frontSpring = cpDampedSpringNew(chassis, front, front_anchor, cpvzero, 0, FRONT_SPRING, FRONT_DAMPING);
    
    // The rear strut is a swinging arm that holds the wheel a at a certain distance from a pivot on the chassis.
    // A perfect fit for a pin joint conected between the chassis and the wheel's center.
    auto rearJoint = cpPinJointNew(chassis, rear, cpvsub(cpv(-5, -12), COG_ADJUSTMENT), cpvzero);
    
    // return cpvtoangle(cpvsub([_chassis.body local2world:_rearJoint.anchr1], _rearWheel.body.pos));
    //    cpFloat rearStrutRestAngle = cpvtoangle(cpvsub(cpBodyLocal2World(chassis, cpPinJointGetAnchr1(rearJoint)), cpBodyGetPos(rear)));
    
    // Create the rear zero-length spring.
    cpVect rear_anchor = cpBodyWorld2Local(chassis, cpBodyGetPos(rear));
    auto rearSpring = cpDampedSpringNew(chassis, rear, rear_anchor, cpvzero, 0, REAR_SPRING, REAR_DAMPING);
    
    // Attach a slide joint to the wheel to limit it's range of motion.
    auto rearStrutLimit = cpSlideJointNew(chassis, rear, rear_anchor, cpvzero, 0, 9.5);
    
    // The main motor that drives the buggy.
    auto motor = cpSimpleMotorNew(chassis, front, ENGINE_MAX_W);
    cpConstraintSetMaxForce(motor, 0.0);
    
    // I don't know if "differential" is the correct word, but it transfers a fraction of the rear torque to the front wheels.
    // In case the rear wheels are slipping. This makes the buggy less frustrating when climbing steep hills.
    auto differential = cpSimpleMotorNew(rear, front, 0);
    cpConstraintSetMaxForce(differential, ENGINE_MAX_TORQUE*DIFFERENTIAL_TORQUE);
    
    // Wheel brakes.
    // While you could reuse the main motor for the brakes, it's easier not to.
    // It won't cause a performance issue to have too many extra motors unless you have hundreds of buggies in the game.
    // Even then, the motor constraints would be the least of your performance worries.
    auto frontBrake = cpSimpleMotorNew(chassis, front, 0);
    cpConstraintSetMaxForce(frontBrake, ROLLING_FRICTION);
    auto rearBrake = cpSimpleMotorNew(chassis, rear, 0);
    cpConstraintSetMaxForce(rearBrake, ROLLING_FRICTION);
    
    cpSpaceAddConstraint(this->_space, frontSpring);
    cpSpaceAddConstraint(this->_space, rearStrutLimit);
    cpSpaceAddConstraint(this->_space, frontJoint);
    cpSpaceAddConstraint(this->_space, rearJoint);
    cpSpaceAddConstraint(this->_space, rearSpring);
    cpSpaceAddConstraint(this->_space, motor);
    cpSpaceAddConstraint(this->_space, differential);
    cpSpaceAddConstraint(this->_space, frontBrake);
    cpSpaceAddConstraint(this->_space, rearBrake);
    
    this->_motor = motor;
    this->_frontBrake = frontBrake;
    this->_rearBrake = rearBrake;
}

void HelloWorld::createCarFruits(cpVect pos)
{
    // create some fruits
    for(int i=0; i < 4;i++) {
        auto sprite = PhysicsSprite::createWithSpriteFrameName("watermelon.png");
        float radius = 0.95 * sprite->getContentSize().width / 2 * 0.85;
        sprite->setScale(0.85);
        
        auto body = cpBodyNew(WATERMELON_MASS, cpMomentForCircle(WATERMELON_MASS, 0, radius, cpvzero));
        cpBodySetPos(body, pos);
        sprite->setCPBody(body);
        
        auto shape = cpCircleShapeNew(body, radius, cpvzero);
        cpShapeSetFriction(shape, 1);
        cpShapeSetCollisionType(shape, COLLISION_TYPE_WATERMELON);
        
        cpSpaceAddShape(this->_space, shape);
        cpSpaceAddBody(this->_space, body);
        this->_batch->addChild(sprite, Z_WATERMELON);
    }
}

cpBody* HelloWorld::createCoin(cpVect pos)
{
    auto sprite = PhysicsSprite::createWithSpriteFrameName("coin01.png");
    float radius = 0.95 * sprite->getContentSize().width / 2;
    
    auto body = cpBodyNewStatic();
    cpBodySetPos(body, pos);
    sprite->setCPBody(body);
    
    auto shape = cpCircleShapeNew(body, radius, cpvzero);
    shape->u = 0;
    shape->group = GROUP_COIN;
    shape->collision_type = COLLISION_TYPE_COIN;
    shape->sensor = true;
    
    // rogue ("orphan") bodies, needs to be added to a container, otherwise they could be GC'd
    //    this._rogueBodies.push( body );
    cpSpaceAddStaticShape(_space, shape);
    this->_batch->addChild(sprite, Z_COIN);
    
    auto animation = AnimationCache::getInstance()->getAnimation("coin");
    auto animate = Animate::create(animation);
    auto repeat = RepeatForever::create(animate);
    sprite->runAction(repeat);
    
    // Needed for deletion
    cpBodySetUserData(body, sprite);
    
    return body;
    
}
