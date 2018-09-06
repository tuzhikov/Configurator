#include <time.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace UnitHolidays;
using namespace UnitDeyPlan;
using namespace UnitFormPlayList;

/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent, Controller *pdata) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settings( new SettingsDialog(parent) ),
    statusBar(new QLabel(parent)),
    progress(new QProgressBar(parent)),
    ptrController(pdata),
    date_time_make(__TIMESTAMP__),
    number_version("1.100")
{
    ui->setupUi(this);
    //create
    createStatusBar();
    createToolBar();
    createToolTip();
    createConnectionGUI();
    createPlans(ui->planWidget);
    createHolidays(ui->wHoliday);
    createStyle();
    //Read setting in register
    readSettings();
    // database to GUI
    updateDataToGui();
    // install structure datatime
    // ui->dateTimeEdit->setDateTime(ui->dateTimeEdit);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ptrController->setDateTime(ui->dateTimeEdit->dateTime());
    timeUpdate = new QTimer(this);
    timeUpdate->start(1000);
    //connect(&timer, &QTimer::timeout, this, &MainWindow::onUpdateTimeToGui);
    connect(timeUpdate, SIGNAL(timeout()),this,SLOT(onUpdateTimeToGui()));

    //QTime updateTime  = new (updateTime);

     //connect(updateTime, &QAction::triggered, settings, &SettingsDialog::openWindowsSetting);
    // macros
    qDebug()<<date_time_make;
    //test
//    QDateTime test(ui->dateTimeEditConfig->dateTime());
//    qDebug()<<test<<test.currentDateTimeUtc().toTime_t()<<test.currentDateTimeUtc().toMSecsSinceEpoch();
//    QDateTime test2;
//    test2.setTime_t(test.currentDateTimeUtc().toTime_t());
//    qDebug()<<test2;
//    QDateTime test3;
//    test3.setTime_t(test.currentDateTimeUtc().toMSecsSinceEpoch()/1000);
//    qDebug()<<test3;
//    qDebug()<<test3.toTimeSpec(Qt::UTC);

    ///////////////
    /*
    time_t rawtime;
    struct tm * ptm;
    //ptm = localtime( &rawtime );
    QDateTime test(QDateTime::currentDateTime());
    ptm->tm_hour = test.time().hour();
    ptm->tm_min = test.time().minute();
    ptm->tm_sec = test.time().second();
    ptm->tm_year = test.date().year()-1900;
    ptm->tm_mon = test.date().month();
    ptm->tm_mday = test.date().day();
    ptm->tm_yday = test.date().dayOfYear();
    ptm->tm_wday = test.date().dayOfWeek();

    rawtime = mktime( ptm );
    test.setTime_t(rawtime);qDebug()<<test;

    qDebug()<<rawtime;
    qDebug()<<ptm->tm_year<<ptm->tm_mon<<ptm->tm_mday<<ptm->tm_wday<<ptm->tm_yday;
    qDebug()<<ptm->tm_hour<<ptm->tm_min<<ptm->tm_sec<<ptm->tm_isdst;
    time_t raw = mktime( ptm );
    struct tm * ptm1;
    ptm1 = gmtime( &raw );
    qDebug()<<raw;
    qDebug()<<ptm1->tm_year<<ptm1->tm_mon<<ptm1->tm_mday<<ptm1->tm_wday<<ptm1->tm_yday;
    qDebug()<<ptm1->tm_hour<<ptm1->tm_min<<ptm1->tm_sec<<ptm1->tm_isdst;

    QDateTime test4;test4.setTime_t(raw);
    qDebug()<<test4;
    */
//    uint64_t time(0x59439ADC);
//    const QString strtime(
//                QDateTime::fromTime_t(time,QTimeZone::systemTimeZone()).toString("dd.MM.yyyy hh:mm"));
//    qDebug()<<strtime;
    //    // test time
//    QTime myTimer;
//    myTimer.start();
//    int nMilliseconds = myTimer.elapsed();
//    qDebug()<<nMilliseconds;
}
/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
}
/**
 * @brief MainWin::writeSettings
 */
void MainWindow::writeSettings()
{
    Settings::set(Settings::GEOMETRY) = saveGeometry();
    Settings::set(Settings::PRJ_PACH) = projectPath.isEmpty()?QString(): projectPath;
    Settings::set(Settings::COMNAME,Settings::COMPORT) = settings->getComName();
    Settings::set(Settings::BAUDRATE,Settings::COMPORT) = settings->getBaudRate();
    Settings::set(Settings::DATABITS,Settings::COMPORT) = settings->getDataBits();
    Settings::set(Settings::STOPBITS,Settings::COMPORT) = settings->getStopBits();
    Settings::set(Settings::PARITY,Settings::COMPORT) = settings->getParity();
    Settings::set(Settings::FLOWCONTROL,Settings::COMPORT) = settings->getFlowControl();
}
/**
 * @brief MainWin::readSettings
 */
