//
//  SaveSQL.cpp
//
//
//  Created by albatrus on 2014/06/10.
//
//

#include "SaveSQL.h"
#define dbName "english-database.db"

USING_NS_CC;

//データベースを開く
int SaveSQL::sqliteOpen(sqlite3 **db){

    auto filePath = FileUtils::getInstance()->getWritablePath();
    filePath += dbName;
    log("接続先は%s",filePath.c_str());
    
    
    return sqlite3_open(filePath.c_str(),db);
}




//データベースのテーブルの作成
void SaveSQL::sqliteCreateTable(){
    
    //データベースを作成
    sqlite3 *db = NULL;
    if(sqliteOpen(&db) == SQLITE_OK){
        
        
//データベースの削除
        char *errMsg = NULL;
        int err = 0;

        err = sqlite3_exec(db,
                           "DROP TABLE IF EXISTS questions",
                           NULL, NULL, &errMsg);
        
        if(err != SQLITE_OK){
            printf("%s\n", errMsg);
            sqlite3_free(errMsg);
            errMsg = NULL;
            /* TODO: エラー処理 */
        }
//データベース削除終
        
        
        //テーブル(questions)を作成
        //列: no:int primary key auto ,text:text,correctCount:int,inCorrectCount:int,reviewFlag:int
        const char *sql_createtable = "CREATE TABLE questions(questionId integer primary key,type  text,japanese text,english text)";
        sqlite3_stmt *stmt = NULL;
        if (sqlite3_prepare_v2(db, sql_createtable, -1, &stmt, NULL) == SQLITE_OK) {
            
            if (sqlite3_step(stmt) == SQLITE_DONE) {
                
                CCLOG("create table done");

            }
            
            sqlite3_reset(stmt);
            
        }
        
        sqlite3_finalize(stmt);

        
        
    }
    
    sqlite3_close(db);
        
    
    
    
    
}

//keyから値を取り出す。
Vector<Question*> *SaveSQL::sqliteGetValueForKey(const char *key){
    
    sqlite3 *db = NULL;
    //配列を作成
    Vector<Question*> *questions = new Vector<Question*>;
    
    if (sqliteOpen(&db) == SQLITE_OK) {
        
//種別に合わせて取り出す問題を決める

        //sql コマンドの作成
        std::string sql = StringUtils::format("SELECT * FROM questions WHERE type = '%s' ORDER BY RANDOM() LIMIT 10",key);

        
        //const char型に変換
        const char *sql_select = sql.c_str();
        CCLOG("%s",sql_select);
        sqlite3_stmt *stmt = NULL;
        
        
        if (sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL) == SQLITE_OK) {
            
            int r;
            while (SQLITE_ROW == (r=sqlite3_step(stmt))){

                //宣言(for文でクラス宣言する場合、 newしないと同番地のメモリアドレスが指定され逐一上書きされる模様)
                /* Question *question;だと駄目 */
                Question *question = new Question;
                //格納
                //id
                question->questionId = (int)sqlite3_column_int(stmt, 0);
                //種別
                question->type = (std::string)StringUtils::format("%s", sqlite3_column_text(stmt, 1));
                //日本語
                question->japanese = (std::string)StringUtils::format("%s", sqlite3_column_text(stmt, 2));
                //英語
                question->english = (std::string)StringUtils::format("%s", sqlite3_column_text(stmt, 3));
                
                //配列に挿入
                questions->pushBack(question);

            }
            if (SQLITE_DONE!=r){
                // エラー
                log("DB error :実行エラー");
                sqlite3_reset(stmt);
            }
            
        }
        
        
        sqlite3_finalize(stmt);
        
    }
    
    sqlite3_close(db);
    
    
    
    return questions;
}


void SaveSQL::sqliteSetValue(){
    
    
    std::string fullpath = CCFileUtils::getInstance()->getWritablePath();
    fullpath += dbName;
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt;
    if (sqlite3_open(fullpath.c_str(), &db) == SQLITE_OK) {
        
        
        //questionsテーブルにデータを追加
            const char *sql_select = "insert into questions(questionId,type,japanese,english)VALUES(?,?,?,?)";
            stmt = NULL;
            
            if (sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL) == SQLITE_OK) {
                
                //データをインサート
                for (Question *question : *ImportCSV::importCSV()) {
                
                    
                    sqlite3_bind_int(stmt, 1, question->questionId);
                    sqlite3_bind_text(stmt, 2, question->type.c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 3, question->japanese.c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 4, question->english.c_str(), -1, SQLITE_TRANSIENT);

                    
                    if (sqlite3_step(stmt) == SQLITE_DONE) {
                        
                        log("insert sucess");
                        
                    }
                    
                    sqlite3_reset(stmt);

                    
                }
                
            }
        
        
        sqlite3_finalize(stmt);
        
    }
    
    sqlite3_close(db);
}