#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <qprogressbar.h>
#include <qlabel.h>
#include <qtextedit.h>
#include <qmessagebox.h>
#include <QtWidgets/QSlider>
#include <QRadioButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDebug>
#include <QStackedWidget>
#include <stdint.h>
#include <QFileDialog>

#include "src/FORM/createholidays.h"
#include "src/FORM/createdayplan.h"
#include "src/FORM/createformplaylist.h"
#include "src/FORM/settingsdialog.h"
#include "src/SETTING/settings.h"
#include "src/CONTROLLER/controller.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum PANEL{pnSETTING,pnDIAGNOSIS,pnEND};
    explicit MainWindow(QWidget *parent = nullptr, Controller *pdata=nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    uint8_t value_speaker[2] = {50,50};
    QMap<QAction*,QTextEdit*> mainWindowFormMap;
    QList<QAction*> ListCommand;
    QList<QAction*> ListConnect;
    QList<QAction*> ListView;
    QListWidget* lWgtHoliday;
    //QListWidget* lWgtPlayList;
    QList< QListWidget* > lWgtPlansList;
    QStackedWidget *swPlans;
    SettingsDialog *settings;
    QLabel *statusBar;
    Controller *ptrController;
    QTimer *timerTest;
    QTimer *timeUpdate;
    QButtonGroup *grToolButton;
    QString projectPath;
    const QString date_time_make;
    const QString number_version;

    void createToolTip(void);
    void createToolBar(void);
    void createStatusBar(void);
    void createStyle(void);
    void createConnectionGUI(void);
    void createSoundMenu(void);
    void createPlans( QWidget * const page );
    void createOnePlan(QWidget *page, const int, const int );
    void createHolidays( QWidget * const page );
    void createOneHoliday( QWidget *page, const int maxItem );
    void createFormPlayList( QWidget * const page );
    void setTitle(const QString file_name);
    void writeSettings();
    void readSettings();
    void showStatusMessage(const QString &message);
    void makeItemLabel(QListWidget *lstWgt = nullptr,
                       const QString text = "");

    void makeItemPlan( QListWidget *lstWgt=nullptr,
                       const int number = 1,
                       const QTime time = QTime::currentTime(),
                       const int valume = 30 );
    void makeItemHoliday(QListWidget *lstWgt=nullptr,
                          const int index    = 1,
                          const int max_plan = 1,
                          const QDate date   = QDate::currentDate(),
                          const int num_plan = 1);
    void addItemPlan ( QListWidget * );
    void addItemPlanFromDatabase(QListWidget *const, const int , const int );
    void remoteItemPlan( QListWidget * );
    void clearItemPlan ();
    void addItemHoliday ( QListWidget* );
    void remoteItemHoliday( QListWidget* );
    bool remoteItem( QListWidget* );
    bool indexSorting ( QListWidget* );
    void showLabelEnabled( QLabel *const lb );
    void showLabelDisabled( QLabel *const lb );
    void showLabel( const bool on, QLabel *const lb );
    void updateGuiToPlan();
    void updateOneGuiToPlan(QListWidget * const lWgt,
                         const int numberPlan );
    void updatePlanToGui();
    void updateOnePlanToGui(QListWidget * const lWgt,
                         const int numberPlan);
    void updateGuiToHoliday();
    void updateHolidayToGui();
    void updateGuiToWeek();
    void updateWeekToGui();
    void updateGuiToOther();
    void updateOtherToGui();
    void updateGuiToData();
    void updateDataToGui();
    void defaultChooseConnect(void);

    AccessData &retDataBase()const
    {
        AccessData &data(ptrController->retDataProject().retDataProject());
        return data;
    }
    AccessDataProject &retDataProject()const
    {
        AccessDataProject &data(ptrController->retDataProject());
        return data;
    }
protected:
    void closeEvent(QCloseEvent *event);
    bool eventFilter( QObject *obj, QEvent *evt );
private slots:
    void onWindowsOut();
    void onActionabout();
    void onOpenFile(void);
    void onSaveFiles(void);
    void onToolBar(bool visible);
    void onSwitchPanelMode(int);
    void onAddPlan(void);
    void onRemotePlan(void);
    void onAddItemPlan(void);
    void onRemoteItemPlan(void);
    void onPagePlan( int );
    void onAddHoliday( void );
    void onRemoteHoliday( void );
    void onSetMaxPlanWeek(const int);
    void onCommandRun( const int );
    void onConnect( const int );
    void onCommandReadStatus();
    void onWindowOk(const QString &msg);
    void onWindowError(const QString &msg);
    void onUpdateStatusToGui(const TYPE_STATUS&);
    void onUpdateDataToGui();
    void onUpdateTimeToGui();
    void onTimerDiagnosis(bool);
    void onTimerDiagnosisDisabled();
    void onTimerDiagnosisEnabled();
    void onTimerEnabledDataTime();

signals:
    void signalSendMessage(const QByteArray&,const QColor&);

};

#endif // MAINWINDOW_H