void MainWindow::readSettings()
{
    // set default
    QFile file(":/config/default.ini");
    file.open(QIODevice::ReadOnly);
    Settings::setDefaults(file.readAll());
    // read
    const QByteArray gemData = Settings::get(Settings::GEOMETRY).toByteArray();
    restoreGeometry(gemData);
    // COM PORT
    const QString comname = Settings::get(Settings::COMNAME,Settings::COMPORT).toString();
    if (!comname.isEmpty())
        settings->setComName(comname);
    const QString baudRate = Settings::get(Settings::BAUDRATE,Settings::COMPORT).toString();
    if (!baudRate.isEmpty())
        settings->setBaudRate(baudRate);
    const QString databits = Settings::get(Settings::DATABITS,Settings::COMPORT).toString();
    if (!databits.isEmpty())
        settings->setDataBits(databits);
    const QString stopbits = Settings::get(Settings::STOPBITS,Settings::COMPORT).toString();
    if (!stopbits.isEmpty())
        settings->setStopBits(stopbits);
    const QString parity = Settings::get(Settings::PARITY,Settings::COMPORT).toString();
    if (!parity.isEmpty())
        settings->setParity(parity);
    const QString flowcnt = Settings::get(Settings::FLOWCONTROL,Settings::COMPORT).toString();
    if (!flowcnt.isEmpty())
        settings->setFlowControl(flowcnt);
    settings->updateParametrs();
}
/**
 * @brief MainWindow::closeEvent
 * @param event
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    delete settings;
    event->accept();
}
/**
 * @brief MainWindow::createToolTip
 */
void MainWindow::createToolTip()
{
    ui->actionWrite->setToolTip(tr("Write all"));
    ui->actionRead->setToolTip(tr("Read all"));
    ui->actionDevice->setToolTip(tr("Get the type of device."));
}
/**
 * @brief MainWindow::createToolBar
 */
void MainWindow::createToolBar()
{
    // add ListCommand
    ListCommand<<ui->actionWrite<<ui->actionRead;
    ui->mainToolBar->addActions(ListCommand);
    //ui->actionDevice->setVisible(false); //
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->actionDevice);
}
/**
 * @brief MainWindow::createStatusBar
 */
void MainWindow::createStatusBar()
{
     /*Add progress in status bar*/
    progress->setVisible(false);
    progress->setTextVisible(true);
    progress->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    // connect progressbar
    connect(ptrController,&Controller::signalProgressRange,progress,&QProgressBar::setRange,Qt::DirectConnection);
    connect(ptrController,&Controller::signalProgressValue,progress,&QProgressBar::setValue,Qt::DirectConnection);
    connect(ptrController,&Controller::signalProgressVisible,progress,&QProgressBar::setVisible,Qt::DirectConnection);

    ui->statusBar->addPermanentWidget(progress);
    /*Set message in status bar*/
    QLabel *p = new QLabel(this);
    p->setText(tr("Version:")+number_version);
    p->setOpenExternalLinks(true);
    ui->statusBar->addPermanentWidget(p);
    ui->statusBar->addWidget(statusBar);
    ui->statusBar->showMessage(tr("Configurator is open."),2000);
}
/**
 * @brief MainWindow::createStyle
 */
void MainWindow::createStyle()
{
    /*set Style Sweet the windows and button*/
    QFile styleFile(":/QSS/style_files001.qss");
    bool result = styleFile.open(QFile::ReadOnly);
    if(result){
      QByteArray style = styleFile.readAll();
      qApp->setStyleSheet(style);
    }
}
/**
 * @brief MainWindow::createConnectionGUI
 */
