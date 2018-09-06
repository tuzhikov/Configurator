#include <time.h>
#include "controller.h"
#include "crc.h"

//#define DEBUG_ANSWER

/**
 * @brief Controller::commandWriteAll
 */
void Controller::commandWriteAll()
{
    QQueue<QByteArray> cmd;

    commandSetTime(cmd);
    commandSetSetting(cmd);
    commandSetHolidays(cmd);
    commandSetWeek(cmd);
    commandSetPlans(cmd);

    sendMessage(cmd);
    qDebug()<<"command Write All";
}

/**
 * @brief Controller::commandRead wav
 */
void Controller::commandReadAll()
{
    QQueue<QByteArray> cmd;

    commandGetTime(cmd);
    commandGetSetting(cmd);
    commandGetHolidays(cmd);
    commandGetWeek(cmd);
    commandGetPlans(cmd);

    sendMessage(cmd);
    qDebug()<<"command Read All";
}

void Controller::commandForRead(QQueue<QByteArray>& queue, TYPE_NUMBER_COMMAND subopcode, uint8_t index)
{
    QByteArray cmd;
    cmd.append(static_cast<char>(6));           // lengh low
    cmd.append(static_cast<char>(0));           // lengh hi
    cmd.append(TYPE_TL);                        // type
    cmd.append(CMD_TR_READ);                    // cmd
    cmd.append(static_cast<char>(subopcode));   // type cmd
    cmd.append(static_cast<char>(index));
    collectTransportLevel(cmd);
    queue.enqueue(cmd);
}

/**
 * @brief Controller::commandGetTime
 */
void Controller::commandGetTime(QQueue<QByteArray>& queue)
{
    commandForRead(queue, CMD_OPCODE_TIME);
    qDebug()<<"command Get Time";
}

/**
 * @brief Controller::commandSetTime
 */
void Controller::commandSetTime(QQueue<QByteArray>& queue)
{
    QByteArray cmd;

    cmd.append(sizeof(timeDevice)+6);// lengh time +crc(2) +cmd(4)
    cmd.append(static_cast<char>(0));
    cmd.append(TYPE_TL);        // type
    cmd.append(CMD_TR_WRITE);   // cmd
    cmd.append(CMD_OPCODE_TIME); // type cmd
    cmd.append(static_cast<char>(0));
    //fill in the data
    cmd.append((const char*)&timeDevice,sizeof(timeDevice)); //data
    collectTransportLevel(cmd);
    queue.enqueue(cmd);
    qDebug()<<"command Set Data: "<<timeDevice.year<<"."<<timeDevice.month<<"."<<timeDevice.day
           <<"Time: "<<timeDevice.hour<<":"<<timeDevice.min<<":"<<timeDevice.sec;
}

/**
 * @brief Controller::commandGetPlans
 */
void Controller::commandGetPlans(QQueue<QByteArray>& queue)
{

    const uint8_t max_plan = static_cast<uint8_t>(retDataProject().retDataProject().getMaxPlans());

    for (uint8_t i=0; i<max_plan;i++)
    {
        commandForRead(queue, CMD_OPCODE_DAY_PLANS,i);
    }
    qDebug()<<"command Get Plans";
}

/**
 * @brief Controller::commandSetPlans
 */
void Controller::commandSetPlans(QQueue<QByteArray>& queue)
{
    const uint8_t max_plan = static_cast<uint8_t>(retDataProject().retDataProject().getMaxPlans());

    for (uint8_t i=0; i<max_plan; i++)
    {
        QByteArray cmd;
        cmd.append(static_cast<char>(dataproject.retDataProject().getLengthOneDatePlan())+6);// lengh +crc(2) +cmd(4)
        cmd.append(static_cast<char>(0));
        cmd.append(TYPE_TL);        // type
        cmd.append(CMD_TR_WRITE);   // cmd
        cmd.append(CMD_OPCODE_DAY_PLANS); // type cmd
        cmd.append(static_cast<char>(0));
        //fill in the data
        cmd.append(dataproject.retDataProject().getDataOnePlan(0));
        collectTransportLevel(cmd);
        queue.enqueue(cmd);
    }

    qDebug()<<"command Set Plans";
}

