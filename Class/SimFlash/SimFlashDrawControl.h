#ifndef SIMFLASHDRAWCONTROL_H
#define SIMFLASHDRAWCONTROL_H

#include "./FlashSimClass.h"
#include <QQuickItem>
#include <QQuickPaintedItem>
#include <QPainter>
#include <QImage>
#include <QString>
#include "../TransionEffect.h"
#include "../../Model/MainModelClass.h"
#include "../MenuClass/PrivateMenuClass.h"

typedef struct __FlashModifyContent_s
{
    int mStartAddress;      //开始的地址一定是8字节对齐
    int mStartOffset;       //从8字节对齐后的偏移
    int mLineByteCount;     //每行选中的字节数
    int mLineCount;         //有几行选中
    bool mModifyFlag;       //是否修改标志
}FlashModifyContent_s;

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
    Q_INVOKABLE void sub_SetMainModelObj( QObject * pObjectP );
    Q_INVOKABLE int fun_GetModifyContentLineCount( void );
    Q_INVOKABLE QString fun_GetModifyAddress( int pIndex );
    Q_INVOKABLE QString fun_GetModifyContent( int pIndex );
    Q_INVOKABLE void sub_ModifyFlashContent( int pAddressValue, QString pContent );

    Q_PROPERTY( QString flashSize READ flashSize WRITE setFlashSize NOTIFY flashSizeChanaged )

    int fun_LowLevelInterface( void * );

    void hoverMoveEvent( QHoverEvent * event ) override;

private:
    void sub_DataToImage();
    int fun_CalcLineDisplayByteS( int pWidth, int pDataDisplayWidth, int pAscDisplayWidth );
    void sub_ChanageScrollValue();
    void sub_AdjustDragSelectRect();
    void sub_MenuItemHandle( int pIndex );
    void sub_SetFlashContent( QRect pSelectRect );
    void sub_DrawModifyContentImage( void );

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

    MainModelClass * mMainModleObjP;

    FlashModifyContent_s * mModifyContentP;

signals:
    void sub_SignalReDraw();
    void flashSizeChanaged();
    void flashContentModify();

public slots:
    void sub_SlotReDraw();

};

#endif // SIMFLASHDRAWCONTROL_H