void MainWindow::createConnectionGUI()
{
    /*set slot GUI*/
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(onActionabout())); // about
    // toolbar
    connect(ui->mainToolBar,SIGNAL(visibilityChanged(bool)),this,SLOT(onToolBar(bool)));
    // connect add plan
    connect(ui->pbAddPlan,SIGNAL(clicked(bool)),this,SLOT(onAddPlan()));
    connect(ui->pbRemotePlan,SIGNAL(clicked(bool)),this,SLOT(onRemotePlan()));
    connect(ui->cbSelectPlan,SIGNAL(currentIndexChanged(int)),this,SLOT(onPagePlan(int)));
    // connect holiday
    connect(ui->pbAddHoliday,SIGNAL(clicked(bool)),this,SLOT(onAddHoliday()));
    connect(ui->pbRemoteHoliday,SIGNAL(clicked(bool)),this,SLOT(onRemoteHoliday()));
    // connect plan
    connect(ui->pbAddItemPlan,SIGNAL(clicked(bool)),this,SLOT(onAddItemPlan()));
    connect(ui->pbRemoteItemPlan,SIGNAL(clicked(bool)),this,SLOT(onRemoteItemPlan()));
    //connect week
    connect(ui->sbTotalPlan,SIGNAL(valueChanged(int)),this,SLOT(onSetMaxPlanWeek(int)));
    // win setting uart
    connect(ui->actionDevice, &QAction::triggered, settings, &SettingsDialog::openWindowsSetting);

    // command
    QSignalMapper *smCommand = new QSignalMapper(this);
    connect(smCommand,SIGNAL(mapped(int)),this,SLOT(onCommandRun(int)));

    for (int i = 0; i <ListCommand.size(); i++)
    {
        if (i <ListCommand.size())
        {
            smCommand->setMapping(ListCommand.at(i), i);
            connect(ListCommand.at(i), SIGNAL(triggered()), smCommand,SLOT(map()));
        }
    }
    // interface
    QSignalMapper *smConnect = new QSignalMapper(this);
    connect(smConnect,SIGNAL(mapped(int)),this,SLOT(onConnect(int)));
    for (int i = 0; i<ListConnect.size(); i++)
    {
        smConnect->setMapping(ListConnect.at(i), i);
        connect(ListConnect.at(i), SIGNAL(triggered()), smConnect,SLOT(map()));
    }
    // command test
    timerTest = new QTimer(this);
    connect(ui->pb_GetStatus, &QPushButton::clicked, this, &MainWindow::onCommandReadStatus);
    //connect(timerTest,&QTimer::timeout,this,&MainWindow::onCommandReadStatus);
    //connect(ui->gbDiagnosis,&QGroupBox::toggled,this,&MainWindow::onTimerDiagnosis);
    connect(ptrController,&Controller::signalTimerDiagnosisDisabled,this,&MainWindow::onTimerDiagnosisDisabled);
    // update new
    connect(ptrController,&Controller::signalStop,this,&MainWindow::onUpdateDataToGui);
    // message
    connect(ptrController,&Controller::signalMessageOk,this,&MainWindow::onWindowOk);
    connect(ptrController,&Controller::signalMessageError,this,&MainWindow::onWindowError);
    //set parametrs connecnt
    connect(settings,&SettingsDialog::signalListParametr,ptrController,&Controller::setParametrCOMPORT);
    // TIME
    connect(ui->dateTimeEdit,&QDateTimeEdit::dateTimeChanged,ptrController,&Controller::setDateTime);
    connect(ui->gbOtherDate_Time, &QGroupBox::clicked, this, &MainWindow::onTimerEnabledDataTime);
    // update status
    connect(ptrController,&Controller::signalDataStatus, this, &MainWindow::onUpdateStatusToGui);
    // open/save project file
    // connect save \ open files
    connect(ui->actionSaveProject,SIGNAL(triggered(bool)),this,SLOT(onSaveFiles()));
    connect(ui->actionOpenProject,SIGNAL(triggered(bool)),this,SLOT(onOpenFile()));
}

/**
 * @brief MainWindow::makeItemLabel
 * @param lstWgt
 * @param text
 */
void MainWindow::makeItemLabel(QListWidget *lstWgt, const QString text)
{
    QLabel *form = new QLabel( text, lstWgt );
    form->setAlignment(Qt::AlignCenter); //qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #707070, stop: 1 #FFFFFF)
    form->setStyleSheet("QLabel {background-color: #D3D3D3;"
                        "border: 1px solid gray;border-radius: 5px;margin-top: 5ex;}");
    QListWidgetItem *item = new QListWidgetItem( lstWgt );
    item->setSizeHint( form->sizeHint() );
    lstWgt->setItemWidget( item, form );
}

/**
 * @brief MainWindow::makeItemPlan
 * @param lstWgt
 * @param number
 * @param time
 * @param val1
 * @param val2
 */
void MainWindow::makeItemPlan( QListWidget *lstWgt,
                               const int number,
                               const QTime time,
                               const int valume )
{
    CreateDayPlan *form = new CreateDayPlan(this,number,time, valume);
    QListWidgetItem *item = new QListWidgetItem( lstWgt );
    item->setSizeHint( form->sizeHint() );
    lstWgt->setItemWidget( item, form );
}
/**
 * @brief MainWindow::makeItemHoliday
 * @param lstWgt
 * @param index
 * @param mounth
 * @param day
 * @param num_plan
 */
void MainWindow::makeItemHoliday(QListWidget *lstWgt,
                                 const int index,
                                 const int max_plan,
                                 const QDate date,
                                 const int num_plan)
{
    CreateHolidays *form = new CreateHolidays( this,index,date,max_plan,num_plan );
    QListWidgetItem *item = new QListWidgetItem( lstWgt );
    item->setSizeHint( form->sizeHint() );
    lstWgt->setItemWidget(item,form);
    connect(ui->sbTotalPlan,SIGNAL(valueChanged(int)),form,SLOT(setMaxPlan(int)));
}
/**
 * @brief MainWindow::addItemPlanFromDatabase
 * @param lWgt
 * @param number_plan
 */
void MainWindow::addItemPlanFromDatabase(QListWidget *const lWgt, const int number_plan, const int item)
{
    const int index_item = item+1;
    TYPEPLANITEMS day = {};
    retDataBase().getOnePlanDay(number_plan, item, &day);
    const QTime time(day.hour, day.min);
    // create new item
    makeItemPlan(lWgt, index_item, time, day.Volume );
    //qDebug("MainWindow::addItemPlanFromDatabase numberPlan = %d index_item = %d", number_plan, index_item );
}
/**
 * @brief MainWindow::addItemPlan
 */