/**
 * @brief Controller::commandGetWeek
 */
void Controller::commandGetWeek(QQueue<QByteArray>& queue)
{
    commandForRead(queue, CMD_OPCODE_WEEK);
    qDebug()<<"command Get Week";
}

/**
 * @brief Controller::commandSetWeek
 */
void Controller::commandSetWeek(QQueue<QByteArray>& queue)
{
    QByteArray cmd;
    cmd.append(static_cast<char>(dataproject.retDataProject().getLengthDateWeek())+6);// lengh +crc(2) +cmd(4)
    cmd.append(static_cast<char>(0));
    cmd.append(TYPE_TL);        // type
    cmd.append(CMD_TR_WRITE);   // cmd
    cmd.append(CMD_OPCODE_WEEK); // type cmd
    cmd.append(static_cast<char>(0));
    //fill in the data
    cmd.append(dataproject.retDataProject().getDataWeek());
    collectTransportLevel(cmd);
    queue.enqueue(cmd);

    qDebug()<<"command Set Week:";
}

/**
 * @brief Controller::commandGetHolidays
 */
void Controller::commandGetHolidays(QQueue<QByteArray>& queue)
{
    commandForRead(queue, CMD_OPCODE_HOLIDAYS);
    qDebug()<<"command Get Holidays";
}

/**
 * @brief Controller::commandSetHolidays
 */
void Controller::commandSetHolidays(QQueue<QByteArray>& queue)
{
    QByteArray cmd;
    cmd.append(static_cast<char>(dataproject.retDataProject().getLengthDateHoliday())+6);// lengh time +crc(2) +cmd(4)
    cmd.append(static_cast<char>(0));
    cmd.append(TYPE_TL);        // type
    cmd.append(CMD_TR_WRITE);   // cmd
    cmd.append(CMD_OPCODE_HOLIDAYS); // type cmd
    cmd.append(static_cast<char>(0));
    //fill in the data
    cmd.append(dataproject.retDataProject().getDataHoliDay());
    collectTransportLevel(cmd);
    queue.enqueue(cmd);
    qDebug()<<"command Set Holiday";
}

/**
 * @brief Controller::commandGetSetting
 */
void Controller::commandGetSetting(QQueue<QByteArray>& queue)
{
    commandForRead(queue, CMD_OPCODE_SETTINGS);
    qDebug()<<"command Get Setting";
}

/**
 * @brief Controller::commandSetSetting
 */
void Controller::commandSetSetting(QQueue<QByteArray>& queue)
{
    QByteArray cmd;
    cmd.append(static_cast<char>(dataproject.retDataProject().getLengthDateSetting())+6);// lengh time +crc(2) +cmd(4)
    cmd.append(static_cast<char>(0));
    cmd.append(TYPE_TL);        // type
    cmd.append(CMD_TR_WRITE);   // cmd
    cmd.append(CMD_OPCODE_SETTINGS); // type cmd
    cmd.append(static_cast<char>(0));
    //fill in the data
    cmd.append(dataproject.retDataProject().getDataSetting());
    collectTransportLevel(cmd);
    queue.enqueue(cmd);
    qDebug()<<"command Set setting";
}

/**
 * @brief Controller::commandGetStatus
 */
void Controller::commandGetStatus()
{
    QQueue<QByteArray> queue;
    commandForRead(queue, CMD_OPCODE_STATUS);
    sendMessage(queue);
    qDebug()<<"command commandGetStatus";
}

/**
 * @brief Controller::answerSetTime
 * @return
 */
RET_ANSWER Controller::answerSetTime(const char *, const int){ return SUCCESSFULLY; }

RET_ANSWER Controller::answerGetTime(const char* pDate, const int lengh)
{
    TYPETIME time = {};
    if ( sizeof(time) == lengh)
    {
        memcpy(&time,pDate,static_cast<uint32_t>(lengh));
        emit signalTime(time);
        return SUCCESSFULLY;
    }
    setMessageError(tr("<CENTER><b>Data Time is incorrect!</CENTER></b>"));
    return ERROR_ANSWER;
}

