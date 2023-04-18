
#ifndef DIFFERENTUPDATACONTROL_H
#define DIFFERENTUPDATACONTROL_H

#include "../../Control/HexDataDisplayControl.h"

class DifferentUpdataControl : public HexDataDisplayControl
{
    Q_OBJECT
public:
    DifferentUpdataControl( QQuickItem * pParent = nullptr );
    virtual ~DifferentUpdataControl();
};

#endif // DIFFERENTUPDATACONTROL_H
