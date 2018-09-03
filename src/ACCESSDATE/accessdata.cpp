#include <cstring>
#include "accessdata.h"

using namespace ACCESS_DATE;


AccessData::AccessData():pDataProject(new TYPEPROJECT)
{
    // default configure
    setDefault(pDataProject);
}

AccessData::~AccessData()
{
    delete pDataProject;
}
/**
 * @brief AccessData::setDefault
 * @param pPrg
 */
void AccessData::setDefault(TYPEPROJECT *const pPrg)
{
    // clear
    clearProject();
    // set default data
    // setting
    pPrg->setting.bSpeedUp = 0;
    pPrg->setting.TeachingPeriods = 2;
    pPrg->setting.bSpeedUp = true;
    pPrg->setting.SpeedUpTime = 10;
    pPrg->setting.Direction = 1;
    pPrg->setting.mode_manual = 0;
}
/**
 * @brief AccessData::clearProject
 */
void AccessData::clearProject()
{
    memset(pDataProject,0,sizeof(TYPEPROJECT));
    clearHoliday();
    clearPlans();
}

/**
*/
void AccessData::setSetting (   const uint8_t SampleNum,
                                const uint8_t TeachingPeriods,
                                const uint8_t bSpeedUp,
                                const uint8_t SpeedUpTime,
                                const uint8_t Direction,
                                const uint8_t mode_manual)

{
    pDataProject->setting.SampleNum = SampleNum;
    pDataProject->setting.TeachingPeriods = TeachingPeriods;
    pDataProject->setting.bSpeedUp =  bSpeedUp;
    pDataProject->setting.SpeedUpTime = SpeedUpTime;
    pDataProject->setting.Direction = Direction;
    pDataProject->setting.mode_manual = mode_manual;
}
/**
*/
void AccessData::setSetting ( const TYPESETTING prm )
{
    TYPESETTING *const pSett = &pDataProject->setting;
    memcpy(pSett,&prm,sizeof(TYPESETTING));
}

/**
*/
bool AccessData::setOnePlanDay( const uint8_t num_plan,
                                const uint8_t item,
                                const uint8_t hour,
                                const uint8_t min,
                                const uint8_t bright_rg,
                                const uint8_t volume,
                                const uint8_t sensor )
{
    if ( (num_plan<getMaxPlans())&&(item<getMaxItemDday()) )
    {
        pDataProject->day_plans[num_plan].Items[item].hour = hour;
        pDataProject->day_plans[num_plan].Items[item].min = min;
        pDataProject->day_plans[num_plan].Items[item].BrightRG = bright_rg;
        pDataProject->day_plans[num_plan].Items[item].Volume = volume;
        pDataProject->day_plans[num_plan].Items[item].bSensor = sensor;
        return true;
    }
    return false;
}
/**
*/
bool AccessData::setOnePlanDay( const uint8_t num_plan,
                                const uint8_t item,
                                const TYPEPLANITEMS prm )
{
    if ( (num_plan<getMaxPlans())&&(item<getMaxItemDday()) )
    {
        TYPEPLANITEMS *const pPrm = &pDataProject->day_plans[num_plan].Items[item];
        memcpy(pPrm,&prm,sizeof(TYPEPLANITEMS));

        return true;
    }
    return false;
}
/**
 * @brief AccessData::setActiveOnePlanDay
 * @param num_plan
 * @return
 */