RET_ANSWER Controller::answerSetPlans(const char *, const int){ return SUCCESSFULLY; }

RET_ANSWER Controller::answerGetPlans(const char* p, const int length)
{
    const int cound_data = static_cast<int>(dataproject.retDataProject().getLengthOneDatePlan());
    const uint8_t index = static_cast<uint8_t>(*p);

    if (cound_data == length)
    {
        QByteArray date( (p), length );
        dataproject.retDataProject().setDateOnePlan(index, date);
        return SUCCESSFULLY;
    }
    setMessageError(tr("<CENTER><b>Data Plan is incorrect!</CENTER></b>"));
    return ERROR_ANSWER;
}

RET_ANSWER Controller::answerSetWeek(const char *, const int) { return SUCCESSFULLY; }

RET_ANSWER Controller::answerGetWeek(const char* p, const int length)
{
    const int cound_data = static_cast<int>(dataproject.retDataProject().getLengthDateWeek());

    if (cound_data == (length-1))
    {
           QByteArray date( p, length );
           dataproject.retDataProject().setDateWeek(date);
           return SUCCESSFULLY;
    }

    setMessageError(tr("<CENTER><b>Data week is incorrect!</CENTER></b>"));
    return ERROR_ANSWER;
}

RET_ANSWER Controller::answerSetHolidays(const char *, const int){ return SUCCESSFULLY; }

RET_ANSWER Controller::answerGetHolidays(const char* p, const int length)
{
    const int cound_data = static_cast<int>(dataproject.retDataProject().getLengthDateHoliday());

    if (cound_data == length)
    {
           QByteArray date( p, length );
           dataproject.retDataProject().setDateHoliDay(date);
           return SUCCESSFULLY;
    }

    setMessageError(tr("<CENTER><b>Data holidays is incorrect!</CENTER></b>"));
    return ERROR_ANSWER;
}

RET_ANSWER Controller::answerSetSetting(const char *, const int){ return SUCCESSFULLY; }

RET_ANSWER Controller::answerGetSetting(const char* p, const int length)
{
    const int cound_data = static_cast<int>(dataproject.retDataProject().getLengthDateSetting());

    if (cound_data == length)
    {
           QByteArray date( p, length );
           dataproject.retDataProject().setDateSetting(date);
           return SUCCESSFULLY;
    }

    setMessageError(tr("<CENTER><b>Data setting is incorrect!</CENTER></b>"));
    return ERROR_ANSWER;
}

RET_ANSWER Controller::answerGetStatus(const char* pDate, const int lengh)
{
    TYPE_STATUS status = {};
    const int lengh_test(sizeof(status));

    if (lengh_test==lengh)
    {
        memcpy(&status,pDate,sizeof(status));
        emit signalDataStatus(status);
        return SUCCESSFULLY;
    }

    setMessageError(tr("<CENTER><b>Data Controller::answerGetStatus is incorrect!</CENTER></b>"));
    // stop timer diagnosis
    emit signalTimerDiagnosisDisabled();
    return ERROR_ANSWER;
}

/**
 * @brief Controller::collectTransportLevel
 * @param cmd
 * @return
 */
void Controller::collectTransportLevel(QQueue<QByteArray> &list)
{
    for (int i=0; i<list.count(); i++)
    {
        collectTransportLevel(list[i]);
    }
}
/**
 * @brief collectTransportLevel
 * @param cmd
 * @return
 */
void Controller::collectTransportLevel( QByteArray &cmd )
{
    cmd.prepend(header.pHeader2,header.lengn2);
    quint16 crc16 = CRC::Bit16( cmd );
    cmd.append((const char*)&crc16,sizeof(crc16));
}
/**
 * @brief Controller::checksumReceived
 * @param cmd
 * @return
 */
