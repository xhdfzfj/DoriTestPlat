#ifndef GUIDRAWCONTROL_H
#define GUIDRAWCONTROL_H

#include <QObject>
#include <QQuickItem>
#include <QtQuick/QQuickPaintedItem>
#include <QPainter>
#include <QImage>
#include <QTimer>
#include <QThread>
#include <map>
#include "./HexDataClass.h"


class GuiDrawControl : public QQuickPaintedItem
{
    Q_OBJECT
public:
    GuiDrawControl( QQuickItem * pParent = nullptr );
    virtual ~GuiDrawControl();

    void sub_HexDataInput( uint8_t * pDataP, int pDataLen, int pStartOffset );
    void sub_ReDrawHexData();
    void sub_TestFunction();

    Q_INVOKABLE void sub_SizeChanage();
    Q_INVOKABLE void sub_ScrollBarChanage( real pPosition );


private:
    void sub_HexDataDraw( void );
    int fun_CalcLineDisplayCount( int pWidth, int pHexDataWidth, int pSpaceWidth );
    void sub_MergeHexDataS( void );
    int fun_CalcDisplayHeight( int pStrHeight, int _pLineByteS );
    void sub_DrawHexDataToImage( int pStrWidth, int pColonWidth, int pLineHeight, int pLineByteCount );
    QString fun_GetDataToHexDisplayStr( uint8_t * pDataP, int pOffset, int pLen );

signals:
    void sub_SignalReDrawSignal();

public slots:
    void sub_SlotReDraw();

    /*********************************
     * 系统重载
     * ******************************/
public:
    void paint( QPainter *painter ) override;

private:
    QThread * mGuiThreadP;

    QFont mFont;
    QImage * mMainImageP;

    std::map< int, HexDataClass * > mHexDataS;

    //由于调入时使用了std thread所以QT的一此东东无法使用
//    QTimer * mRedrawTimerP;
//    bool mRedrawTimerFlag;
};

#endif // GUIDRAWCONTROL_H
