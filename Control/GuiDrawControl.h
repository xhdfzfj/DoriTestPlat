#ifndef GUIDRAWCONTROL_H
#define GUIDRAWCONTROL_H

#include <QObject>
#include <QQuickItem>
#include <QtQuick/QQuickPaintedItem>
#include <QPainter>
#include <QImage>
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
    void sub_MergeHexDataS( void );
    int fun_CalcDisplayHeight( int pStrHeight, int _pLineByteS );
    void sub_DrawHexDataToImage( int pStrWidth, int pColonWidth, int pLineHeight, int pLineByteCount );

    /*********************************
     * 系统重载
     * ******************************/
public:
    void paint( QPainter *painter ) override;

private:
    QFont mFont;
    QImage * mMainImageP;

    std::map< int, HexDataClass * > mHexDataS;
};

#endif // GUIDRAWCONTROL_H
