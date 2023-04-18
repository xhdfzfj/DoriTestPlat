
#ifndef HEXDATADISPLAYCONTROL_H
#define HEXDATADISPLAYCONTROL_H

#include <QObject>
#include <QQuickItem>
#include <QtQuick/QQuickPaintedItem>
#include <QPainter>
#include <QImage>


class HexDataDisplayControl : public QQuickPaintedItem
{
    Q_OBJECT
public:
    Q_INVOKABLE void sub_SizeChange();

public:
    HexDataDisplayControl( QQuickItem * pParent = nullptr );
    virtual ~HexDataDisplayControl();

private:
    QImage * mMainImageP;

    /*********************************
     * 系统重载
     * ******************************/
public:
    void paint( QPainter *painter ) override;
};

#endif // HEXDATADISPLAYCONTROL_H
