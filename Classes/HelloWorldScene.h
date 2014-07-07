#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "chipmunk.h"
#include "CCBLoaderLayer.h"
#include "Constant.h"

USING_NS_CC;
USING_NS_CC_EXT;

class GameState
{
public:
    GameState()
    : level(0)
    , score(0)
    , time(0)
    , state(STATE_PAUSE)
    {}
    GameState(int lvl)
    : level(0)
    , score(0)
    , time(0)
    , state(STATE_PAUSE)
    {
        level = lvl;
    }
    int level;
    int score;
    float time;
    int state;
};

class HelloWorld : public CCBLoaderLayer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(GameState* gameState);
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init(GameState* gameState);
    
    // implement the "static create()" method manually
    static HelloWorld* create(GameState* gameState);
    HelloWorld();
    ~HelloWorld();
    virtual void dispose();
    virtual void onEnter();
    virtual void onExit();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float delta);
    virtual bool onTouchBegan(Touch *touch, Event *event);
    virtual void onTouchMoved(Touch *touch, Event *event);
    virtual void onTouchEnded(Touch *touch, Event *event);
    virtual void onTouchCancelled(Touch *touch, Event *event);
    void onNextLevel(cocos2d::Ref * sender);
    void onMainMenu(cocos2d::Ref * sender);
    void onReStart(cocos2d::Ref * sender);
    void onRemoveMe(cocos2d::Ref * sender);
    
    static cpBool CollisionBeginCoin(cpArbiter *arb, cpSpace *space, void *data);
    static cpBool CollisionBeginFinish(cpArbiter *arb, cpSpace *space, void *data);
    static cpBool CollisionBeginWatermelon(cpArbiter *arb, cpSpace *space, void *data);
    
private:
    float _scaleFactor;
    std::vector<cpShape*> *_shapesToRemove;
    CC_SYNTHESIZE(int, m_level, Level);
    int m_maxLevel;
    GameState* _gameState;
    int _deferredState;
    cpSpace* _space; // strong ref
    PhysicsDebugNode* _debugNode;
    cpConstraint* _motor;
    cpConstraint* _frontBrake;
    cpConstraint* _rearBrake;
    PhysicsSprite* _carSprite;
    cpBody* _chassis;
    cpBody* _frontWheel;
    cpBody* _rearWheel;
    SpriteBatchNode* _batch;
    ParallaxNode* _scrollNode;
    ParticleSystemQuad* _carSmoke;
    DrawNode* _terrain;
    LabelBMFont* _scoreLabel;
    LabelBMFont* _timeLabel;
    
    /** Touch listener */
    EventListenerTouchOneByOne* _touchListener;
    
    void initHUD();
    void addScore(int value);
    void setGameStateDeferred(int state);
    void setGameState(int state);
    void displayLevelComplete();
    void displayGameOver();
    void enableTouchEvent(bool enabled);
    void enableCollisionEvent(bool enabled);
    void initPhysics();
    void setupLevel(int level);
    void setThrottle(int throttle);
    void createSegment(cpVect src, cpVect dest);
    void createCar(cpVect pos);
    cpBody* createChassis(cpVect pos);
    cpBody* createWheel(cpVect pos, int tag);
    void createCarJoints(cpBody* chassis, cpBody* front, cpBody* rear);
    void createCarFruits(cpVect pos);
    cpBody* createCoin(cpVect pos);
    void createFinish(cpVect pos);
    void createWorldBoundary(Rect rect);
};

#endif // __HELLOWORLD_SCENE_H__
