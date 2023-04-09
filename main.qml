import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.14
import QtQuick.Dialogs 1.3
//import Dori.TestPlat.ModelObject 1.0

Window {
    width: 1920
    height: 1080
    visible: true
    title: qsTr("功能测试软件")
    id:guiMainWindow

    onWidthChanged:
    {
        console.log( "onWidthChanged:", guiMainWindow.width );
    }

    onHeightChanged:
    {
        console.log( "onHeightChanged:", guiMainWindow.height );
    }

    property int workState: 0   //1:spi分析 2:通用的内存显示
    property int serialAddIndex : 0

    Grid
    {
        id:guiMainGrid
        anchors.fill: parent
        rows: 3 //行数
        spacing: 3
        Rectangle
        {
            width: parent.width
            height: 60
            border.width: 2
            border.color:"darkgray"

            Button
            {
                text:qsTr( "分析SPI数据" )
                anchors.verticalCenter: parent.verticalCenter
                x:6
                width:100
                id:guiAnalyseSpiBut
                onClicked:
                {
                    onGuiAnalyseSpiClick()
                }
            }

            Button
            {
                text:qsTr( "启动串行FLASH" )
                anchors.top:guiAnalyseSpiBut.top
                anchors.left: guiAnalyseSpiBut.right
                anchors.leftMargin: 3
                width:130
                id:guiFlashSimBut
                onClicked:
                {
                    workState = 2
                    if( text === "启动串行FLASH" )
                    {
                        text = "停止串行FLASH";
                    }
                    else
                    {
                        text = "启动串行FLASH"
                    }
                    fun_LoaderSpiFlash();
                }
            }

            Button
            {
                id:guiAddComSetupBut
                text:qsTr( "新增串口设置" )
                anchors.top:guiFlashSimBut.top
                anchors.left: guiFlashSimBut.right
                anchors.leftMargin: 3
                width:120
                onClicked:
                {
                    serialAddIndex += 1;
                    if( serialAddIndex < listItems.length )
                    {
                        guiDynamicComSetupModel.append( {"_id": serialAddIndex + 1, "_text":listItems[ serialAddIndex ] } )
                    }
                }
            }

            Button
            {
                text:qsTr( "测试按键" )
                anchors.top:guiAddComSetupBut.top
                anchors.left: guiAddComSetupBut.right
                anchors.leftMargin: 30
                width:100
                id:guiTestBut
                onClicked:
                {
                    onTestButClick()

                }
            }
        }

        ScrollView
        {
            id:guiComSetup
            width: parent.width
            height: 160
            ListView
            {
                id:guiComSetupListView
                model:guiDynamicComSetupModel
                clip:true
                width:parent.width
                height:guiComSetupListView.contentHeight
                delegate:Row
                {
                    width:parent.width
                    height:50
                    leftPadding:5
                    Text
                    {
                        id:guiComSetupName
                        text:_text

                        anchors.verticalCenter: parent.verticalCenter
                    }

                    ComboBox
                    {
                        id:serialNumComboBox

                        width:120
                        model: serialConfigModelInstance.serialNumList
                        currentIndex: 1
                        onCurrentIndexChanged:
                        {
                            serialConfigModelInstance.onSerialComboBoxSelect(currentIndex)
                            if(currentIndex == 0)
                            {
                                currentIndex = 1;
                            }
                        }
                        Component.onCompleted:
                        {
                            onCurrentIndexChanged(currentIndex);
                        }
                    }

                    Text
                    {

                        text:qstr( "波特率:" )

                        anchors.verticalCenter: parent.verticalCenter
                    }

                    ComboBox
                    {
                        id:baudComboBox

                        width:120

                        model: serialConfigModelInstance.baudList
                        currentIndex:0
                        onCurrentIndexChanged: serialConfigModelInstance.onBaudComboBoxSelect(currentIndex)
                        Component.onCompleted:
                        {
                            onCurrentIndexChanged(currentIndex);
                        }
                    }
                    Button
                    {
                        text:serialConfigModelInstance.serialStatus === 0?qsTr( "打开串口" ):qsTr( "关闭串口" )
                        id:guiOpenComBut

                        onClicked:
                        {
                            MainModelObj.sub_OpenSerialPortClick( this.text );
                        }
                    }
                }

            }
        }

        Grid
        {
            id:guiDiplayAndInfoGrid
            width: parent.width
            height: parent.height - 200
            columns: 2

            //LOG显示部份
            Rectangle
            {
                width: parent.width / 3
                height: parent.height
                border.width: 2
                border.color: "darkslategray"
                //color:"red"
                TableView
                {
                    id:guiLogView
                    objectName: "guiLogViewObj"
                    clip:true
                    anchors.fill: parent
                    topMargin: logColumnsHeader.implicitHeight + 2
                    model: logViewModelInstance
                    implicitHeight: 1024

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
                onLoaded:
                {
                    console.log( "装载完成" );
                    if( workState === 1 )
                    {

                    }
                    else
                    {

                    }
                }
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

    function onTestButClick()
    {
        if( guiLoader.status === Loader.Ready )
        {
            //卸载被LOADER的元素
            guiLoader.active = false
        }

        if( workState === 1 )
        {
            //分析SPI文件状态
            workState = 0;  //回到初始状态
        }

        //if( workState === 1 )
        //{
        //    MainModelObj.sub_TestButClick();
        //}
    }

    function fun_LoaderSpiFlash()
    {
        if( guiLoader.status === Loader.Ready )
        {
            //卸载被LOADER的元素
            guiLoader.active = false
        }

        if( workState == 2 )
        {
            guiLoader.source = "flashSim.qml"
        }
    }

    property var listItems:
    [
        "模拟GPRS串口:",
        "模拟FLASH串口:",
    ]

    ListModel
    {
        id:guiDynamicComSetupModel
        ListElement
        {
            _id:1
            _text:"模拟GPRS串口:"
        }

    }

}