void MainWindow::addItemPlan(QListWidget *lWgt)
{
    if( !lWgt ) return ;
    // read from database
    const int max_plan = retDataBase().getMaxPlans();
    // add new item
    const int indexShow = lWgt->count()+1;
    if(indexShow<=max_plan)
    {
        makeItemPlan( lWgt, indexShow );
        qDebug("add items plan %d",indexShow);
    }
}
/**
 * @brief MainWindow::remoteItemPlan
 */
void MainWindow::remoteItemPlan(QListWidget *lWgt)
{
    remoteItem(lWgt);
}
/**
 * @brief MainWindow::clearItemPlan
 */
void MainWindow::clearItemPlan()
{
    QList< QListWidget* > lWgts(lWgtPlansList);

    foreach (QListWidget* lWd, lWgts)
    {
        const int lengh(lWd->count());
        for (int i=0; i<lengh; i++)
        {
            remoteItemPlan(lWd);
        }
    }
}
/**
 * @brief MainWindow::addItemHoliday
 * @param lWgt
 */
void MainWindow::addItemHoliday(QListWidget *lWgt)
{
    if( !lWgt ) return ;
    // read from database
    const int max_holiday = retDataBase().getMaxHoliday();
    //gui max plan
    const int max_current_plan = ui->sbTotalPlan->value();
    // add new item
    const int indexShow = lWgt->count()+1;
    if(indexShow<=max_holiday)
    {
        makeItemHoliday( lWgt, indexShow, max_current_plan);
        qDebug("add items%d",indexShow);
    }
}
/**
 * @brief MainWindow::remoteItemHoliday
 * @param lWgt
 */
void MainWindow::remoteItemHoliday(QListWidget *lWgt)
{
    remoteItem(lWgt);
}
/**
 * @brief MainWindow::remoteItem
 * @param lWgt
 * @return
 */
bool MainWindow::remoteItem(QListWidget *lWgt)
{
    if( !lWgt ) return false;
    // select items
    QList< QListWidgetItem* > items = lWgt->selectedItems();
    if (items.empty())
    {
        lWgt->setCurrentRow(lWgt->count()-1);
        items<<lWgt->currentItem();
    }
    foreach( QListWidgetItem* item, items )
    {
        int row = lWgt->row( item );
        QWidget *form = lWgt->itemWidget(item);
        lWgt->takeItem( row );
        delete form;
        delete item;
    }
    // sorting index
    indexSorting(lWgt);
    qDebug("remote items %d",lWgt->count());
    return true;
}
/**
 * @brief MainWindow::indexSorting
 * @return
 */
bool MainWindow::indexSorting(QListWidget *lWgt)
{
    if( !lWgt ) return false;
    for(int i=0;i<lWgt->count();i++)
    {
        QListWidgetItem *item = lWgt->item(i);
        InterfaceForms *form = dynamic_cast<InterfaceForms *>( lWgt->itemWidget(item) );
        form->setNumberItem(i+1);
    }
    return true;
}
/**
 * @brief MainWindow::createPlans
 */
void MainWindow::createPlans( QWidget * const page )
{
    // read from database max plan
    const int max_plans = retDataBase().getMaxPlans();
    // create form plans
    swPlans = new QStackedWidget(page);
    for(int i=0; i<max_plans;i++)
    {
        //read from database
        const int item_plan = retDataBase().countItemPlan(i);
        // create one plan
        QWidget *wdg = new QWidget;
        createOnePlan(wdg, i, item_plan);
        swPlans->addWidget(wdg);
        qDebug("MainWindow::createPlans: item_plan=%d",item_plan);
    }
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(swPlans);
    layout->setContentsMargins(0,0,0,0);
    page->setLayout(layout);
    qDebug("MainWindow::createPlans: swPlans->count()=%d",swPlans->count());
}

/**
 * @brief MainWindow::createOnePlan
 * @param page
 * @param maxItem
 */
void MainWindow::createOnePlan( QWidget *page, const int number_plan, const int maxItem )
{
    QListWidget* lWgt = new QListWidget;

    QLayout* vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(0,0,0,0);
    vlayout->addWidget( lWgt );

    page->setLayout(vlayout);
    for (int i = 0; i<maxItem; i++)
    {
        addItemPlanFromDatabase( lWgt, number_plan, i);
    }
    // add list
    lWgtPlansList.append(lWgt);
}
/**
 * @brief MainWindow::createHolidays
 */
void MainWindow::createHolidays(QWidget *const page)
{
    // read from database
    const int count_holiday = retDataBase().countItemHoliday();
    // create form holiday
    createOneHoliday(page,count_holiday);
}
/**
 * @brief MainWindow::createOneHoliday
 * @param page
 * @param maxItem
 */
void MainWindow::createOneHoliday(QWidget *page, const int maxItem)
{
    QListWidget* lWgt = new QListWidget;

    QLayout* vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(0,0,0,0);
    vlayout->addWidget( lWgt );

    page->setLayout(vlayout);

    for (int i = 1; i<=maxItem; i++)
    {
        // read from database
        const int max_plan = retDataBase().countPlan();
        TYPEHOLIDAY item = {};
        retDataBase().getOneHoliday(i,&item);
        QDate date(QDate::currentDate().year(),item.month,item.day);
        // make item
        makeItemHoliday( lWgt,i,max_plan,date,(item.plan+1) );
    }
    // private pointer
    lWgtHoliday = lWgt;
}

