import QtQuick 2.14
import QtQuick.Controls 2.14
import xhd.controls.guidrawcontrol 1.0

Rectangle
{
    anchors.fill: parent

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

                Row
                {
                    id:guiSpiCmdInfoHeader
                    y:guiSpiCmdInfoView.contentY
                    z:2

                    Repeater
                    {
                        model:guiSpiCmdInfoView.columns > 0 ? guiSpiCmdInfoView.columns : 1
                        Label
                        {
                            width:guiSpiCmdInfoView.columnWidthProvider( modelData )
                            height: 30
                            text:spiCmdViewModelInstance.headerData( modelData, Qt.Horizontal )
                            color:"white"
                            padding: 10
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            background: Rectangle { color: "blue" }
                        }
                    }
                }

                columnWidthProvider:function( column )
                {
                    var _tmpWidth;
                    var _retWidth;
                    var _columnIndex;

                    _tmpWidth = parent.width;

                    _columnIndex = column;
                    if( _columnIndex === 0 )
                    {
                        _retWidth = _tmpWidth / 5 ;
                    }
                    else if( _columnIndex === 1 )
                    {
                        _retWidth = ( _tmpWidth / 5 ) * 4;
                    }
//                    else
//                    {
//                        _retWidth = ( _tmpWidth / 5 ) * 2;
//                    }

                    //console.log( "width:", _retWidth, " colum:", column );
                    return _retWidth;
                }

                delegate:Rectangle
                {
                    color: model.row % 2 == 0 ? "grey" : "lightgrey"
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
                        anchors.topMargin: 3
                    }
                }
            }//TableView
        }

        GuiDrawControl
        {
            id: guiSpiDrawControl
            width: parent.width / 2
            height: parent.height

            fillColor: "red"
        }
    }

    Component.onCompleted:
    {
        MainModelObj.sub_GetDrawObjectFromQml( guiSpiDrawControl );
    }
}
