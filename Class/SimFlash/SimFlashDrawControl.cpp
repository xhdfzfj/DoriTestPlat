#include "SimFlashDrawControl.h"

SimFlashDrawControl::SimFlashDrawControl(QQuickItem * pParent) : QQuickPaintedItem( pParent )
{
    mFont = QFont( "SimSun", 16 );

    mAddressTableFontColor = Qt::white;
    mAddressTableBackColor = Qt::black;
    mMainImageP = nullptr;

    connect( this, SIGNAL( sub_SignalReDraw() ), this, SLOT( sub_SlotReDraw() ), Qt::QueuedConnection );

}


SimFlashDrawControl::~SimFlashDrawControl()
{

}

/**
 * @brief SimFlashDrawControl::sub_QmlLoadered
 *      Qml装载完成
 */
void SimFlashDrawControl::sub_QmlLoadered()
{
    qDebug() << "simflash width " << width();
    sub_DataToImage();
}

/**
 * @brief SimFlashDrawControl::sub_SlotReDraw
 */
void SimFlashDrawControl::sub_SlotReDraw()
{

}


/**
 * @brief SimFlashDrawControl::paint
 * @param pPainter
 */
void SimFlashDrawControl::paint( QPainter * pPainter )
{

}
