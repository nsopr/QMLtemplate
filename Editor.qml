import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

Item {
    property bool useStack: false
    property string input: ""
    property string select: ""
    signal textChange()
    signal popEditor()

    ColumnLayout{
        anchors.fill: parent
        Button{
            visible: useStack
            text: "戻る"
            onClicked: popEditor()
        }
        Text{
            Layout.fillWidth: true
            text: select
            color: "white"
            font.pointSize: Qt.application.font.pointSize * 2
            horizontalAlignment: Text.AlignHCenter
        }

        TextArea{
            Layout.fillWidth: true
            Layout.fillHeight: true
            text: input
            onTextChanged:{
                input = text
                textChange()
            }
        }
    }
}
