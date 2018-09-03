#ifndef ACCESSDATA_H
#define ACCESSDATA_H

#include <stdint.h>
#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QDataStream>
#include "datastructure.h"

namespace ACCESS_DATE{

    class AccessData
    {
    private:
        TYPEPROJECT *pDataProject;
    public:
        explicit AccessData();
        ~AccessData();
        // max value
        inline uint32_t getMaxPlans( void ){return MAX_DAY_PLANS;}
        inline uint32_t getMaxItemDday( void ){return MAX_PLAN_ITEMS;}
        inline uint32_t getMaxHoliday( void ){return MAX_HOLIDAYS;}
        inline uint32_t getMaxDayWeek( void ){return MAX_DAY_WEEK;}
        inline uint32_t getLengthDate( void ){ return sizeof(TYPEPROJECT); }
        inline uint32_t getLengthOneDatePlan( void ){ return sizeof(TYPEDAY); }
        inline uint32_t getLengthDateHoliday( void ){ return sizeof(TYPEHOLIDAY)*MAX_HOLIDAYS; }
        inline uint32_t getLengthDateWeek( void ){ return sizeof(TYPEWEEK)*MAX_DAY_WEEK; }
        inline uint32_t getLengthDateSetting( void ){ return sizeof(TYPESETTING); }
        // crc16
        bool checkCRC(){return true;}
        // bin all project
        QByteArray getDataStructure()const
        {
            void *pVoid = pDataProject;
            char *pC = static_cast <char*> (pVoid);
            return QByteArray(pC,sizeof(TYPEPROJECT));
        }
        void setDateStructure(QByteArray &data)
        {
            memcpy(pDataProject,data.constData(),sizeof(TYPEPROJECT));
        }
        // bin One plan
        QByteArray getDataOnePlan(size_t index)const
        {
            if ( index < MAX_DAY_PLANS )
            {
                void* p_void = &pDataProject->day_plans[index];
                char* p_char = static_cast <char*> (p_void);
                return QByteArray(p_char,sizeof(TYPEDAY));
            }

            return QByteArray();
        }
        bool setDateOnePlan(size_t index, QByteArray &data)
        {
            if ( index < MAX_DAY_PLANS )
            {
                void* p_void = &pDataProject->day_plans[index];
                memcpy(p_void,data.constData(),sizeof(TYPEDAY));

                return true;
            }

            return false;
        }
        // bin HoliDay
        QByteArray getDataHoliDay()const
        {
            void* p_void = pDataProject->holidays;
            char* p_char = static_cast <char*> (p_void);
            return QByteArray(p_char,sizeof(TYPEHOLIDAY)*MAX_HOLIDAYS);
        }
        void setDateHoliDay(QByteArray &data)
        {
            memcpy(pDataProject->holidays,data.constData(),sizeof(TYPEHOLIDAY)*MAX_HOLIDAYS);
        }
        // bin Week
        QByteArray getDataWeek()const
        {
            void* p_void = pDataProject->week;
            char* p_char = static_cast <char*> (p_void);
            return QByteArray(p_char,sizeof(TYPEWEEK)*MAX_DAY_WEEK);
        }
        void setDateWeek(QByteArray &data)
        {
            memcpy(pDataProject->week,data.constData(),sizeof(TYPEWEEK)*MAX_DAY_WEEK);
        }
        // bin setting
        QByteArray getDataSetting()const
        {
            void* p_void = &pDataProject->setting;
            char* p_char = static_cast <char*> (p_void);
            return QByteArray(p_char,sizeof(TYPESETTING));
        }
        void setDateSetting(QByteArray &data)
        {
            memcpy(&pDataProject->setting,data.constData(),sizeof(TYPESETTING));
        }

        //default parametr
        void setDefault(TYPEPROJECT *const pPrg);
        void clearProject();
        // Setters
        void setSetting ( const uint8_t SampleNum,
                          const uint8_t TeachingPeriods,
                          const uint8_t bSpeedUp,
                          const uint8_t SpeedUpTime,
                          const uint8_t Direction,
                          const uint8_t mode_manual );
        void setSetting ( const TYPESETTING prm );

        bool setOnePlanDay( const uint8_t num_plan,
                            const uint8_t item,
                            const uint8_t hour,
                            const uint8_t min,
                            const uint8_t bright_rg,
                            const uint8_t volume,
                            const uint8_t sensor );
        bool setOnePlanDay( const uint8_t num_plan,
                            const uint8_t item,
                            const TYPEPLANITEMS prm );

        bool setActiveOnePlanDay( const uint8_t num_plan );

        bool setOneHoliday( const uint8_t item,
                            const uint8_t month,
                            const uint8_t day,
                            const uint8_t plan);
        bool setOneHoliday( const uint8_t item,
                            const TYPEHOLIDAY prm );
        bool clearHoliday( void );
        bool clearPlans( void );

        bool setOneWeek( const uint8_t day, const uint8_t plan );
        bool setOneWeek(const uint8_t day, const TYPEWEEK *prm );

        //Getters
        bool getSetting (uint8_t* const SampleNum,
                         uint8_t* const TeachingPeriods,
                         uint8_t* const bSpeedUp,
                         uint8_t* const SpeedUpTime,
                         uint8_t* const Direction,
                         uint8_t* const mode_manual );
        bool getSetting ( TYPESETTING *const prm );


        bool getOnePlanDay( const uint8_t num_plan,
                            const uint8_t item,
                            uint8_t *const hour,
                            uint8_t *const min,
                            uint8_t *const bright_rg,
                            uint8_t *const volume,
                            uint8_t *const sensor );
        bool getOnePlanDay( const uint8_t num_plan,
                            const uint8_t item,
                            TYPEPLANITEMS *const prm );

        bool getOneHoliday( const uint8_t item,
                            uint8_t *const month,
                            uint8_t *const day,
                            uint8_t *const plan);
        bool getOneHoliday( const uint8_t item,
                            TYPEHOLIDAY *const prm );

        bool getOneWeek(const uint8_t day,
                         uint8_t *const plan );
        bool getOneWeek(const uint8_t day, TYPEWEEK *const prm );

        uint32_t countItemHoliday();
        uint32_t countItemPlan(const uint8_t plan_number);
        uint32_t countPlan();

        friend QDataStream &operator <<( QDataStream &stream,const AccessData &pthis );
        friend QDataStream &operator >>( QDataStream &stream, AccessData &pthis );
        friend QDebug operator<<( QDebug d, const AccessData &pthis );
    };
    /**
     * @brief ACCESS_DATE::AccessData::operator <<
     * @param stream
     * @param pthis
     * @return
     */
    inline QDataStream &operator <<(QDataStream &stream, const AccessData &pthis)
    {
        stream<<pthis.getDataStructure();
        return stream;
    }
    /**
     * @brief ACCESS_DATE::AccessData::operator <<
     * @param d
     * @param pthis
     * @return
     */
    inline QDebug operator<<(QDebug deb, const AccessData &pthis)
    {
        deb<<QString(pthis.getDataStructure());
        return deb;
    }
    /**
     * @brief ACCESS_DATE::AccessData::operator >>
     * @param stream
     * @param pthis
     * @return
     */
    inline QDataStream &operator >>(QDataStream &stream, AccessData &pthis)
    {

        QByteArray data;
        stream>>data;
        pthis.setDateStructure(data);
        return stream;
    }

}

#endif // CREATEDAYPLAN_H
