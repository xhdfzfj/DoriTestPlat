import QtQuick 2.15
import QtQuick.Controls 2.14
import xhd.controls.differentupdatecontrol 1.0

Item
{
    Timer
    {
        id:guiSizeChangeTimer
        repeat: false
        onTriggered:
        {
            console.log( "sizeChange!!!" );
            guiDifferentUpdataControl1.sub_SizeChange();
            guiDifferentUpdataControl2.sub_SizeChange();
        }
    }

    Grid
    {
        id:guiFlashSimGrid
        width: parent.width
        height: parent.height
        columnSpacing: 2
        columns: 3
        Rectangle
        {
            width: parent.width / 3
            height:parent.height
            Column
            {
                anchors.fill: parent
                Row
                {
                    Label
                    {
                        id:guiSrcDiffFile
                        text:"旧文件:"
                    }
                    Label
                    {
                        id:guiSrcDiffFilePath
                        text:"                "
                    }
                    Button
                    {
                        text:qsTr( "浏览" )
                        height: 20
                        onClicked:
                        {
                            diffrentGuiIndex = 1;
                            fds.open()
                        }
                    }
                }
                DifferentUpdataControl
                {
                    id:guiDifferentUpdataControl1
                    width: parent.width
                    height: parent.height - guiSrcDiffFile.height
                    fillColor: "lightsteelblue"

                    onWidthChanged:
                    {
                        console.log( "DifferentUpdataControl width:", width )
                        if( guiSizeChangeTimer.running )
                        {
                            guiSizeChangeTimer.stop();
                        }
                        guiSizeChangeTimer.interval = 3;
                        guiSizeChangeTimer.start();
                    }

                    onHeightChanged:
                    {
                        console.log( "DifferentUpdataControl height", height )
                        if( guiSizeChangeTimer.running )
                        {
                            guiSizeChangeTimer.stop();
                        }
                        guiSizeChangeTimer.interval = 3;
                        guiSizeChangeTimer.start();
                    }

                    MouseArea
                    {
                        anchors.fill: parent
                        propagateComposedEvents: true
                        acceptedButtons: Qt.AllButtons
                        onWheel:
                        {
                            if( wheel.angleDelta.y > 0 )
                            {
                                guiDifferentUpdataControl1.sub_WheelEvent( 1, 1 );
                            }
                            else
                            {
                                guiDifferentUpdataControl1.sub_WheelEvent( 0, 1 );
                            }
                        }
                    }
                }
            }
        }
        Rectangle
        {
            width: parent.width / 3
            height:parent.height
            Column
            {
                anchors.fill: parent
                Row
                {
                    Label
                    {
                        id:guiSrcNewFile
                        text:"新文件:"
                    }
                    Label
                    {
                        id:guiNewDiffFilePath
                        text:"                "
                    }
                    Button
                    {
                        text:qsTr( "浏览" )
                        height: 20
                        onClicked:
                        {
                            diffrentGuiIndex = 2;
                            fds.open()
                        }
                    }
                }
                DifferentUpdataControl
                {
                    id:guiDifferentUpdataControl2
                    width: parent.width
                    height: parent.height - guiSrcDiffFile.height
                    fillColor: "lightblue"

                    MouseArea
                    {
                        anchors.fill: parent
                        propagateComposedEvents: true
                        acceptedButtons: Qt.AllButtons
                        onWheel:
                        {
                            if( wheel.angleDelta.y > 0 )
                            {
                                guiDifferentUpdataControl2.sub_WheelEvent( 1, 1 );
                            }
                            else
                            {
                                guiDifferentUpdataControl2.sub_WheelEvent( 0, 1 );
                            }
                        }
                    }
                }
            }
        }
        Rectangle
        {
            width: parent.width / 3
            height:parent.height
            Column
            {
                anchors.fill: parent
                Row
                {
                    Label
                    {
                        id:guiResultDiffFile
                        text:"页大小为256"
                    }

                    Button
                    {
                        text:qsTr( "生成" )
                        height: 20
                        onClicked:
                        {
                            guiDifferentUpdataControl3.sub_CreateBsDiffFile();
                        }
                    }
                }
                DifferentUpdataControl
                {
                    id:guiDifferentUpdataControl3
                    width: parent.width
                    height: parent.height - guiSrcDiffFile.height
                    fillColor: "lightsteelblue"
                }
            }
        }
    }

    function sub_OpenFile(pIndex, pFileUrl, pFolder )
    {
        var _len
        var _len1
        if( pIndex === 1 )
        {
            _len = pFileUrl.toString().length
            _len1 = pFolder.toString().length

            console.log( pFileUrl.toString() )
            console.log( pFolder.toString() )

            guiSrcDiffFilePath.text = pFileUrl.toString().substring( _len1 )

            guiDifferentUpdataControl1.sub_DifferentFile( pFileUrl )
        }
        else if( pIndex === 2 )
        {
            _len = pFileUrl.toString().length
            _len1 = pFolder.toString().length

            console.log( pFileUrl.toString() )
            console.log( pFolder.toString() )

            guiNewDiffFilePath.text = pFileUrl.toString().substring( _len1 )

            guiDifferentUpdataControl2.sub_DifferentFile( pFileUrl )
        }
    }

    Component.onCompleted:
    {
        guiDifferentUpdataControl1.sub_DifferentType( 0 );
        guiDifferentUpdataControl2.sub_DifferentType( 1 );
        guiDifferentUpdataControl3.sub_DifferentType( 2 );
        guiDifferentUpdataControl1.sub_SetMainModelObj( MainModelObj );
        guiDifferentUpdataControl2.sub_SetMainModelObj( MainModelObj );
        guiDifferentUpdataControl3.sub_SetMainModelObj( MainModelObj );
    }
}