/**
 * @brief MainWindow::setTitle
 * @param str
 */
void MainWindow::setTitle(const QString file_name)
{
    const QString wintitle(QObject::tr("Configurator    "));
    this->setWindowTitle(wintitle+file_name);
}
/**
 * @brief MainWindow::onWindowsOut
 */
void MainWindow::onWindowsOut()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QTextEdit *window = mainWindowFormMap[action];
    if(!window){
          // create debug windows
          window = new QTextEdit;
          window->setWindowIcon(QPixmap(":/ico/win48x48.ico"));
          window->installEventFilter(this);
          mainWindowFormMap.insert(action, window);
          }
    window->setVisible(action->isChecked());
}
/**
 * @brief MainWindow::onActionabout
 */
void MainWindow::onActionabout()
{
    QMessageBox::about(this,tr("About the program a Configure."),
                            tr(
                           "<h3>The program configure. version: %1 </h3>"
                           "<p>All rights to this software belong to Co., Ltd. RTK</p>"
                           "<p> <a href=""www.rtk-orel.ru""> www.rtk-orel.ru </a></p>"
                           "<p> Build time: %2 </p>"
                           ).arg(number_version).arg(date_time_make));
}
/**
 * @brief MainWindow::onToolBar
 * @param visible
 */
void MainWindow::onToolBar(bool visible)
{
    ui->mainToolBar->setVisible(visible);
}

/**
 * @brief MainWindow::onSwitchPanelMode
 * @param numberPanel
 */
void MainWindow::onSwitchPanelMode(int numberPanel)
{
    if(numberPanel<pnEND)
    {
        ui->stackedWidget->setCurrentIndex(numberPanel);
    }
}
/**
 * @brief MainWindow::onAddPlan
 */
void MainWindow::onAddPlan()
{
    // read from database max parametr
    const int max_plans = retDataBase().getMaxPlans();
    // add
    const int index = ui->sbTotalPlan->value()+1;
    if (index<max_plans)
    {
        ui->sbTotalPlan->setValue(index);
        // set item combobox
        ui->cbSelectPlan->clear();
        for(int i=1;i<=index;i++)
        {
            ui->cbSelectPlan->addItem(tr("Number Plan ")+QString::number(i));
        }
    }
    // set current page
    const int index_curr = ui->sbTotalPlan->value()-1;
    ui->cbSelectPlan->setCurrentIndex(index_curr);
    onPagePlan(index_curr);
}
/**
 * @brief MainWindow::onRemotePlan
 */
void MainWindow::onRemotePlan()
{
    const int index = ui->sbTotalPlan->value()-1;
    if (index)
    {
        // set new value
        ui->sbTotalPlan->setValue(index);
        // remote item combobox
        ui->cbSelectPlan->clear();
        for(int i=1;i<=index;i++)
        {
            ui->cbSelectPlan->addItem(tr("Number Plan ")+QString::number(i));
        }
    }
}
/**
 * @brief MainWindow::onAddItemPlan
 */
void MainWindow::onAddItemPlan()
{
    const int index = ui->cbSelectPlan->currentIndex();
    if ( ( !lWgtPlansList.isEmpty()) && (index<lWgtPlansList.count()))
    {
        addItemPlan(lWgtPlansList.at(index));
        qDebug("onAddItemPlan add %d",ui->cbSelectPlan->currentIndex());
    }else{
        qDebug("onAddItemPlan ERROR LENGH %d",index);
    }

}
/**
 * @brief MainWindow::onRemoteItemPlan
 */
void MainWindow::onRemoteItemPlan()
{
    const int index = ui->cbSelectPlan->currentIndex();
    if ( ( !lWgtPlansList.isEmpty()) && (index<lWgtPlansList.count()))
    {
        remoteItemPlan(lWgtPlansList.at(index));
        qDebug("onRemoteItemPlan remote %d",ui->cbSelectPlan->currentIndex());
    }else{
        qDebug("onRemoteItemPlan ERROR LENGH %d",index);
    }
}
/**
 * @brief MainWindow::onPagePlan
 */
void MainWindow::onPagePlan(int index)
{
    swPlans->setCurrentIndex(index);
    ui->gbCurrentPlan->setTitle(tr("Current Plan ")+QString::number(index+1));
}
/**
 * @brief MainWindow::onAddHoliday
 */
void MainWindow::onAddHoliday()
{
    addItemHoliday(lWgtHoliday);
}
/**
 * @brief MainWindow::onRemoteHoliday
 */
void MainWindow::onRemoteHoliday()
{
    remoteItemHoliday(lWgtHoliday);
}

/**
 * @brief MainWindow::showLabelEnabled
 * @param lb
 */
void MainWindow::showLabelEnabled(QLabel * const lb)
{
    lb->setStyleSheet("background: green;");
}
/**
 * @brief MainWindow::showLabelDisabled
 * @param lb
 */
