import QtQuick 2.15
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3

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
                anchors.leftMargin: 12

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
                            text: "Alert"
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
                            text: "0"
                        }
                    }
                }
            }
        }

        Rectangle
        {
            width: parent.width - ( parent.width / 3 )
            height: parent.height
            color: "red"
        }
    }

    Component.onCompleted:
    {
//        console.log( "guiFlashSimGrid width:", guiFlashSimGrid.width )
//        console.log( "guiFlashSimGrid height:", guiFlashSimGrid.height )
    }
}
