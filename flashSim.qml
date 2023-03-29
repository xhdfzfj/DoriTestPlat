import QtQuick 2.15
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import xhd.controls.simflashddrawcontrol 1.0

Item
{
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

            ScrollView
            {
                topPadding: 8
                x:10

                ColumnLayout
                {
                    spacing: 8
                    RowLayout
                    {
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
                            text: "16"
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


    Component.onCompleted:
    {
//        console.log( "guiFlashSimGrid width:", guiFlashSimGrid.width )
//        console.log( "guiFlashSimGrid height:", guiFlashSimGrid.height )

        guiSimFlashControl.sub_QmlLoadered( guiSimFlashScrollBar );

    }
}