void MainWindow::showLabelDisabled(QLabel * const lb)
{
    lb->setStyleSheet("background: red;");
}
/**
 * @brief MainWindow::showLabel
 * @param on
 * @param lb
 */
void MainWindow::showLabel(const bool on, QLabel * const lb)
{
    if (on)
        showLabelEnabled(lb);
    else
        showLabelDisabled(lb);
}

/**
 * @brief MainWindow::updateGuiToPlan
 * @param numberPlan
 */
void MainWindow::updateOneGuiToPlan(QListWidget *const lWgt, const int numberPlan)
{
    for ( int i=0; i<lWgt->count(); i++ )
    {
        QListWidgetItem *const item = lWgt->item(i);
        CreateDayPlan *const formItem = dynamic_cast<CreateDayPlan *>( lWgt->itemWidget(item) );
        int volume = 0;

        formItem->getVolume( &volume );

        TYPEPLANITEMS plan;
        plan.hour = static_cast<uint8_t>(formItem->getTime().hour());
        plan.min = static_cast<uint8_t>(formItem->getTime().minute());
        plan.Volume = static_cast<uint8_t>(volume);
        plan.BrightRG = static_cast<uint8_t>(100);
        plan.bSensor = (uint8_t)0;
        if ( retDataBase().setOnePlanDay( numberPlan, i, plan) == false )
        {
            qDebug() << "Class MainWindow: functions updateGuiToPlan: ERROR";
        }
    }
    //set active plans
    if ( retDataBase().setActiveOnePlanDay( numberPlan ) == false )
    {
        qDebug() << "Class MainWindow: functions updateGuiToPlan: ERROR";
    }

}
/**
 * @brief MainWindow::updateGuiToPlan
 */
void MainWindow::updateGuiToPlan()
{
    QList< QListWidget* > lWgts(lWgtPlansList);
    retDataBase().clearPlans();

    for (int i=0;i<lWgts.count(); i++)
    {
        QListWidget *const lWgt(lWgts.at(i));
        updateOneGuiToPlan(lWgt, i);
    }
}
void MainWindow::updateOnePlanToGui(QListWidget * const lWgt,
                                 const int numberPlan)
{
    const int count_item = retDataBase().countItemPlan(numberPlan);
    for ( int i=0; i<count_item; i++)
    {
        addItemPlanFromDatabase(lWgt, numberPlan, i);
    }
    qDebug("MainWindow::updateOnePlanToGui count_item=%d",count_item);
}
/**
 * @brief MainWindow::updatePlanToGui
 */
void MainWindow::updatePlanToGui()
{
    // delete item
    clearItemPlan();
    // add new items
    QList< QListWidget* > lWgts(lWgtPlansList);
    const int count_plan = retDataBase().countPlan();
    for ( int i=0; i<count_plan; i++)
    {
        QListWidget *const iwd = lWgts.at(i);
        updateOnePlanToGui(iwd, i);
    }
    // set tool date
    ui->sbTotalPlan->setValue(count_plan);
    // set item combobox (If the plans are zero)
    ui->cbSelectPlan->clear();
    for(int i=0;i<count_plan;i++)
    {
        ui->cbSelectPlan->addItem(tr("Number Plan ")+QString::number(i+1));
    }
    // If the plans are zero,
    if (!count_plan) // bug configurations
    {
        ui->cbSelectPlan->addItem(tr("Number Plan 1"));
    }
    qDebug("MainWindow::updatePlanToGui count_plan=%d",count_plan);
}
/**
 * @brief MainWindow::updateGuiToHoliday
 */
void MainWindow::updateGuiToHoliday()
{
    QListWidget const *const lWgt(lWgtHoliday);

    retDataBase().clearHoliday();

    for(int i=0;i<lWgt->count();i++)
    {
        QListWidgetItem *const item = lWgt->item(i);
        CreateHolidays *const formItem = dynamic_cast<CreateHolidays *>( lWgt->itemWidget(item) );

        TYPEHOLIDAY holiday;
        holiday.month = static_cast<uint8_t>(formItem->getDateEdit().month());
        holiday.day = static_cast<uint8_t>(formItem->getDateEdit().day());
        holiday.plan = static_cast<uint8_t>(formItem->getNumberPlan()-1);

        if ( retDataBase().setOneHoliday(i,holiday) == false )
        {
            qDebug() << "Class MainWindow: functions updateGuiToHoliday: ERROR";
        }
    }
}
/**
 * @brief MainWindow::updateHolidayToGui
 */
void MainWindow::updateHolidayToGui()
{

    QListWidget *const lWgt(lWgtHoliday);

    if (!lWgt) return;
    // remote old page
    const int max_item(lWgt->count());
    for ( int i = 0; i<max_item;i++ )
    {
        remoteItemHoliday(lWgt);
    }
    // create new page
    const uint32_t max_current_plan = retDataBase().countPlan();
    const uint32_t max_current_holiday = retDataBase().countItemHoliday();
    for ( uint32_t i=0; i<max_current_holiday; i++)
    {
        TYPEHOLIDAY holiday = {};
        retDataBase().getOneHoliday(i, &holiday);
        const QDate date(QDate::currentDate().year(), holiday.month, holiday.day);
        makeItemHoliday( lWgt, (i+1), max_current_plan, date, (holiday.plan+1) );// The plan starts from zero
        qDebug("updateHolidayToGui %d",(i+1));
    }
}
/**
 * @brief MainWindow::updateGuiToWeek
 */
