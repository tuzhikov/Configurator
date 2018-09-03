#ifndef CMD_DEVICE_H
#define CMD_DEVICE_H

#include <stdint.h>
#include <time.h>
#include "src/ACCESSDATE/datastructure.h"
#define COD_CLEAR_WAV 32
#define CAT_(a, b) a ## b
#define CAT(a, b) CAT_(a, b)
#define STATIC_ASSERT(cond) typedef int CAT(STATIC_ASSERT, __LINE__)[(cond) ? 1 : -1]

enum TYPE_TRACK{
    btZSR, btZSO,btZSP, btZSZ,btZSVRR, btZSVRZ, btTVP};
enum NUMBER_SPEAKER{
    SPEAKER_ONE,SPEAKER_TWO};

enum TYPE_NUMBER_COMMAND_TRANSPORT{
    CMD_TR_ERROR = 0x00,
    CMD_TR_WRITE = 0x01,
    CMD_TR_READ  = 0x02,
    END_TYPE_NUMBER_COMMAND_TRANSPORT
};

enum TYPE_NUMBER_COMMAND{
    CMD_OPCODE_RS485		= 0x00
  , CMD_OPCODE_TIME			= 0x02
  , CMD_OPCODE_SETTINGS     = 0x04
  , CMD_OPCODE_DAY_PLANS    = 0x06
  , CMD_OPCODE_WEEK         = 0x08
  , CMD_OPCODE_HOLIDAYS     = 0x10
  , CMD_OPCODE_FLASH        = 0x20
  , CMD_OPCODE_STATUS		= 0x40
  , CMD_ERROR_OPCODE        = 0x80
  , CMD_ERROR_OPCODE_CRC16	= 0xFE
  , CMD_MAX_OPCODE = 11
};

enum TYPE_NUMBER_COMMAND_TEST{
    CMD_TEST_LISTEN     = 0x00,
    CMD_TEST_DEVICE     = 0x01,
    CMD_TEST_ERROR      = 0xEE,
    END_TYPE_NUMBER_COMMAND_TEST
};

enum TYPE_DEVICE{
    TYPE_TL  = 0x10,
    END_TYPE_DEVICE
};

enum TYPE_ERROR{
    ERR_FLH     = 0x00000001, // ошибка записи flash
    ERR_DP      = 0x00000002, // ошибка цифрового потенциометра
    ERR_ETH     = 0x00000004, //  ошибка работы esp8266
    ERR_GPS     = 0x00000008  //  ошибка работы gps
};

enum TYPE_TEST_FLAG{
    TEST_RED        = 0x00000001, // активный красный
    TEST_GREEN      = 0x00000002, // активный зеленый
    TEST_WAIT       = 0x00000004, // активный ждите
    TEST_GPS        = 0x00000008, // активный GPS (отвечает есть один спутик)
    TEST_BUTTON     = 0x00000010, // кнопка нажата
    TEST_MOTION     = 0x00000020, // датчик движения сработал
    END_TYPE_TEST_FLAG
};

enum FILE_HEAD_WAV{
    MAX_LENGH_FILE_NAME = 64,
    MAX_LENGH_HEAD_WAV  = 44,
    END_FILE_PREFIX
};

enum WIFI_SETTING{
    MAX_NAME = 32,
    MAX_PASSWORD = 32,
    END_WIFI_SETTING
};

const uint8_t HeaderProtocol1 [] = { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA };
const uint8_t HeaderProtocol2 [] = { 0x55, 0xC3, 0x5A };
//
struct HEADER{
    char *pHeader1;
    char *pHeader2;
    uint32_t lengn1;
    uint32_t lengn2;
};
const HEADER header = {
    (char*)(HeaderProtocol1),
    (char*)(HeaderProtocol2),
    sizeof(HeaderProtocol1),
    sizeof(HeaderProtocol2)
};

#pragma pack(push,1)
// data error
struct DATAERROR {
    uint32_t error;
    uint16_t ver; // версия прошивки.
};
struct TYPE_TEST_ONE_TRACK {
    uint8_t sound_sp;     //
    uint8_t value_sk;     // уровень громкости на динамике 1
    uint8_t ct_plan;      //
};

// structure time RTC
struct TYPETIME{
    //uint64_t time;  // представление времени с сек.
    //int8_t gmt;    // часовой пояс
    uint8_t     sec;     // 0-59
    uint8_t     min;     // 0-59
    uint8_t     hour;    // 0-23
    uint8_t     weekday; // 0-6  // 0 - Sunday
    uint8_t     day;     // 1-31
    uint8_t     month;   // 1-12 // 1 - Jan
    uint16_t    year;    // any positive
};
STATIC_ASSERT(sizeof(TYPETIME) == 8);
// structure test
struct TYPE_STATUS {
    TYPETIME time;                  // время прибора
    int8_t   HolidayIndex;
    uint8_t  CurPlan;
    uint8_t  CurItem;
    uint8_t  CurMode;
    uint16_t CurBrightRG;  // %
    uint16_t CurBrightY;   // %
    uint16_t CurVolume;    // %
    uint8_t  RedState;
    uint8_t  YellowState;
    uint8_t  GreenState;
    uint8_t  MainState;
    int16_t  RCount;
    int16_t  GCount;
    uint16_t Errors;
    int16_t  SensorValue;
    int8_t   SensorFlags;
    int8_t   bSensorMode;
    int8_t   Reserved[6];
};
STATIC_ASSERT(sizeof(TYPE_STATUS) == 38);

#pragma pack(pop)

#endif // CMD_DEVICE_H
