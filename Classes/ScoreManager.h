//
//  ScoreManager.h
//  ccs2dx_v3tutorial001
//
//  Created by noguchika on 2014/02/24.
//
//

#ifndef __ccs2dx_v3tutorial001__ScoreManager__
#define __ccs2dx_v3tutorial001__ScoreManager__

#include <iostream>
#include "cocos2d.h"
#include "picojson.h"

#define SCORE_STORAGE_KEY "SCORE"

class ScoreManager
{
private:
    std::vector<int> *_score;
    static ScoreManager* _instance;
public:
    static ScoreManager* getInstance();
    ScoreManager();
    ~ScoreManager();
    bool init();
    std::vector<int>* getScores();
    void addScore(int value);
};

#endif /* defined(__ccs2dx_v3tutorial001__ScoreManager__) */
