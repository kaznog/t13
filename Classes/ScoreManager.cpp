//
//  ScoreManager.cpp
//
//  Created by kaznog on 2014/02/24.
//
//

#include "ScoreManager.h"

ScoreManager* ScoreManager::_instance = nullptr;

ScoreManager* ScoreManager::getInstance()
{
    if (!_instance) {
        _instance = new ScoreManager();
        if (_instance && _instance->init()) {
        } else {
            CC_SAFE_DELETE(_instance);
            _instance = nullptr;
        }
    }
    return _instance;
}

ScoreManager::ScoreManager()
: _score(nullptr)
{
    
}

ScoreManager::~ScoreManager()
{
    if (_score->size() != 0) {
        _score->clear();
    }
    delete _score;
}

bool ScoreManager::init()
{
    auto pUserDefault = cocos2d::UserDefault::getInstance();
    std::string score_storage = pUserDefault->getStringForKey(SCORE_STORAGE_KEY);
    if (score_storage != "") {
        picojson::value value;
        std::string err;
        picojson::parse(value, score_storage.c_str(), score_storage.c_str()+strlen(score_storage.c_str()), &err);
        if (err.empty()) {
            _score = new std::vector<int>();
            picojson::object& all = value.get<picojson::object>();
            picojson::array& scores = all["scores"].get<picojson::array>();
            for (picojson::array::iterator scoreit = scores.begin(); scoreit != scores.end(); scoreit++) {
                int score = (int)scoreit->get<double>();
                _score->push_back(score);
            }
        } else {
            _score = new std::vector<int>{ 0, 0, 0, 0, 0 };
        }
    } else {
        _score = new std::vector<int>{ 0, 0, 0, 0, 0 };
    }
    return true;
}

std::vector<int>* ScoreManager::getScores()
{
    return _score;
}

void ScoreManager::addScore(int value)
{
    using namespace picojson;
    _score->push_back(value);
    sort(_score->begin(), _score->end());
    _score->erase(_score->begin());
    reverse(_score->begin(), _score->end());
    picojson::object o;
    picojson::array scores;
    for (std::vector<int>::iterator it = _score->begin(); it != _score->end(); it++) {
        int score_value = *it;
        picojson::value picojson_value((double)score_value);
        scores.push_back(picojson_value);
        
    }
    picojson::object obj;
    obj.insert(std::make_pair("scores", picojson::value(scores)));
    picojson::value json(obj);
    cocos2d::UserDefault* pUserDefault = cocos2d::UserDefault::getInstance();
    pUserDefault->setStringForKey(SCORE_STORAGE_KEY, json.serialize().c_str());
    pUserDefault->flush();
}

