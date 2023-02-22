import QtQuick 2.14
import xhd.controls.guidrawcontrol 1.0

Rectangle
{
    anchors.fill: parent

    Grid
    {
        id:guiSpiDataAnalyseGrid
        width: parent.width
        height: parent.height

        GuiDrawControl
        {
            id: guiSpiDrawControl
            width: parent.width
            height: parent.height

            fillColor: "red"
        }
    }

    Component.onCompleted:
    {
        MainModelObj.sub_GetDrawObjectFromQml( guiSpiDrawControl );
    }
}
