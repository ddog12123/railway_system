//
// Created by sy200 on 24-12-17.
//
#include <format>
#include <iostream>
#include "train_model.h"
#include <print>
#include "train.h"
#include <utils.h>
#include <cstring>
#include <ctime>
#include <stdio.h>

void printTrain(TrainData &p){
    std::wstring status;
    switch (p.train_status) {
        case TRAIN_STATUS_NORMAL:
            status=L"正点";
            break;
        case TRAIN_STATUS_CANCELLED:
            status=L"取消";
            break;
        case TRAIN_STATUS_DELAYED:
            status=L"晚点";
            break;
        case TRAIN_STATUS_STOPPED:
            status=L"停运";
            break;
        case TRAIN_STATUS_UNKNOWN:
            status=L"未知状态";
            break;
        default:
            status=L"错误";
    }
    wchar_t stime[1024]={0};
    wchar_t atime[1024]={0};
    wchar_t number[5]={0};
    tm *stime1=localtime(&p.start_time);
    tm stime2;
    size_t err=wcsftime(stime, 1024, L"%H:%M:%S, %J %B %Y", localtime(&p.start_time));
    wcsftime(atime, 1024, L"%H:%M:%S, %J %m %Y", localtime(&p.arrive_time));
    wchar_t start_station[32];
    wchar_t arrive_station[32];
    size_t ret[32];
    mbstowcs_s(ret, start_station,64, p.start_station,31);
    mbstowcs_s(ret, arrive_station,64, p.arrive_station,31);
    mbstowcs_s(ret, number,5, p.number,31);
    wprintf(L"%3d %4ls %4ls %4ls %ls %ls %4.2f %4d %ls\n",
                  p.id, number, start_station, arrive_station,
                  stime, atime, ((float)p.ticket_price)/100, p.ticket_remain, status.c_str());
    fflush(stdout);
}
void test_analyse(){
    time_t time1;
    time1=std::time(NULL);
    char *buffer=(char *)calloc(1024, sizeof(char ));
    char num[]="G2222";
    TrainChange change;
    memset(&change,0,sizeof(TrainChange));
    change.ticket_remain=5;
    change.change_ticket_remain=true;
    change.ticket_price=50000;
    change.change_ticket_price=true;
    change.number=num;
    change.change_number=true;
    change.arrive_station=num;
    change.train_status=TRAIN_STATUS_NORMAL;
    change.change_is_running=true;
    analyzeChange(&change, buffer, 1024);
    memset(buffer, 0, 1024);
    std::string str(buffer);
    std::cout<<str<<std::endl;
    TrainQuery query;
    memset(&query,0,sizeof(TrainChange));
    query.ticket_remain=5;
    query.query_ticket_remain=IGNORE_THIS;
    query.query_is_running=EQUAL;
    query.train_status= TRAIN_STATUS_NORMAL;
    query.query_ticket_remain=GREATER;
    query.ticket_remain=500;
    analyzeCondition(&query, buffer, 1024);
    time_t time2;
    time2=std::time(NULL);
    std::println("{}",buffer);
    std::println("Spent {} seconds.", time2-time1);
}

void test_db(){

    //添加
    TrainData *data=(TrainData*) calloc(10,sizeof(TrainData));

    for(int i=0;i<10;i++){
        std::srand((unsigned int)time(NULL));
        int r=rand();
        sprintf(data[i].number, "%x", r);
        data[i].start_time=3600*24*i+time(NULL);
        data[i].arrive_time=3600*24*(i+1)+time(NULL);
        r=rand();
        sprintf(data[i].start_station, "%x", r);
        r=rand();
        sprintf(data[i].arrive_station, "%x", r);
        r=rand()%1000;
        data[i].ticket_remain=r;
        r=rand()%114514;
        data[i].ticket_price=r;
        data[i].train_status=rand()%4;
    }

    addTrainList(data, 10);
    free(data);
    TrainQuery query;
    memset(&query, 0, sizeof(TrainQuery));

    query.ticket_remain=500;
    query.query_ticket_remain= true;
    TrainData *queryData= (TrainData*) malloc(5* sizeof(TrainData));
    uint32_t num;
    getTrainList(&query, queryData, 5, 1, &num);

    std::println("Condition:");
    for(int i=0;i<num;i++){
        printTrain(queryData[i]);
    }
    TrainQuery queryAll;
    memset(&queryAll, 0, sizeof(TrainQuery));

    queryAll.id=0;
    queryAll.query_id=GREATER;
    TrainData *queryALLData= (TrainData*) malloc(5* sizeof(TrainData));
    uint32_t numAll;
    getTrainList(&queryAll, queryALLData, 5, 1, &numAll);
    std::println("All - Page 1:");
    for(int i=0;i<numAll;i++){
        printTrain(queryALLData[i]);
    }
    free(queryALLData);
    TrainData *queryALLData2= (TrainData*) malloc(5* sizeof(TrainData));
    uint32_t numAll2;
    getTrainList(&queryAll, queryALLData2, 5, 1, &numAll2);
    std::println("All - Page 2:");
    for(int i=0;i<numAll2;i++){
        printTrain(queryALLData2[i]);
    }
    free(queryALLData2);

    TrainChange change;
    change.train_status=TRAIN_STATUS_STOPPED;
    change.change_is_running= true;
    editTrain(&query, &change);
    TrainData* edit=(TrainData*) malloc(10* sizeof(TrainData));
    fflush(stdout);
    std::println("After change:");
    fflush(stdout);
    getTrainList(&query, edit, 10, 1, &num);
    for(int i=0;i<num;i++){
        printTrain(edit[i]);
    }


    deleteTrain(&query);
    fflush(stdout);

    TrainData* deleted=(TrainData*) malloc(10* sizeof(TrainData));
    uint32_t numRemain;
    getTrainList(&query, deleted, 10, 1, &numRemain);
    if(numRemain==0){
        std::println("Delete Successfully.");
    }
    else{
        std::println("\033[31mDelete Successfully.\033[0m");
    }
    free(deleted);
}
int main(){
    setlocale(LC_CTYPE, "");
    initConfig();
    initDb();
    char *sql=(char *) calloc(1024, sizeof(char));
    char *buffer=(char *) calloc(1024, sizeof(char));
    sprintf_s(sql,1024, "select count(*) from trains;");
    char** resultp=NULL;
    int row=0;
    int col=0;
    char *errMsg=NULL;
    int ret=sqlite3_get_table(db,sql,&resultp,&row, &col, &errMsg);
    free(sql);
    sql=NULL;
    free(buffer);
    buffer=NULL;
    if(ret!=SQLITE_OK){
        printf("\033[31m%s",errMsg);
        sqlite3_free(errMsg);
        errMsg=NULL;
        return ret;
    }
    printf("row=%d, col=%d, result=%s", row, col, resultp[1*1]);
    sqlite3_free(errMsg);
    finalizeDb();
    return 0;
}