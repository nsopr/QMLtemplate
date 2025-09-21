import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

Item {
    signal pushEditor()
    signal createAccount()
    signal logIn()
    property string buttonText: ""

    ColumnLayout{
        anchors.fill: parent
        Text {
            Layout.fillWidth: true
            text: "タイトル"
            color: "white"
            font.pointSize: Qt.application.font.pointSize * 3
            horizontalAlignment: Text.AlignHCenter
        }
        ListView{
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: ListModel{
                ListElement{a: "あああ"}
                ListElement{a: "bbb"}
            }
            delegate: ColumnLayout{

                spacing: 10
                Button{
                    width: parent.width
                    height: 10
                    text: model.a
                    onClicked: {
                        buttonText = text
                        pushEditor()
                    }
                }
            }
        }
    }
}


