#ifndef CREATEDAYPLAN_H
#define CREATEDAYPLAN_H

#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTimeEdit>
#include "InterfaceForms.h"

namespace UnitDeyPlan{
    class CreateDayPlan : public InterfaceForms
    {
        Q_OBJECT
        QLabel *lbText;
        QTimeEdit *timeEdit;
        QSpinBox *value;
        QSpacerItem *horizontalSpacer;

    public:
        explicit CreateDayPlan(QWidget *parent = nullptr,
                               const int index = 1,
                               const QTime &time = QTime::currentTime(),
                               const int val = 70 );
        void setNumberItem( const int number );
        int getNumberItem( void ) const;
        void setTime(const QTime time );
        QTime getTime( void ) const;
        void setVolume( const int val );
        void getVolume( int *const val );
    signals:
    public slots:
    };
}

#endif // CREATEDAYPLAN_H
