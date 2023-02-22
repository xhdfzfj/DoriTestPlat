#ifndef GUIDRAWCONTROL_H
#define GUIDRAWCONTROL_H

#include <QObject>
#include <QQuickItem>
#include <QtQuick/QQuickPaintedItem>
#include <QPainter>

class GuiDrawControl : public QQuickPaintedItem
{
    Q_OBJECT
public:
    GuiDrawControl( QQuickItem * pParent = nullptr );
    virtual ~GuiDrawControl();

    /*********************************
     * 系统重载
     * ******************************/
public:
    void paint( QPainter *painter ) override;
};

#endif // GUIDRAWCONTROL_H