void MainWindow::updateGuiToWeek()
{

    QList <QSpinBox*> list; list<<ui->sbMonday<<ui->sbTuesday<<ui->sbWednesday
                               <<ui->sbThursday<<ui->sbFriday<<ui->sbSaturday<<ui->sbSunday;
    for (int i = 0; i<list.count(); i++)
    {
        const int plan = list.at(i)->value()?(list.at(i)->value()-1):0;
        retDataBase().setOneWeek(i, plan);
    }
}
/**
 * @brief MainWindow::updateWeekToGui
 */
void MainWindow::updateWeekToGui()
{
    QList <QSpinBox*> list; list<<ui->sbMonday<<ui->sbTuesday<<ui->sbWednesday
                               <<ui->sbThursday<<ui->sbFriday<<ui->sbSaturday<<ui->sbSunday;
    for (int i = 0; i<list.count(); i++)
    {
        uint8_t plan = 0;
        retDataBase().getOneWeek(i, &plan);
        list.at(i)->setValue((plan+1)); //
    }
}
/**
 * @brief MainWindow::ToOther
 */
void MainWindow::updateGuiToOther()
{
    // setting
    TYPESETTING setting ={};
    setting.SampleNum = static_cast<uint8_t>(ui->sb_sound_number->value()-1);
    setting.TeachingPeriods = 2;
    setting.SpeedUpTime = static_cast<uint8_t>(ui->sb_time_acceleration->value());
    setting.bSpeedUp = static_cast<uint8_t>(ui->gb_acceleration->isChecked());
    setting.mode_manual = static_cast<uint8_t>(ui->gbRS485->isChecked());
    setting.Direction = static_cast<uint8_t>(ui->sb_data_direct->value());
    retDataBase().setSetting( setting );
}
/**
 * @brief MainWindow::updateOtherToGui
 */
void MainWindow::updateOtherToGui()
{
    // setting
    TYPESETTING setting ={};
    retDataBase().getSetting( &setting );
    ui->sb_time_acceleration->setValue(static_cast<int>(setting.SpeedUpTime));
    ui->gb_acceleration->setChecked(static_cast<bool>(setting.bSpeedUp));
    ui->sb_sound_number->setValue(static_cast<int>(setting.SampleNum)+1);
    ui->gbRS485->setChecked(static_cast<bool>(setting.mode_manual));
    ui->sb_data_direct->setValue(static_cast<int>(setting.Direction));
}
/**
 * @brief MainWindow::updateGuiToData
 */
void MainWindow::updateGuiToData()
{
   // plan
    updateGuiToPlan();
    //week
    updateGuiToWeek();
    // holiday
    updateGuiToHoliday();
    // setting
    updateGuiToOther();
}
/**
 * @brief MainWindow::updateDataToGui
 */
void MainWindow::updateDataToGui()
{
    // plan
    updatePlanToGui();
    //week
    updateWeekToGui();
    // holiday
    updateHolidayToGui();
    // setting
    updateOtherToGui();
}
/**
 * @brief MainWindow::defaultChooseConnect
 */
void MainWindow::defaultChooseConnect()
{
    onConnect(0);
}

/**
 * @brief MainWindow::onSetMaxPlan
 */
void MainWindow::onSetMaxPlanWeek(const int maxPlan)
{
    ui->sbMonday->setMaximum(maxPlan);
    ui->sbTuesday->setMaximum(maxPlan);
    ui->sbWednesday->setMaximum(maxPlan);
    ui->sbThursday->setMaximum(maxPlan);
    ui->sbFriday->setMaximum(maxPlan);
    ui->sbSaturday->setMaximum(maxPlan);
    ui->sbSunday->setMaximum(maxPlan);
}
/**
 * @brief MainWindow::onOpenFile
 */
void MainWindow::onOpenFile(void)
{
    const QString projectPaths(
                projectPath.isEmpty()?QDir::homePath() : projectPath );
    const QString file =
        QFileDialog::getOpenFileName(this,tr("Open File"), projectPaths, tr("VCC (*.vcc);;"));

    if(!file.isEmpty())
    {
        // save pach
        projectPath = QFileInfo(file).path();
        //
        QFile readFile(file);
        readFile.open(QFile::ReadOnly);
        QDataStream outFile(&readFile);
        outFile.setVersion(QDataStream::Qt_DefaultCompiledVersion);
        retDataProject().readFileToProject(outFile);
        readFile.close();
        //update GUI
        updateDataToGui();
        // set file to title
        setTitle(file);
    }
}
/**
 * @brief MainWindow::onSaveFiles
 */
