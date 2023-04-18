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
            guiSimFlashControl1.sub_SizeChange();
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
                    }
                }
                DifferentUpdataControl
                {
                    id:guiSimFlashControl1
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
                    id:guiSimFlashControl2
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
                    id:guiSimFlashControl3
                    width: parent.width
                    height: parent.height - guiSrcDiffFile.height
                    fillColor: "lightsteelblue"
                }
            }
        }
    }

    Component.onCompleted:
    {

    }
}