RET_ANSWER Controller::checksumReceived(const QByteArray &cmd)
{
    RET_ANSWER result = ERROR_ANSWER;

    if ( cmd.size()<10 )
    {
        return DELAY_ANSWER;
    }

    const int lehgn = cmd.length();
    const char *pLengh_p(cmd.data()+3); // 4 byte
    const int max_date(*pLengh_p+5);

    if ( max_date>lehgn )// error packet length
    {
        qDebug()<<"Length error of the received packet: protocol.cpp";
        return DELAY_ANSWER;
    }
    // Prune the tail
    const QByteArray cmdl(cmd.left(max_date));

    if ( !CRC::Bit16(cmdl) ) // CRC ok
    {
        const quint8 answer = static_cast<const quint8>(cmdl.at(8)); //
        if ( answer != 0xFF )
        {
            const TYPE_NUMBER_COMMAND_TRANSPORT direct = static_cast<const TYPE_NUMBER_COMMAND_TRANSPORT>(cmdl.at(6));
            const TYPE_NUMBER_COMMAND opcode = static_cast<const TYPE_NUMBER_COMMAND>(cmdl.at(7));
            const char *start_data(cmdl.data()+9);   // begin pointer date without a reply byte
            const int lengh(cmdl.count()-11);    // lehgn date without a reply byte

            return answerRun(direct, opcode, start_data, lengh);
        }
        else // [answer][command][coderror]
        {
            const QString message(
                        QString("<CENTER><b>Code error = %1</CENTER></b>").arg(
                            QString::number(cmdl.at(9))));
            setMessageError(message);
        }
    }
    else
    {
        setMessageError(tr("<CENTER><b>Error CRC16!</CENTER></b>"));
    }

    return result;
}
/**
 * @brief Controller::parserReceivedPacket
 * @return
 */
RET_ANSWER Controller::parserReceivedPacket(const QByteArray &cmd)
{
    static QByteArray readBuffer;
    static TYPE_STEP step = STEP_ONE;
    static int counter = 0;
    const int max_repeat = (500/currentInterface->retTimeDelay());
    RET_ANSWER result = ERROR_ANSWER;
    const QByteArray HEAD(header.pHeader2,header.lengn2);

    switch(step)
    {
        case STEP_ONE:
            counter = 0;
            readBuffer.clear();
            step = STEP_TWO;
        case STEP_TWO:
        {
            //time is over
            if ( ++counter>max_repeat )
            {
                step = STEP_ERROR;
            }
            else
            {
                readBuffer.append(cmd);
                const int index = readBuffer.indexOf(HEAD);
                if ( index !=-1 )
                {
                    result = checksumReceived(readBuffer.remove(0,index));
                    step = (result!=DELAY_ANSWER)?STEP_ONE:STEP_TWO;
                    return result;
                }
            }
        }
        return DELAY_ANSWER;

        case STEP_ERROR:
        default:
        {
            step = STEP_ONE;
        }
        return NO_ANSWER;
    }
}
/**
 * @brief Controller::sendOutputMessage
 */
void Controller::sendOutputMessage()
{
    if (!messageError.isEmpty())
    {
        emit signalMessageError(messageError);
        messageError.clear();
    }
    else if (!messageOk.isEmpty())
    {
        emit signalMessageOk(messageOk);
        messageOk.clear();
    }
}
/**
 * @brief Controller::nextAnswerWavTest
 * @return
 */
