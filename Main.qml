import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

ApplicationWindow {
    id: root
    visible: true
    width: 640
    height: 480
    Material.theme:  Material.Dark
    Material.accent: Material.DeepPurple

    property bool useStack: width < height
    property string keepText: ""
    property string keepTitle: ""
    property bool  isMailVerified: false

    //get font from URL
    FontLoader {
        id: jpFont
        source: "https://nsopr.github.io/todo/Meiryo-01.ttf"
    }
    font.family: jpFont.name


    menuBar: MenuBar{
        Material.background: "transparent"
        Menu{
            title: "メニュー"
            Action{
                text: "アカウント作成"
                onTriggered: dialogLib.item.openSignUp()
            }
            Action{
                text: "ログイン"
                onTriggered: dialogLib.item.openLogin()
            }
            Menu{
                id: accountStatus
                title: "現在のアカウント"
                width: currentIDLength.width * 3 / 2
                MenuItem{
                    id: currentID
                    TextMetrics{
                        id: currentIDLength
                        text: currentID.text
                        font: currentID.font
                    }
                    enabled: false
                }
                MenuItem{id: mailVerified}
                MenuItem{id: currentDisplayName}
                MenuItem{id: currentGroup}
            }
        }
    }

    Loader{
        // anchors.fill: parent
        anchors.centerIn: parent
        width: parent.width * 0.95
        height: parent.height * 0.95
        active: true
        sourceComponent: useStack ? stack : bundle
    }

    Loader{
        id: dialogLib
        anchors.centerIn: parent
        width: parent.width * 0.7
        source: "Dlg.qml"
    }

    Connections{
        target: firebase
        function onSignIn_failed(){
            dialogLib.item.openLogin()
        }
        function onSignIn_succeeded(email, username, group, is_mail_verified){
            currentID.text = email
            isMailVerified = is_mail_verified
            var status = isMailVerified ? "完了":"未完了"
            mailVerified.text = "本人確認:\t" + status
            currentDisplayName.text = "表示名:\t" + username
            currentGroup.text = "所属:\t" + group
        }
    }

    Component{
        id:bundle
        RowLayout{
            List{
                id: list
                Layout.fillWidth: true
                Layout.fillHeight: true
                onPushEditor: keepTitle = buttonText
                onCreateAccount: dialogLib.item.openSignUp()
                onLogIn: dialogLib.item.openLogin()
            }
            Editor{
                id: editor
                useStack: useStack
                input: keepText
                select: keepTitle
                onTextChange: keepText = input
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }

    Component{
        id: stack
        StackView{
            id: stv
            initialItem: List{
                id: list
                visible: false
                onPushEditor: {
                    keepTitle = buttonText
                    stv.push(editor)
                }
                onCreateAccount: dialogLib.item.openSignUp()
                onLogIn: dialogLib.item.openLogin()

            }
            Editor{
                id: editor
                visible: false
                useStack: parent.width < parent.height
                input: keepText
                select: keepTitle
                onTextChange: keepText = input
                onPopEditor: stv.pop()
            }
        }
    }
}
