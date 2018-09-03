#include "createdayplan.h"

using namespace UnitDeyPlan;

CreateDayPlan::CreateDayPlan(QWidget *parent,
                             const int index,
                             const QTime &time,
                             const int val ) : InterfaceForms(parent)
{
    lbText = new QLabel(QString::number(index)+": ",parent);
    timeEdit = new QTimeEdit(time,parent);
    value = new QSpinBox(parent);
    value->setRange(0,100); //100 %
    value->setValue(val);
    horizontalSpacer = new QSpacerItem(220, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QHBoxLayout*hLayout = new QHBoxLayout(parent);
    hLayout->addWidget(lbText);
    QLabel *text1 = new QLabel(tr("Time (h:m): "),parent);
    hLayout->addWidget(text1);
    hLayout->addWidget(timeEdit);
    QLabel *text3 = new QLabel(tr("Speaker  (%): "),parent);
    hLayout->addWidget(text3);
    hLayout->addWidget(value);
    hLayout->addSpacerItem(horizontalSpacer);
    this->setLayout(hLayout);
    this->setMinimumHeight(30);
}
/**
 * @brief CreateDayPlan::setNumberItem
 * @param number
 */
void CreateDayPlan::setNumberItem(const int number)
{
    lbText->setText(QString::number(number)+": ");
}
/**
 * @brief CreateDayPlan::getNumberItem
 * @return
 */
int CreateDayPlan::getNumberItem() const
{
    return lbText->text().toInt();
}
/**
 * @brief CreateDayPlan::setTime
 * @param date
 */
void CreateDayPlan::setTime(const QTime time)
{
    timeEdit->setTime(time);
}
/**
 * @brief CreateDayPlan::getTime
 * @return
 */
QTime CreateDayPlan::getTime() const
{
    return timeEdit->time();
}
/**
 * @brief CreateDayPlan::setValume
 * @param val1
 * @param val2
 */
void CreateDayPlan::setVolume(const int val)
{
    value->setValue(val);
}
/**
 * @brief CreateDayPlan::getValume
 * @param val1
 * @param val2
 */
void CreateDayPlan::getVolume(int * const val)
{
    *val = value->value();
}
