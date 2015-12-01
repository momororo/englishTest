//
//  GameScene.h
//  boki-3
//
//  Created by yasutomo on 2015/06/08.
//
//

#ifndef __boki_3__GameScene__
#define __boki_3__GameScene__

#include "cocos2d.h"
#include <vector>
#include <string>
#include <sstream>
#include "Config.h"

using namespace std;

class GameScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
    
    //タッチイベント用メソッドの宣言
    //TouchBegan()関数->タッチ開始
    bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
    //TouchMoved()関数->タッチ中
//    void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    //TouchCancelled()関数->タッチキャンセル
    void onTouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    //TouchEnded()関数->タッチ終了
    void onTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
    
    //Update
//    void update(float delta);

    //文字処理メソッド
    vector<string> split(const string &str, const string &delim);

    //配列
    cocos2d::Vector<Question*> *questions = new cocos2d::Vector<Question*>;
    //配列2(出題リスト)
    cocos2d::Vector<Question*> *ListOfQuestions = new cocos2d::Vector<Question*>;
    //選ばれた問題
    cocos2d::Label* questionLabel;
    //選択肢の配列
    cocos2d::Vector<cocos2d::Sprite*> *choices = new cocos2d::Vector<cocos2d::Sprite*>;

    
    
    //問題カウント
    int questionCount = 0;
    //正解カウント
    int correctCount = 0;
    
    //問題文作成
    void makeQuestionText();
    //選択肢作成
    void makeChoiceText();
    //解答作成処理
    void makeAnswer();
};


#endif /* defined(__boki_3__GameScene__) */
