import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.14
import QtQuick.Dialogs 1.3
//import Dori.TestPlat.ModelObject 1.0

Window {
    width: 1024
    height: 768
    visible: true
    title: qsTr("功能测试软件")

    onClosing:
    {
        MainModelObj.sub_ClearSelf();
    }

    property int workState: 0

    Grid
    {
        id:guiMainGrid
        anchors.fill: parent
        rows: 2 //行数
        spacing: 3
        Rectangle
        {
            width: parent.width
            height: 60
            //color:"red"

            Button
            {
                anchors.verticalCenter: parent.verticalCenter
                x:3
                width:80
                text:qsTr( "打开串口")
                id:guiOpenComBut

                onClicked:
                {
                    MainModelObj.sub_OpenSerialPortClick( this.text );
                    if( this.text === qsTr( "打开串口" ) )
                    {
                        this.text = qsTr( "关闭串口" )
                    }
                    else
                    {
                        this.text = qsTr( "打开串口" )
                    }
                }
            }

            Button
            {
                text:qsTr( "分析SPI数据" )
                anchors.top:guiOpenComBut.top
                anchors.left: guiOpenComBut.right
                anchors.leftMargin: 3
                width:100
                id:guiAnalyseSpiBut
                onClicked:
                {
                    onGuiAnalyseSpiClick()
                }
            }
        }

        Grid
        {
            id:guiDiplayAndInfoGrid
            width: parent.width
            height: parent.height - 60
            columns: 2

            //LOG显示部份
            Rectangle
            {
                width: parent.width / 3
                height: parent.height
                //color:"red"
                TableView
                {
                    id:guiLogView
                    objectName: "guiLogViewObj"
                    clip:true
                    anchors.fill: parent
                    topMargin: logColumnsHeader.implicitHeight + 2
                    model: logViewModelInstance

                    Row
                    {
                        id:logColumnsHeader
                        y:guiLogView.contentY
                        z:2
                        Repeater
                        {
                            model:guiLogView.columns > 0 ? guiLogView.columns : 1
                            Label
                            {
                                width:guiLogView.columnWidthProvider(modelData)
                                height:30
                                text: logViewModelInstance.headerData(modelData, Qt.Horizontal)
                                color: "white"
                                font.pixelSize: 15
                                padding: 10
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                background: Rectangle { color: "#333333" }
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
                            _retWidth = ( _tmpWidth / 5 ) * 2;
                        }
                        else if( _columnIndex === 1 )
                        {
                            _retWidth = ( _tmpWidth / 5 );
                        }
                        else
                        {
                            _retWidth = ( _tmpWidth / 5 ) * 2;
                        }

                        console.log( "width:", _retWidth, " colum:", column );
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
            }//Rectangle

            //根据不同功能的装载器
            Loader
            {
                id:guiLoader
                width: parent.width - parent.width / 3
                height: parent.height
            }

//            Rectangle
//            {
//                width: parent.width - parent.width / 4
//                height: parent.height
//                color: "yellow"
//            }
        }

    }

    FileDialog
    {
        id:fds
        title: "选择文件"
        folder: shortcuts.desktop
        selectExisting: true
        selectFolder: false
        selectMultiple: false
        //nameFilters: ["json文件 (*.json)"]
        onAccepted:
        {
            //labels.text = fds.fileUrl;
            console.log("You chose: " + fds.fileUrl);
            MainModelObj.sub_AnalyseSpiClick( fds.fileUrl );
        }

        onRejected:
        {
            //labels.text = "";
            console.log("Canceled");
            //Qt.quit();
        }
    }

    function onGuiAnalyseSpiClick()
    {
        if( workState !== 1 )
        {
            workState = 1;
            fds.open();
            guiLoader.source = "AnalyseSpi.qml";
        }
    }


}
