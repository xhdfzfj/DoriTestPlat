#ifndef SIMFLASHDRAWCONTROL_H
#define SIMFLASHDRAWCONTROL_H

#include "./FlashSimClass.h"
#include <QQuickItem>
#include <QQuickPaintedItem>
#include <QPainter>
#include <QImage>
#include <QString>
#include "../MenuClass/PrivateMenuClass.h"
#include "../TransionEffect.h"

class SimFlashDrawControl : public QQuickPaintedItem, public FlashSimClass
{
    Q_OBJECT
public:
    explicit SimFlashDrawControl( QQuickItem * pParent = nullptr );
    virtual ~SimFlashDrawControl();

    Q_INVOKABLE void sub_QmlLoadered( QObject * pObjectP );
    Q_INVOKABLE void sub_WheelEvent( int pDirect ); //0代表向下  1代表向上
    Q_INVOKABLE void sub_ScrollBarChanage( qreal pValue );
    Q_INVOKABLE void sub_MouseDrag( qreal pX, qreal pY );
    Q_INVOKABLE void sub_MouseRelease();
    Q_INVOKABLE void sub_MouseLeftButtonClick( qreal pX, qreal pY );
    Q_INVOKABLE void sub_MouseRightButtonClick( qreal pX, qreal pY );

    Q_PROPERTY( QString flashSize READ flashSize WRITE setFlashSize NOTIFY flashSizeChanaged )

    int fun_LowLevelInterface( void * );


    void hoverMoveEvent( QHoverEvent * event );


private:
    void sub_DataToImage();
    int fun_CalcLineDisplayByteS( int pWidth, int pDataDisplayWidth, int pAscDisplayWidth );
    void sub_ChanageScrollValue();
    void sub_AdjustDragSelectRect();

public:
    QString flashSize()
    {
        int _tmpValue;
        QString _retStr;

        _tmpValue = ( int )GetSimFlashSize();

        _retStr = QString::number( _tmpValue );
        return _retStr;
    }

    void setFlashSize( QString pValue )
    {
        int _tmpValue;

        _tmpValue = pValue.toInt();

        //
        //未编码
        //
        emit flashSizeChanaged();
    }

    /*********************************
     * 系统重载
     * ******************************/
public:
    void paint( QPainter *painter ) override;

private:
    const int mSpaceValue = 3;
    int mFontSize;
    QFont mFont;
    QColor mAddressTableFontColor;
    QColor mAddressTableBackColor;
    QColor mBackColor;
    int mFontHeight;
    int mDataFontWidth;
    int mColonFontWidth;
    int mSingleAscWidth;
    int mAddressStringWidth;

    int mCurrDisplayIndex;
    int mCurrDisplayEndIndex;
    int mLineByteS;

    QImage * mMainImageP;
    QObject * mScrollBarP;
    bool mStopScrollChanageFlag;

    bool mDragFlag;
    bool mDragSelectFlag;
    qreal mCurrMouseX;
    qreal mCurrMouseY;
    QImage * mSaveDragRectImageP;
    QRect mSaveDragRect;
    QRect mSelectRect;
    int mPrevDragWidth;

    TransionEffect * mEffectObjP;
    PrivateMenuClass * mContentMenuP;
    QRect * mContentItemRectP;
    int mContentItemRectCount;

signals:
    void sub_SignalReDraw();
    void flashSizeChanaged();

public slots:
    void sub_SlotReDraw();

};

#endif // SIMFLASHDRAWCONTROL_H
