#ifndef SIMFLASHDRAWCONTROL_H
#define SIMFLASHDRAWCONTROL_H

#include "./FlashSimClass.h"
#include <QQuickItem>
#include <QQuickPaintedItem>
#include <QPainter>
#include <QImage>

class SimFlashDrawControl : public QQuickPaintedItem, public FlashSimClass
{
    Q_OBJECT
public:
    SimFlashDrawControl( QQuickItem * pParent = nullptr );
    virtual ~SimFlashDrawControl();

    Q_INVOKABLE void sub_QmlLoadered();

private:
    void sub_DataToImage();
    int fun_CalcLineDisplayByteS( int pWidth, int pDataDisplayWidth, int pAscDisplayWidth );

    /*********************************
     * 系统重载
     * ******************************/
public:
    void paint( QPainter *painter ) override;

private:
    int mFontSize;
    QFont mFont;
    QColor mAddressTableFontColor;
    QColor mAddressTableBackColor;
    QColor mBackColor;

    int mCurrDisplayIndex;
    int mCurrDisplayEndIndex;
    int mLineByteS;

    QImage * mMainImageP;

signals:
    void sub_SignalReDraw();

public slots:
    void sub_SlotReDraw();

};

#endif // SIMFLASHDRAWCONTROL_H
