//
//  SaveSQL.cpp
//
//
//  Created by albatrus on 2014/06/10.
//
//

#include "ImportCSV.h"

USING_NS_CC;


Vector<Question*>* ImportCSV::importCSV()
{
    
    //データの読み込み
    Vector<Question*>*questions = new Vector<Question*>;
    //ファイルの指定
    std::string csvStr = FileUtils::getInstance()->getStringFromFile("question.csv");
    //改行を区切りにして分割する
    ValueVector balls = split(csvStr, "\n");
    //最初の行は列名のため1からループスタート
    for (int i = 1; i < (int)balls.size()-1 ; i ++) {
        
        //1行をカンマを区切りにして分割する
        ValueVector params = split(balls.at(i).asString(), ",");
        //オブジェクトを宣言
        Question *question = new Question;
        //1:id 2:種別 3:日本語 4:英語
        question->questionId = params.at(0).asInt();
        question->type = params.at(1).asString();
        question->japanese = params.at(2).asString();
        question->english = params.at(3).asString();
        
        questions->pushBack(question);
        
    }

    return questions;
    
}


ValueVector ImportCSV::split(const std::string &str, const std::string &delim){
    ValueVector res;
    size_t current = 0, found;
    while((found = str.find_first_of(delim, current)) != std::string::npos){
        res.push_back(Value(std::string(str, current, found - current)));
        current = found + 1;
    }
    res.push_back(Value(std::string(str, current, str.size() - current)));
    return res;
}

