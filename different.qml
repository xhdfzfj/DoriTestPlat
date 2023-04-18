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
                    }
                }
                DifferentUpdataControl
                {
                    id:guiDifferentUpdataControl2
                    width: parent.width
                    height: parent.height - guiSrcDiffFile.height
                    fillColor: "lightblue"
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
                        text:"结果文件:"
                    }
                    Label
                    {
                        id:guiResultDiffFilePath
                        text:"                "
                    }
                    Button
                    {
                        text:qsTr( "浏览" )
                        height: 20
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

    function sub_OpenFile(pIndex, pFileUrl)
    {
        if( pIndex === 1 )
        {
            guiDifferentUpdataControl1.sub_DifferentFile( pFileUrl )
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