void MainWindow::onSaveFiles(void)
{
    //update
    updateGuiToData();
    //
    const QString projectPaths(projectPath.isEmpty()?QDir::homePath() : projectPath );
    const QString file =
        QFileDialog::getSaveFileName(this,tr("Save File"),projectPaths, tr("VCC (*.vcc);;"));

    if(!file.isEmpty())
    {
        // save pach
        projectPath = QFileInfo(file).path();
        QFile appFile(file);
        //appFile.open(QFile::Append); // открываем файл для дозаписи;
        appFile.open(QIODevice::WriteOnly);
        QDataStream inFile(&appFile); // передаем потоку указатель на QIODevice;
        inFile.setVersion(QDataStream::Qt_DefaultCompiledVersion); //  явно указываем версию Qt, для сериализации;
        retDataProject().writeProjectToFile(inFile);
        appFile.flush(); // записываем весь буффер в файл;
        appFile.close();
        // set file to title
        setTitle(QFileInfo(file).fileName());
    }
}

/**
 * @brief MainWindow::onCommandRun
 * @param indexCommand
 */
void MainWindow::onCommandRun(const int indexCommand)
{
      // read data
    if ( ( indexCommand == Controller::cmReadAll ))
    {
//        const QMessageBox::StandardButtons buttons =
//            QMessageBox::question(this,tr("Read the data from UZTVOP"),tr("Current data will be lost!"));
//        if ( buttons != QMessageBox::Yes)
//        {
//            return;
//        }
    }
    ui->mainToolBar->setEnabled(false);// lock button
    //update
    updateGuiToData();
    // run command
    ptrController->commandRun(indexCommand);
    //qDebug("number command %d",indexCommand);
}
/**
 * @brief MainWindow::onConnect
 */
void MainWindow::onConnect(const int number)
{
    qDebug("number connect %d",number);
}
/**
 * @brief MainWindow::onCommandReadStatus
 */
void MainWindow::onCommandReadStatus()
{
    ptrController->commandRun(Controller::cmReadStatus);
    qDebug("command test %d",Controller::cmReadStatus);
}
/**
 * @brief MainWindow::onWindowError
 * @param msg
 */
void MainWindow::onWindowError(const QString &msg)
{
    QMessageBox::critical(this,tr("Device Error!"),msg);
}

/**
 * @brief MainWindow::onUpdateStatusToGui
 * @param status
 */
void MainWindow::onUpdateStatusToGui(const TYPE_STATUS& status)
{
    const QDate date(status.time.year, status.time.month, status.time.day);
    qDebug()<< date.toString("dd.MM.yyyy");
    const QTime time(status.time.hour, status.time.min, status.time.sec);
    qDebug()<< time.toString("hh:mm:ss");

    ui->lb_data_time->setText(date.toString("dd.MM.yyyy")+"/"+time.toString("hh:mm:ss"));
    ui->lb_data_item->setText(QString::number(status.CurItem));
    ui->lb_data_plan->setText(QString::number(status.CurPlan));
    ui->lb_data_value->setText(QString::number(status.CurVolume)+"%");
    ui->lb_data_error->setText(QString::number(status.Errors));
}

/**
 * @brief MainWindow::onUpdateDataToGui
 */
void MainWindow::onUpdateDataToGui()
{
    updateDataToGui(); // update date
    ui->mainToolBar->setEnabled(true); // unlock button
}

void MainWindow::onUpdateTimeToGui()
{
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
}
/**
 * @brief MainWindow::onTimerDiagnosisEnabled
 * @param enabled
 */
void MainWindow::onTimerDiagnosis(bool enabled)
{
    if ( enabled )
    {
        onTimerDiagnosisEnabled();
    }
    else
    {
        onTimerDiagnosisDisabled();
    }
}
/**
 * @brief MainWindow::onTimerDiagnosisDisabled
 */
void MainWindow::onTimerDiagnosisDisabled()
{
    timerTest->stop();
}
/**
 * @brief MainWindow::onTimerDiagnosisEnabled
 */
void MainWindow::onTimerDiagnosisEnabled()
{
    timerTest->start(1000);
}

/**
 * @brief MainWindow::onTimerEnabledDataTime
 */
void MainWindow::onTimerEnabledDataTime()
{
    bool enabled = ui->gbOtherDate_Time->isChecked();

    if ( enabled == true )
    {
        timeUpdate->stop();
    }
    else
    {
        timeUpdate->start();
    }
}

/**
 * @brief MainWindow::onWindowOk
 * @param msg
 */
void MainWindow::onWindowOk(const QString &msg)
{
    QMessageBox::information(this,tr("Device response!"),msg);
}
/**
 * @brief MainWindow::eventFilter
 * @param obj
 * @param evt
 * @return
 */
bool MainWindow::eventFilter(QObject *obj, QEvent *evt)
{

    if ( evt->type() == QEvent::Close)
    {

    }
    if( evt->type() == QEvent::MouseButtonDblClick )
    {

    }
    if( evt->type() == QEvent::MouseButtonRelease )
    {

    }
    return QMainWindow::eventFilter( obj, evt );
}
/**
 * @brief MainWindow::showStatusMessage
 * @param message
 */
void MainWindow::showStatusMessage(const QString &message)
{
    statusBar->setText(message);
}