QByteArray Controller::nextAnswerWavTest()
{
    QByteArray retData;
#ifdef DEBUG_ANSWER
        //const uint8_t BuffAnswerCmd[] ={
        //0xAA,0xAA,0xAA,0xAA,0xAA,0x55,0xC3,0x5A,0x05,0x00,0x10,0x01,0x00,0x03,0x00,0x37,0x2F
        //
        //0xAA,0xAA,0xAA,0xAA,0xAA,0x55,0xC3,0x5A,0x05,0x00,0x10,0x01,0x00,0x00,0x00,0x37,0xDF
        //test
        //0xAA,0xAA,0xAA,0xAA,0xAA,0x55,0xC3,0x5A,0x1B,0x00,0x10,0x02,0x00,0x00,0x2D,0x5E,0x42,0x59,0x00,
        //0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x1E,0x1E,0x00,0x00,0x40,0xAC
//        const uint8_t BuffFistWav[] ={
//                // read
//                0xAA,0xAA,0xAA,0xAA,0xAA,0x55,0xC3,0x5A,0x73,0x00,0x10,0x02,0x00,0x02,0x32,0x30,0x30,0x30,0x5F,0x30,0x32,
//                0x30,0x30,0x5F,0xD0,0xA2,0xD0,0xB8,0xD0,0xBA,0x20,0xD1,0x82,0xD0,0xB0,0xD0,0xBA,0x20,0x31,0xD1,0x81,0x20,
//                0x32,0xD1,0x82,0xD0,0xB8,0xD0,0xBA,0xD0,0xB0,0x2E,0x77,0x61,0x76,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x02,
//                0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x06,0x01,0x00,0x00,0x03,0x00,0x00,0x00,0x52,0x49,0x46,0x46,0x24,0xAB,
//                0x00,0x00,0x57,0x41,0x56,0x45,0x66,0x6D,0x74,0x20,0x10,0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x22,0x56,0x00,
//                0x00,0x44,0xAC,0x00,0x00,0x02,0x00,0x10,0x00,0x64,0x61,0x74,0x61,0x00,0xAB,0x00,0x00,0x16,0x00,0x02,0x53,0xA4
        // };
        //retData.append((char*)&BuffAnswerCmd,sizeof(BuffAnswerCmd));
    // extract sound files

    static QQueue<QByteArray> list;
    static TYPE_STEP step = STEP_ONE;
switch(step)
{
    case STEP_ONE:
    {
        list.clear();
        QMap <QString, QByteArray>::iterator it = dataproject.retPlayList().begin();
        quint8 count_sound = dataproject.retPlayList().size();
        bool first_package = true;
        uint16_t count_command = 0;

        for (;it!= dataproject.retPlayList().end(); ++it)
        {
            QByteArray name;name.append(it.key());
            QByteArray data(it.value());
            const quint16 max_pask = (data.size()/max_lengh_data_wav) +1;
            //first wav
            count_command++;
            QByteArray cmd;
            const quint16 lengh = max_lengh_name_file+max_lengh_hander_wav+2+1+4;
            cmd.append((char*)&lengh,sizeof(quint16));
            cmd.append(TYPE_UZTVOP);    // type
            cmd.append(CMD_TR_READ);   // cmd
            cmd.append((char)0);
            cmd.append(CMD_WAV);        // type cmd
            cmd.append(name.data(),max_lengh_name_file); // name
            cmd.append(data.data(),max_lengh_hander_wav); //
            data.remove(0, max_lengh_hander_wav);
            cmd.append((char*)&max_pask, 2);
            count_sound--;
            if ( first_package )
            {
                cmd.append( COD_CLEAR_WAV );
                first_package = false;
            }
            else
            {
                cmd.append( count_sound );
            }
            list.enqueue(cmd);
            // data wav
            for (int i = max_pask;--i>=0;)
            {
                QByteArray cmd;
                const quint16 len = max_lengh_data_wav+6; // CRC16(2) + CMD_WAV(1)+index_psk(2)
                cmd.append((char*)&len,sizeof(quint16));
                cmd.append(TYPE_UZTVOP);    // type
                cmd.append(CMD_TR_READ);   // cmd
                cmd.append((char)0);
                cmd.append(CMD_WAV);        // type cmd

                const int lengh(data.size());
                const char *const pBegin(data.data());
                if ( lengh<max_lengh_data_wav )
                {
                    cmd.append(pBegin,lengh);
                    const int lengh_null(max_lengh_data_wav-lengh);
                    cmd.append(lengh_null, (char)0);
                }
                else
                {
                    cmd.append(pBegin, max_lengh_data_wav);
                    data.remove(0, max_lengh_data_wav);
                }
                // add number paskage
                cmd.append((char*)&i,sizeof(quint16));
                list.enqueue(cmd);
            }
        }
        // add command OK
        while ( count_command++<codeNameFile.count() )
        {
            QByteArray cmd;
            cmd.append((uint8_t)5);
            cmd.append((char)0);
            cmd.append(TYPE_UZTVOP);
            cmd.append(CMD_TR_READ);
            cmd.append((char)0);
            cmd.append(CMD_WAV);
            cmd.append((char)0);
            list.enqueue(cmd);
        }

        collectTransportLevel(list);

        step = STEP_TWO;
    }
    case STEP_TWO:

        if (!list.isEmpty())
        {
            retData.append(list.dequeue());
        }
        else
        {
            step = STEP_ONE;
            retData.clear();
        }
    break;

    default:
        step = STEP_ONE;
        retData.clear();
    break;
}

#endif
    return retData;
}
/**
 * @brief Controller::Controller
 * @param parent
 */
