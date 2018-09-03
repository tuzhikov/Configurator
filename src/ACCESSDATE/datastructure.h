#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <stdint.h>

enum MAX_PROJRCT{
    MAX_SOUND_NUMBER    = 4,
    MAX_DAY_WEEK        = 7,    //
    MAX_PLAN_ITEMS      = 24,   // планов в день
    MAX_DAY_PLANS       = 12,   // дней с заданными планами
    MAX_HOLIDAYS        = 20,   // праздничный дней
    END_ENUM
};

#pragma pack(push,1)
///< описание одного плана
struct TYPEPLANITEMS
{
    int8_t  hour;
    int8_t  min;
    uint8_t BrightRG;  // %
    uint8_t Volume;    // %
    uint8_t bSensor;   // bool
};
///< количество планов в одном дне
struct TYPEDAY
{
    TYPEPLANITEMS Items[MAX_PLAN_ITEMS];
    uint8_t ActiveItems;
};
///< план на один день недели
struct TYPEWEEK
{
    int8_t plan;
};
///< план для праздничного дня.
struct TYPEHOLIDAY
{
    int8_t day;
    int8_t month;
    int8_t plan;
    int8_t reserved;
};

///<общие настройки.
struct TYPESETTING
{
    uint8_t SampleNum;        // 0-3
    uint8_t TeachingPeriods;  // 1-2
    uint8_t bSpeedUp;         // flag
    uint8_t SpeedUpTime;      // sec
    uint8_t Direction;        //
    uint8_t mode_manual;      // flag
};
///< project data
struct TYPEPROJECT{
    TYPEHOLIDAY         holidays[MAX_HOLIDAYS];     ///<праздничные дни
    TYPEDAY             day_plans[MAX_DAY_PLANS];   ///<дни с планами
    TYPEWEEK            week[MAX_DAY_WEEK];         ///<неделя с планами
    TYPESETTING         setting;                    ///<общие настройки.
};
#pragma pack(pop)

#endif // DATASTRUCTURE_H
