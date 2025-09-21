import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

Item {
    id: root

    function openLogin(){ loginDialog.open() }
    function openSignUp() { signUpDialog.open() }

    property bool dupl: true
    property bool selectCombo: false

    Connections{
        target: firebase
        function onSignUp_succeeded(){
            signUpDialog.close()
        }
        function onSignUp_failed(err){
            signUpErr.text = "アカウント作成失敗: " + err
        }

        function onSignIn_succeeded(){
            loginDialog.close()
        }
        function onLogIn_failed(err){
            loginErr.text = "ログイン失敗: " + err
        }
    }

    Dialog{
        id: signUpDialog
        anchors.centerIn: parent
        width: parent.width * 0.7
        modal: true
        closePolicy: "NoAutoClose"
        font.family: "Meiryo"
        onOpened: {
            signUpId.text = ""
            displayName.text = ""
            group.currentText = ""
            signUpPass.text = ""
            signUpErr.text = ""
        }
        ColumnLayout{
            width: parent.width
            spacing: 10
            Label{
                Layout.fillWidth: true
                text: "アカウント作成"
                color: "white"
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: signUpDialog.font.pointSize * 2
            }
            TextField{
                id: signUpId
                Layout.fillWidth: true
                placeholderText: "ID(メールアドレス)"
            }
            TextField{
                id: displayName
                Layout.fillWidth: true
                placeholderText: "表示名"
                onTextChanged: {
                    var indexOf = -1
                    for(var i = 0; i < user_list.rowCount(); i++){
                        var item = user_list.data(user_list.index(i,0), Qt.DisplayRole)
                        if(displayName.text === item){
                            indexOf = i
                            break
                        }
                    }
                    if(indexOf !== -1){
                        duplicateName.text = "その名前は既に使用されています"
                        dupl = true
                    }
                    else{
                        duplicateName.text = ""
                        dupl = false
                    }
                }
            }
            Label{
                id: duplicateName
                text:""
                Material.foreground: Material.Red
            }

            RowLayout{
                Label{
                    text: "あなたの所属"
                    color: "white"
                }

                ComboBox{
                    id: group
                    model: group_type
                    textRole: "display"
                    Layout.preferredWidth: parent.width * 0.2
                    onCurrentTextChanged: selectCombo = true
                }
                Item {
                    Layout.fillWidth: true
                }
            }

            TextField{
                id: signUpPass
                Layout.fillWidth: true
                placeholderText: "パスワード"
                echoMode: "Password"
            }
            Label{
                text: "※作成が正常に完了すると、本人確認のメールが届きます\nメール不通の場合、再度アカウント作成を試してください"
                color: "white"
                Layout.fillWidth: true
            }
            RowLayout{
                Item{
                    Layout.fillWidth: true
                }
                Label{
                    id: signUpErr
                    text: ""
                    Material.foreground: Material.Red
                }
                Item {
                    Layout.fillWidth: true
                }

                DialogButtonBox{
                    standardButtons: Dialog.Cancel
                    onRejected: signUpDialog.close()
                }
                DialogButtonBox{
                    standardButtons: Dialog.Ok
                    enabled: !dupl & selectCombo
                    onAccepted: {
                        firebase.signUp_signIn(0, signUpId.text, displayName.text, group.currentText, signUpPass.text)
                    }
                }
            }
        }


    }

    Dialog{
        id: loginDialog
        anchors.centerIn: parent
        width: parent.width * 0.7
        modal: true
        closePolicy: "NoAutoClose"
        ColumnLayout{
            width: parent.width
            spacing: 10
            Label{
                Layout.fillWidth: true
                text: "ログイン"
                color: "white"
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: loginDialog.font.pointSize * 2
            }
            TextField{
                id: loginId
                Layout.fillWidth: true
                placeholderText: "ID(メールアドレス)"
            }
            TextField{
                id: loginPass
                Layout.fillWidth: true
                placeholderText: "パスワード"
                echoMode: "Password"
            }
            Label{
                id: loginErr
                text: ""
                Material.foreground: Material.Red
            }
            RowLayout{
                Item{
                    Layout.fillWidth: true
                }
                DialogButtonBox{
                    standardButtons: Dialog.Ok|Dialog.Cancel
                    onAccepted: {
                        firebase.signUp_signIn(1, loginId.text, "", "", loginPass.text)
                    }
                    onRejected: loginDialog.close()
                }
            }
        }
    }

}