Controller::Controller( QObject *parent ) :
                    QObject(parent),
                    timerRead(new QTimer(parent))
{
    connect(this,SIGNAL(signalStart()),this,SIGNAL(signalStep()),Qt::DirectConnection);
    connect(this,SIGNAL(signalStep()),this,SLOT(on_Machine()),Qt::DirectConnection);
    connect(timerRead,SIGNAL(timeout()),this,SLOT(on_Machine()),Qt::DirectConnection);

    currentInterface = new ComPort();
}
/**
 * @brief Controller::on_Machine
 */
void Controller::on_Machine()
{

    int progress = 0;

    switch(stat)
    {
        case stOpen:
            if(!listCMD.isEmpty())
            {
                if(currentInterface->open())
                {
                    stat=stCounter;
                    disconnect(this,SIGNAL(signalStart()),this,SIGNAL(signalStep()));
                    timerRead->setInterval(currentInterface->retTimeDelay());
                    templistCMD = listCMD;
                    currentValueProgress = 0;
                    const int valueMax = 3*templistCMD.count();
                    //const int valueMax(20*3);
                    emit signalProgressRange(0,valueMax);
                    emit signalProgressVisible(progressVisible);
                }else{
                    stat = stStopExchange;
                    setMessageError(tr("<CENTER><b>Interface open ERROR!</CENTER></b>"));
                }

                listCMD.clear();
                emit signalStep();
            }
        return;

        case stCounter:
            if ( !listCMD.isEmpty() )
            {
                templistCMD = listCMD;
                listCMD.clear();
            }
            if( !templistCMD.isEmpty() )
            {
                emit signalProgressValue(++currentValueProgress);
                sendCmd.clear();
                sendCmd.append(templistCMD.dequeue());
                countSend = 0;
                stat=stWrite;
            }else{
                stat=stClose;
            }
            emit signalStep();
        return;

        case stRepeat:
            if (++countSend<max_repeat)
            {
                stat = stWrite;
            }
            else
            {
                templistCMD.clear();
                stat = stStopExchange;
                qDebug()<<"Error response--------------------------------------------------------------------------";
                setMessageError(tr("<CENTER><b>Device does not answer!</CENTER></b>"));
            }
            emit signalStep();
        return;

        case stWrite:
        {
            currentInterface->sendDate(sendCmd);
            emit signalSendMessage(sendCmd,Qt::green);
            emit signalProgressValue(++currentValueProgress);
            qDebug()<<"Send"<<sendCmd;
            stat=stRead;
            timerRead->start();
        }
        return;

        case stRead:
        {
            QByteArray resBuff;
#ifdef DEBUG_ANSWER
            resBuff.append(nextAnswerWavTest());
#else
            currentInterface->readDate(resBuff);
#endif
            emit signalSendMessage(resBuff,Qt::darkYellow);
            qDebug()<<"Read"<<resBuff;

            const RET_ANSWER answer = parserReceivedPacket(resBuff);

            if ( answer == DELAY_ANSWER) // next package
            {
                return;
            }

            timerRead->stop();
            if ( answer != SUCCESSFULLY )
            {
                progress = currentValueProgress;
                stat = stRepeat;
            }
            else
            {
                progress = ++currentValueProgress;
                stat = stCounter;
            }
            emit signalProgressValue(progress);
            emit signalStep();
        }
        return;

        case stStopExchange:
            emit signalTimerDiagnosisDisabled();
        case stClose:
            currentInterface->close();
        default:
            stat = stOpen;
            timerRead->stop();
            connect(this,SIGNAL(signalStart()),this,SIGNAL(signalStep()),Qt::DirectConnection);
            emit signalProgressValue(0);
            emit signalProgressVisible(false);
            emit signalStop();
            sendOutputMessage();
        return;
    }
}
//// test --------------------------------------------------------
//QByteArray cmd;
//QDateTime timeDevice(QDateTime::currentDateTime());
//const TYPETIME time{
//        .time = timeDevice.toTime_t(),
//        .gmt  = static_cast<uint8_t>(timeDevice.offsetFromUtc()/3600)
//};
//cmd.append(sizeof(time)+(uint8_t)4);// lengh time +crc +cmd
//cmd.append((char)0);
//cmd.append(TYPE_UZTVOP);    // type
//cmd.append(CMD_TR_READ);    // cmd
//cmd.append((char)0);        // answer
//cmd.append(CMD_TIME);       // type cmd
//cmd.append((const char*)&time,sizeof(time));
//collectTransportLevel(cmd);
//QByteArray cmd1(cmd.data(),3);cmd.remove(0,3);
//QByteArray cmd2(cmd.data(),7);cmd.remove(0,7);
//QByteArray cmd3(cmd.data(),3);cmd.remove(0,3);
//QByteArray cmd4(cmd);cmd.clear();
//QVector <QByteArray> list;
//list.append(cmd);
//list.append(cmd1);
//list.append(cmd);
//list.append(cmd2);
//list.append(cmd);
//list.append(cmd3);
//list.append(cmd);
//list.append(cmd4);
//QByteArray cmd_res(index<list.size()?list.at(index++):QByteArray());
//QByteArray resBuff;
//resBuff.append(cmd_res);
//              QByteArray cmd;
//              cmd.append((uint8_t)4);
//              cmd.append((char)0);
//              cmd.append(TYPE_UZTVOP);    // type
//              cmd.append(CMD_TR_WRITE);    // cmd
//              cmd.append((char)0);       // type cmd test
//              cmd.append(CMD_TIME);// type cmd test
//              collectTransportLevel(cmd);

