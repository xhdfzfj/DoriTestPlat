#ifndef GUIDRAWCONTROL_H
#define GUIDRAWCONTROL_H

#include <QObject>
#include <QQuickItem>
#include <QtQuick/QQuickPaintedItem>
#include <QPainter>
#include <map>
#include "./HexDataClass.h"


class GuiDrawControl : public QQuickPaintedItem
{
    Q_OBJECT
public:
    GuiDrawControl( QQuickItem * pParent = nullptr );
    virtual ~GuiDrawControl();

    void sub_HexDataInput( uint8_t * pDataP, int pDataLen, int pStartOffset );

private:
    void sub_HexDataDraw( void );
    int fun_CalcLineDisplayCount( int pWidth, int pHexDataWidth, int pSpaceWidth );

    /*********************************
     * 系统重载
     * ******************************/
public:
    void paint( QPainter *painter ) override;

private:
    QFont mFont;

    std::map< int, HexDataClass * > mHexDataS;
};

#endif // GUIDRAWCONTROL_H
