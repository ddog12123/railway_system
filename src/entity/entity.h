
// train.h

#pragma once

#include <stdint.h>
#include <string>

struct Date
{
    int year  = 0;
    int month = 0;
    int day   = 0;

    int hour   = 0;
    int minute = 0;
    int second = 0;
};

// 枚举车次状态：正常、延期、停运、其他
enum class TrainStatus
{
    NORMAL,
    DELAY,
    STOP,
    OTHER
};

// 车次结构体
struct TrainData
{
    int         train_id = -1; // 唯一 ID
    std::string train_number;  // 车次

    std::string train_start_station;  // 始发站
    std::string train_arrive_station; // 到达站

    Date train_start_time;  // 出发时间
    Date train_arrive_time; // 到达时间

    int   train_ticket_count = 0;    // 票数
    float train_ticket_price = 0.0f; // 价格

    TrainStatus train_status = TrainStatus::OTHER; // 是否有效
};

std::string
date_to_string(const Date& date);

std::string
parse_train_status(TrainStatus status);