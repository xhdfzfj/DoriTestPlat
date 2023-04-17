import QtQuick 2.15
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import xhd.controls.simflashddrawcontrol 1.0
//import xhd.controls.simflaslistmodel 1.0

Item
{
    Connections
    {
        target:guiSimFlashControl
        onFlashContentModify:
        {
            var _tmpi
            var i
            var addressValue;
            var contentValue;

            guiModifyModel.clear();

            _tmpi = guiSimFlashControl.fun_GetModifyContentLineCount();

            for( i = 0; i < _tmpi; i++ )
            {
                addressValue = guiSimFlashControl.fun_GetModifyAddress( i );
                contentValue = guiSimFlashControl.fun_GetModifyContent( i );

                guiModifyModel.append( { "address":addressValue, "content":contentValue } );
            }
        }
    }

    ListModel
    {
        id:guiModifyModel
    }
    id:guiFlashSimMainItem
    anchors.fill: parent

    Grid
    {
        id:guiFlashSimGrid
        width: parent.width
        height: parent.height
        columnSpacing: 2
        columns: 2

        Rectangle
        {
            width: parent.width / 3
            height:parent.height
            color:"ghostwhite"
            id:guiMainRect

            ScrollView
            {
                topPadding: 8
                x:10

                ColumnLayout
                {
                    //spacing: 8
                    RowLayout
                    {
                        id:guiSimFlashRow
                        Label
                        {
                            font.bold: true
                            font.pixelSize: 22
                            text: "Flash大小(M):"
                            Layout.alignment: Qt.AlignCenter
                        }
                        TextField
                        {
                            id: guiFlashSimSize
                            Layout.alignment: Qt.AlignCenter
                            font.pixelSize: 22
                            text: guiSimFlashControl.flashSize
                            background: Rectangle
                            {
                                color:"ghostwhite"
                            }
                        }
                    }

                    RowLayout
                    {
                        Label
                        {
                            font.bold: true
                            font.pixelSize: 22
                            text: "显示地址:"
                            Layout.alignment: Qt.AlignCenter
                        }
                        TextField
                        {
                            Layout.alignment: Qt.AlignCenter
                            font.pixelSize: 22
                            text: "0x00000000"
                            background: Rectangle
                            {
                                color:"ghostwhite"
                            }
                        }
                    }

                    Rectangle
                    {

                        width:guiFlashSimMainItem.width

                        height:guiFlashSimMainItem.height - 8 - guiFlashSimSize.height - guiFlashSimSize.height - 50
                        ListView
                        {
                            id:guiModifyListView
                            anchors.fill: parent
                            //model:guiSimFlashControl.mSimFlashModifyModelObjP
                            //model:simFlashModifyModelInstance
                            model:guiModifyModel
                            delegate: Row
                            {
                                Label
                                {
                                    font.pixelSize: 16
                                    text: address
                                    anchors.verticalCenter: parent.verticalCenter
                                }

                                TextField
                                {
                                   font.pixelSize: 16
                                   text: content
                                   anchors.verticalCenter: parent.verticalCenter
                                   onAccepted:
                                   {
                                       var addressStr
                                       var addressValue
                                       console.log( "modify content enter key", model.index )

                                       addressStr = guiModifyModel.get( model.index ).address;
                                       addressStr = addressStr.substring( 0, addressStr.length - 1 )
                                       addressStr = "0x" + addressStr

                                       addressValue = Number( addressStr )
                                       console.log( "Address:", addressValue )
                                       console.log( "content:", guiModifyModel.get( model.index ).content, "   ", text )

                                       guiSimFlashControl.sub_ModifyFlashContent( addressValue, text );
                                   }
                                }
                            }
                        } //ListView
                    } //Rectangle

                }
            }
        }

        Rectangle
        {
            width: parent.width - ( parent.width / 3 ) - 16;
            height: parent.height
            SimFlashDrawControl
            {
                id:guiSimFlashControl
                width: parent.width
                height: parent.height
                fillColor: "red"

                MouseArea
                {
                    anchors.fill: parent
                    propagateComposedEvents:true
                    acceptedButtons: Qt.AllButtons
                    onWheel:
                    {
                        //if( wheel.modifiers && Qt.ControlModifier )
                        //{
                            if( wheel.angleDelta.y > 0 )
                            {
                                guiSimFlashControl.sub_WheelEvent( 1 );
                            }
                            else
                            {
                                guiSimFlashControl.sub_WheelEvent( 0 );
                            }
                        //}
                    }

                    onPositionChanged:
                    {
                        //console.log( "X:", mouse.x, "Y:", mouse.y )

                        guiSimFlashControl.sub_MouseDrag( mouse.x, mouse.y )

                    }

                    onClicked:
                    {
                        if( mouse.button === Qt.RightButton )
                        {
                            console.log( "mouse right button x:", mouse.x, " y:", mouse.y );
                            guiSimFlashControl.sub_MouseRightButtonClick( mouse.x, mouse.y )
                        }

                        if( mouse.button === Qt.LeftButton )
                        {
                            //guiSimFlashControl.sub_MouseLeftButtonClick( mouse.x, mouse.y )
                        }
                    }

                    onPressed:
                    {
                        if( mouse.button === Qt.LeftButton )
                        {
                            guiSimFlashControl.sub_MouseLeftButtonClick( mouse.x, mouse.y )
                        }
                    }

                    onReleased:
                    {
                        if( mouse.button === Qt.LeftButton )
                        {
                            console.log( "release" );
                            guiSimFlashControl.sub_MouseRelease();
                        }
                    }
                }
            }

            ScrollBar
            {
                id: guiSimFlashScrollBar
                width: 16;
                height: parent.height
                anchors.left: guiSimFlashControl.right
                opacity: 1
                visible: true
                enabled: true
                orientation: Qt.Vertical

                onPositionChanged:
                {
                    guiSimFlashControl.sub_ScrollBarChanage( position );
                }
            }
        }
    }

    function fun_FlashContentModify()
    {

    }

    Component.onCompleted:
    {
        //console.log( "guiFlashSimGrid width:", guiFlashSimMainItem.heigh )
        //console.log( "guiFlashSimGrid height:", guiFlashSimSize.height )

        guiSimFlashControl.sub_QmlLoadered( guiSimFlashScrollBar );
        guiSimFlashControl.sub_SetMainModelObj( MainModelObj );
        //testListView.model = guiSimFlashControl.fun_GetListModel();
        //MainModelObj.sub_TestSelf();

    }
}
