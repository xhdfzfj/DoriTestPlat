import QtQuick 2.15

import QtQuick.Controls 2.14
import xhd.controls.guidrawcontrol 1.0

Rectangle
{
    anchors.fill: parent
    Timer
    {
        id:guiSizeChangeTimer
        repeat: false
        onTriggered:
        {
            console.log( "sizeChange!!!" );
            guiSpiDrawControl.sub_SizeChanage();
        }
    }

    Grid
    {
        id:guiSpiDataAnalyseGrid
        width: parent.width
        height: parent.height
        columnSpacing: 2
        columns: 2

        Rectangle
        {
            width: parent.width / 2
            height: parent.height

            TableView
            {
                id:guiSpiCmdInfoView
                clip:true
                anchors.fill: parent
                implicitHeight: 1024
                model:spiCmdViewModelInstance

                delegate:Rectangle
                {
                    color: model.row % 2 == 0 ? "blue" : "lightblue"
                    Text
                    {
                        anchors.fill: parent
                        anchors.margins: 5
                        color: 'black'
                        clip: true
                        font.pixelSize: 15
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.WordWrap
                        text: display
                        width: columnWidthProvider( model.col )
                    }
                }

//                Row
//                {
//                    id:guiSpiCmdInfoHeader
//                    y:guiSpiCmdInfoView.contentY
//                    z:2

//                    Repeater
//                    {
//                        model:guiSpiCmdInfoView.columns > 0 ? guiSpiCmdInfoView.columns : 1
//                        Label
//                        {
//                            width:guiSpiCmdInfoView.columnWidthProvider( modelData )
//                            height: 30
//                            text:spiCmdViewModelInstance.headerData( modelData, Qt.Horizontal )
//                            color:"white"
//                            //padding: 10
//                            verticalAlignment: Text.AlignVCenter
//                            horizontalAlignment: Text.AlignHCenter
//                            background: Rectangle { color: "blue" }
//                        }
//                    }
//                }

                columnWidthProvider:function( column )
                {
                    var _tmpWidth;
                    var _retWidth;
                    var _columnIndex;

                    _tmpWidth = parent.width;

                    _columnIndex = column;
                    if( _columnIndex === 0 )
                    {
                        _retWidth = _tmpWidth / 3 ;
                    }
                    else if( _columnIndex === 1 )
                    {
                        _retWidth = ( _tmpWidth / 3 ) * 2;
                    }
//                    else
//                    {
//                        _retWidth = ( _tmpWidth / 5 ) * 2;
//                    }

                    //console.log( "spi width:", _retWidth, " colum:", column );
                    return _retWidth;
                }

            }//TableView
        }

        Rectangle
        {
            width: parent.width / 2
            height: parent.height

            GuiDrawControl
            {
                id: guiSpiDrawControl
                width: parent.width - 16
                height: parent.height

                fillColor: "red"

                onWidthChanged:
                {
                    console.log( "DrawControl width:", width )
                    if( guiSizeChangeTimer.running )
                    {
                        guiSizeChangeTimer.stop();
                    }
                    guiSizeChangeTimer.interval = 3;
                    guiSizeChangeTimer.start();
                }

                onHeightChanged:
                {
                    console.log( "DrawControl height", height )
                    if( guiSizeChangeTimer.running )
                    {
                        guiSizeChangeTimer.stop();
                    }
                    guiSizeChangeTimer.interval = 3;
                    guiSizeChangeTimer.start();
                }
            }

            ScrollBar
            {
                id: verticalScrollBar
                width: 16;
                height: parent.height
                anchors.left: guiSpiDrawControl.right
                opacity: 1
                visible: true
                enabled: true
                orientation: Qt.Vertical

                onPositionChanged:
                {
                    console.log( "scrollbar ", position );
                    guiDrawControl.sub_ScrollBarChanage( position );
                }
            }

        }
    }

    Component.onCompleted:
    {
        MainModelObj.sub_GetDrawObjectFromQml( guiSpiDrawControl );
    }
}
