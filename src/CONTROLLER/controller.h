#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QQueue>
#include <QByteArray>
#include <QDateTime>
#include <QColor>
#include <QTimeZone>
#include <QVector>
#include "cmd_device.h"
#include "src/ACCESSDATE/accessdataproject.h"
#include "src/INTERFACE/LinkInterface.h"
#include "src/INTERFACE/comport.h"
#include "src/INTERFACE/wifi.h"
#include "src/CONTROLLER/cmd_device.h"


class Controller : public QObject
{
    Q_OBJECT

public:
    //typedef QMap <QString, QByteArray> TYPE_PLAY;
    //typedef QMap <int,QByteArray> TYPE_CODE_NAME;

    enum TYPE_COMMAND{
                        cmWriteAll      = 0
                      , cmReadAll       = 1
                      , cmReadStatus    = 2
                      , END_TYPE_COMMAND
                     };
    explicit Controller( QObject *parent = nullptr );
    ~Controller(){}
    inline AccessDataProject &retDataProject(){ return dataproject;}
private:
    enum TYPE_STATE{stOpen,stCounter,stRepeat,stWrite,stRead,stClose,stStopExchange};
    enum TYPE_STEP{STEP_ONE,STEP_TWO,STEP_THREE,STEP_ERROR};
    enum TYPE_INTERFACE{INTERFACE_COM,END_INTERFACE};

    TYPE_STATE stat = stOpen;
    AccessDataProject dataproject;
    QQueue<QByteArray> listCMD, templistCMD;
    QByteArray sendCmd;
    QTimer *timerRead;
    LinkInreface* currentInterface = nullptr;
    int currentValueProgress = 0;
    TYPETIME timeDevice;
    int countSend = 0;
    const int max_repeat = 4;
    QString messageError,messageOk;
    bool progressVisible;

    // send command
    void commandForRead(TYPE_NUMBER_COMMAND subopcode, uint8_t index = 0);
    void commandWriteAll();
    void commandReadAll();
    void commandSetTime();
    void commandGetTime();
    void commandGetPlans();
    void commandSetPlans();
    void commandGetWeek();
    void commandSetWeek();
    void commandGetHolidays();
    void commandSetHolidays();
    void commandGetSetting();
    void commandSetSetting();
    void commandGetStatus();

    typedef void (Controller::*NUMBER_COMMAND)();
    const NUMBER_COMMAND command_send[END_TYPE_COMMAND] = {
        &Controller::commandWriteAll, &Controller::commandReadAll, &Controller::commandGetStatus};

    // answer command
    typedef RET_ANSWER (Controller::*NUMBER_COMMAND_READ)(const char *, const int);
    RET_ANSWER answerGetTime(const char *, const int);
    RET_ANSWER answerSetTime(const char *, const int);
    RET_ANSWER answerGetPlans(const char *, const int);
    RET_ANSWER answerSetPlans(const char *, const int);
    RET_ANSWER answerGetWeek(const char *, const int);
    RET_ANSWER answerSetWeek(const char *, const int);
    RET_ANSWER answerGetHolidays(const char *, const int);
    RET_ANSWER answerSetHolidays(const char *, const int);
    RET_ANSWER answerGetSetting(const char *, const int);
    RET_ANSWER answerSetSetting(const char *, const int);
    RET_ANSWER answerGetStatus(const char *, const int);

    struct AnswerCmd{
        NUMBER_COMMAND_READ run;
        TYPE_NUMBER_COMMAND opcode;
        TYPE_NUMBER_COMMAND_TRANSPORT direct;
    };

    const AnswerCmd answerData[CMD_MAX_OPCODE] = {
        {&Controller::answerGetTime,        CMD_OPCODE_TIME,        CMD_TR_READ}
        ,{&Controller::answerSetTime,       CMD_OPCODE_TIME,        CMD_TR_WRITE}
        ,{&Controller::answerGetPlans,      CMD_OPCODE_DAY_PLANS,   CMD_TR_READ}
        ,{&Controller::answerSetPlans,      CMD_OPCODE_DAY_PLANS,   CMD_TR_WRITE}
        ,{&Controller::answerGetWeek,       CMD_OPCODE_WEEK,        CMD_TR_READ}
        ,{&Controller::answerSetWeek,       CMD_OPCODE_WEEK,        CMD_TR_WRITE}
        ,{&Controller::answerGetHolidays,   CMD_OPCODE_HOLIDAYS,    CMD_TR_READ}
        ,{&Controller::answerSetHolidays,   CMD_OPCODE_HOLIDAYS,    CMD_TR_WRITE}
        ,{&Controller::answerGetSetting,    CMD_OPCODE_SETTINGS,    CMD_TR_READ}
        ,{&Controller::answerSetSetting,    CMD_OPCODE_SETTINGS,    CMD_TR_WRITE}
        ,{&Controller::answerGetStatus,     CMD_OPCODE_STATUS,      CMD_TR_READ}
    };

    RET_ANSWER answerRun(TYPE_NUMBER_COMMAND_TRANSPORT direct
                         ,TYPE_NUMBER_COMMAND opcode
                         ,const char* data, const int lengh)
    {
        const size_t kAnswerLenhgt = sizeof(answerData)/sizeof(answerData[0]);

        for ( size_t i=0; i<kAnswerLenhgt; i++ )
        {
            const AnswerCmd* tmp = &answerData[i];
            if ((tmp->direct == direct) && (tmp->opcode == opcode))
            {
                return (this->*tmp->run)(data, lengh);
            }
        }

        return ERROR_ANSWER;
    }
    //
    void collectTransportLevel( QQueue<QByteArray> &list );
    void collectTransportLevel( QByteArray &cmd );
    RET_ANSWER checksumReceived(const QByteArray &);
    RET_ANSWER parserReceivedPacket(const QByteArray &);
    void setMessageError( const QString msg ){messageError = msg;messageOk.clear();}
    void setMessageOk( const QString msg ){messageOk = msg;messageError.clear();}
    void sendOutputMessage();
    QByteArray nextAnswerWavTest();
private slots:
    void on_Machine();
public slots:
    void sendMessage(QQueue<QByteArray> &ba);
    void sendMessage(QByteArray &ba);
    void stopMessage();
    void commandRun(const int numberCmd);
    //void setParametrWIFI(const QStringList &);
    void setParametrCOMPORT(const QStringList &);
    void setDateTime(const QDateTime &);
    void setDateTimeZone(const int);
signals:
    void signalStart();
    void signalStop();
    void signalStep();
    void signalTimerDiagnosisDisabled();
    void signalSearchUsb(QStringList&,quint16,quint16);
    void signalSendMessage(const QByteArray&,const QColor&);
    //void signalStatus(const QString&,const QColor&);
    void signalMessageOk(const QString&);
    void signalMessageError(const QString&);
    void signalProgressRange(const int,const int);
    void signalProgressValue(const int);
    void signalProgressVisible(const bool);
    void signalTypeDevice(const int);
    void signalTime(const TYPETIME &);
    void signalDataStatus(const TYPE_STATUS &);
};

#endif // CONTROLLER_H