bool AccessData::setActiveOnePlanDay(const uint8_t num_plan)
{
    if ( (num_plan<getMaxPlans()) )
    {
        pDataProject->day_plans[num_plan].ActiveItems = static_cast<uint8_t>(countItemPlan(num_plan));

        return true;
    }

    return false;
}
/**
*/
bool AccessData::setOneHoliday( const uint8_t item,
                                const uint8_t month,
                                const uint8_t day,
                                const uint8_t plan)
{
    if ( item<getMaxHoliday() )
    {
        pDataProject->holidays[item].month = month;
        pDataProject->holidays[item].day = day;
        pDataProject->holidays[item].plan = plan;
    }
    return false;
}
/**
*/
bool AccessData::setOneHoliday( const uint8_t item,
                                const TYPEHOLIDAY prm )
{
    if ( item<getMaxHoliday() )
    {
        TYPEHOLIDAY *const pPrm = &pDataProject->holidays[item];
        memcpy(pPrm,&prm,sizeof(TYPEHOLIDAY));
        return true;
    }
    return false;
}
/**
 * @brief AccessData::clearHoliday
 * @return
 */
bool AccessData::clearHoliday()
{
    for ( uint32_t i=0; i<getMaxHoliday(); i++ )
    {
        TYPEHOLIDAY *const pPrm = &pDataProject->holidays[i];
        pPrm->month = -1;
        pPrm->day   = -1;
        pPrm->plan  = 0;
    }
    return true;
}
/**
 * @brief AccessData::clearPlans
 * @return
 */
bool AccessData::clearPlans()
{
    for ( uint32_t i=0; i<getMaxPlans(); i++ )
    {
        for ( uint32_t j=0; j<getMaxItemDday(); j++ )
        {
            TYPEPLANITEMS *const pday = &pDataProject->day_plans[i].Items[j];
            pday->hour      = -1;
            pday->min       = -1;
            pday->BrightRG  = 0xFF;
            pday->Volume    = 0xFF;
            pday->bSensor   = 0;
        }

        pDataProject->day_plans[i].ActiveItems = 0;
    }
    return true;
}
/**
*/
bool AccessData::setOneWeek( const uint8_t day, const uint8_t plan )
{
    if ( day<MAX_DAY_WEEK )
    {
        pDataProject->week[day].plan = plan;
        return true;
    }
    return false;
}
/**
*/
bool AccessData::setOneWeek( const uint8_t day, const TYPEWEEK *prm )
{
    if ( day<MAX_DAY_WEEK )
    {
        TYPEWEEK *const pPrm = &pDataProject->week[day];
        memcpy(pPrm,prm,sizeof(TYPEWEEK));
        return true;
    }
    return false;
}

/**
*/
bool AccessData::getSetting (   uint8_t* const SampleNum,
                                uint8_t* const TeachingPeriods,
                                uint8_t* const bSpeedUp,
                                uint8_t* const SpeedUpTime,
                                uint8_t* const Direction,
                                uint8_t* const mode_manual )
{

    *SampleNum = pDataProject->setting.SampleNum;
    *TeachingPeriods = pDataProject->setting.TeachingPeriods;
    *bSpeedUp = pDataProject->setting.bSpeedUp;
    *SpeedUpTime = pDataProject->setting.SpeedUpTime;
    *Direction = pDataProject->setting.Direction;
    *mode_manual = pDataProject->setting.mode_manual;

    return true;
}
/**
*/
bool AccessData::getSetting ( TYPESETTING *const prm )
{
    prm->SampleNum = pDataProject->setting.SampleNum;
    prm->TeachingPeriods = pDataProject->setting.TeachingPeriods;
    prm->SpeedUpTime = pDataProject->setting.SpeedUpTime;
    prm->bSpeedUp = pDataProject->setting.bSpeedUp;
    prm->Direction = pDataProject->setting.Direction;
    prm->mode_manual = pDataProject->setting.mode_manual;
    return true;
}

