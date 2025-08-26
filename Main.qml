import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    Material.theme:  Material.Dark
    Material.accent: Material.DeepPurple

    //get font from URL
    FontLoader {
        id: jpFont
        source: "https://username.github.io/myapp/fonts/BIZ-UDGothic-01.ttf"
    }

    ColumnLayout{
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
