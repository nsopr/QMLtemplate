import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    Material.theme:  Material.Dark
    Material.accent: Material.DeepPurple


    Column{
        anchors.centerIn: parent
        Button {
            text: "Hello QML"
        }

        TextField {
            placeholderText: "Enter your name"
            width: parent.width * 0.8
        }
    }
}