//// test --------------------------------------------------------

/**
 * @brief Controller::sendMessage
 * @param ba
 */
void Controller::sendMessage(QQueue<QByteArray> &ba)
{
    listCMD.clear();
    listCMD = ba;
    emit signalStart();
}
/**
 * @brief Controller::sendMessage
 * @param ba
 */
void Controller::sendMessage(QByteArray &ba)
{
    listCMD.enqueue(ba);
    emit signalStart();
}
/**
 * @brief Controller::stopMessage
 */
void Controller::stopMessage()
{
    listCMD.clear();
    connect(this,SIGNAL(signalStart()),this,SIGNAL(signalStep()),Qt::DirectConnection);
}
/**
 * @brief Controller::commandRun
 * @param numberCmd
 */
void Controller::commandRun(const int numberCmd)
{
    if (numberCmd<TYPE_COMMAND::END_TYPE_COMMAND)
    {
        // disabled progressbar
        ///progressVisible = (numberCmd == cmReadStatus)?false:true;
        // send command
        (this->*command_send[numberCmd])();
    }
}

/**
 * @brief Controller::setParametrCOMPORT
 */
void Controller::setParametrCOMPORT(const QStringList &list)
{

    LinkInreface *const com(currentInterface);
    com->connections(list);
    qDebug()<<list;
}
/**
 * @brief Controller::setDateTime
 */
void Controller::setDateTime(const QDateTime &datatime)
{
    QDateTime dt(datatime);
    //dt.setTimeZone(QTimeZone::utc());
    //timeDevice.time = dt.toTime_t();
    timeDevice.year = static_cast<uint16_t>(dt.date().year());
    timeDevice.month = static_cast<uint8_t>(dt.date().month());
    timeDevice.day = static_cast<uint8_t>(dt.date().day());
    timeDevice.weekday = static_cast<uint8_t>(dt.date().dayOfWeek());

    timeDevice.hour = static_cast<uint8_t>(dt.time().hour());
    timeDevice.min = static_cast<uint8_t>(dt.time().minute());
    timeDevice.sec = static_cast<uint8_t>(dt.time().second());

    //qDebug()<<dt;
}
/**
 * @brief Controller::setDateTimeZone
 * @param num_zone
 */
void Controller::setDateTimeZone(const int )
{
    //const int indexUTC0 = 12;
    //timeDevice.gmt = num_zone - indexUTC0;
}
