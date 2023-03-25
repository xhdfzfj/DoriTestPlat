#ifndef SIMFLASHDRAWCONTROL_H
#define SIMFLASHDRAWCONTROL_H

#include "./FlashSimClass.h"
#include <QQuickItem>
#include <QQuickPaintedItem>

class SimFlashDrawControl : public QQuickPaintedItem, FlashSimClass
{
    Q_OBJECT
public:
    SimFlashDrawControl();

signals:

};

#endif // SIMFLASHDRAWCONTROL_H