/**
*/
bool AccessData::getOnePlanDay( const uint8_t num_plan,
                                const uint8_t item,
                                uint8_t *const hour,
                                uint8_t *const min,
                                uint8_t *const bright_rg,
                                uint8_t *const volume,
                                uint8_t *const sensor)
{
    if ( (num_plan<getMaxPlans())&&(item<getMaxItemDday()) )
    {
        *hour = pDataProject->day_plans[num_plan].Items[item].hour;
        *min = pDataProject->day_plans[num_plan].Items[item].min;
        *bright_rg = pDataProject->day_plans[num_plan].Items[item].BrightRG;
        *volume = pDataProject->day_plans[num_plan].Items[item].Volume;
        *sensor = pDataProject->day_plans[num_plan].Items[item].bSensor;

        return true;
    }
    return false;
}
/**
*/
bool AccessData::getOnePlanDay( const uint8_t num_plan,
                                const uint8_t item,
                                TYPEPLANITEMS *const prm )
{
    if ( (num_plan<getMaxPlans())&&(item<getMaxItemDday()) )
    {
        prm->hour = pDataProject->day_plans[num_plan].Items[item].hour;
        prm->min = pDataProject->day_plans[num_plan].Items[item].min;
        prm->Volume = pDataProject->day_plans[num_plan].Items[item].Volume;
        prm->BrightRG = pDataProject->day_plans[num_plan].Items[item].BrightRG;
        prm->bSensor = pDataProject->day_plans[num_plan].Items[item].bSensor;

        return true;
    }
    return false;
}
/**
*/
bool AccessData::getOneHoliday( const uint8_t item,
                                uint8_t *const month,
                                uint8_t *const day,
                                uint8_t *const plan)
{
    if (item<getMaxHoliday())
    {
        *month = pDataProject->holidays[item].month;
        *day = pDataProject->holidays[item].day;
        *plan = pDataProject->holidays[item].plan;

        return true;
    }
    return false;
}
/**
*/
bool AccessData::getOneHoliday( const uint8_t item,
                                TYPEHOLIDAY *const prm )
{
    if (item<getMaxHoliday())
    {
        prm->month = pDataProject->holidays[item].month;
        prm->day = pDataProject->holidays[item].day;
        prm->plan = pDataProject->holidays[item].plan;

        return true;
    }
    return false;
}
/**
*/
bool AccessData::getOneWeek( const uint8_t day,
                             uint8_t *const plan )
{
    if ( day<MAX_DAY_WEEK )
    {
        *plan = pDataProject->week[day].plan;

        return true;
    }
    return false;
}
/**
*/
bool AccessData::getOneWeek( const uint8_t day, TYPEWEEK *const prm )
{
    if ( day<MAX_DAY_WEEK )
    {
        prm->plan = pDataProject->week[day].plan;

        return true;
    }
    return false;
}

/**
 * @brief AccessData::countItemHoliday
 * @return
 */
uint32_t AccessData::countItemHoliday()
{
    uint32_t count = 0;
    for ( uint32_t i=0; i<getMaxHoliday(); i++ )
    {
        TYPEHOLIDAY *const pPrm = &pDataProject->holidays[i];
        if ( (pPrm->month<1)||(pPrm->month>12) ||
             (pPrm->day<1)||(pPrm->day>31)
           )
        {
            break;
        }
        count++;
    }
    return count;
}
/**
 * @brief AccessData::countItemPlan
 * @param plan_number
 * @return
 */
uint32_t AccessData::countItemPlan(const uint8_t plan_number)
{
    uint32_t count = 0;

    if ( plan_number < getMaxPlans())
    {
        for ( uint32_t i=0; i<getMaxItemDday(); i++ )
        {
            TYPEPLANITEMS *const pPrm = &pDataProject->day_plans[plan_number].Items[i];

            if ( (pPrm->hour>24)
                 || (pPrm->min>60)
                 || (pPrm->Volume>100)
               )
            {
                break;
            }
            count++;
        }
    }
    return count;
}
/**
 * @brief AccessData::countPlan
 * @return
 */
uint32_t AccessData::countPlan()
{
    uint32_t count = 0;

    for ( uint32_t i=0; i<getMaxPlans(); i++ )
    {
        const uint32_t item = countItemPlan(i);

        if (!item)
        {
            break;
        }
        count++;
    }

    return count;
}
