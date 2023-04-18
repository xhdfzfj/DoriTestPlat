
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

protected:
    const int mSpaceValue = 3;
    int mFontSize;
    QFont mFont;
    QColor mFontColor;
    QColor mBackColor;
    int mFontHeight;
    int mDataFontWidth;
    int mColonFontWidth;
    int mSingleAscWidth;
    int mAddressStringWidth;
    int mLineByteCount;

protected:
    void sub_DataToImage();

    /*********************************
     * 系统重载
     * ******************************/
public:
    void paint( QPainter *painter ) override;
};

#endif // HEXDATADISPLAYCONTROL_H
