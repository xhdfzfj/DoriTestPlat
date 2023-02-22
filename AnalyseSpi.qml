import QtQuick 2.14

Rectangle
{
    anchors.fill: parent

    Grid
    {
        id:guiSpiDataAnalyseGrid
        width: parent.width
        height: parent.height
        columns:2

        Rectangle
        {
            width: parent.width / 2
            height: parent.height
            color:"red"
        }

        Rectangle
        {

            width: parent.width / 2
            height: parent.height
            color:"yellow"
        }
    }
}
