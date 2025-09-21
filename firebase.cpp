#include "firebase.h"

firebase::firebase() {
    refresh_refToken();

    get_public_data();

    connect(this, &firebase::signIn_succeeded, this, &firebase::get_keys);
    connect(this, &firebase::get_dropbox_key_finished, &dp, &dropbox::generate_token);
}

void firebase::get_public_data(){
    QNetworkReply *group_rep =
        nw.send_request("GET",
                        QUrl("https://firestore.googleapis.com/v1/projects/"
                        "ns-bulletin-board-e0e12/databases/(default)/documents/public/group_type"),
                        QStringList(), "");

    connect(group_rep, &QNetworkReply::finished, this, [this, group_rep]{
        QJsonObject jobj = QJsonDocument::fromJson(group_rep->readAll()).object();
        jobj = jobj["fields"].toObject();

        QStringList comp;
        for (auto key : jobj.keys()) {
            comp << jobj[key].toObject()["stringValue"].toString();
        }
        group_type.setStringList(comp);
    });

    QNetworkReply *user_rep =
        nw.send_request("GET",
                        QUrl("https://firestore.googleapis.com/v1/projects/"
                        "ns-bulletin-board-e0e12/databases/(default)/documents/public/user_group"),
                        QStringList(), "");

    connect(user_rep, &QNetworkReply::finished, this, [this, user_rep]{
        user_group_info = QJsonDocument::fromJson(user_rep->readAll()).object();
        QJsonObject jobj = user_group_info["fields"].toObject();
        user_list.setStringList(jobj.keys());
    });
}

void firebase::get_keys(){
    QUrl url;
    QStringList headers;
    headers << "Authorization, Bearer " + em.getLocalStorage(prefix + "idToken").toUtf8();

    //dropbox
    url = QUrl("https://firestore.googleapis.com/v1/projects/ns-bulletin-board-e0e12/databases/(default)/documents/API_Keys/Dropbox_API");

    QNetworkReply *dreply = nw.send_request("GET", url, headers, "");

    connect(dreply, &QNetworkReply::finished, this, [this, dreply]{
        QJsonObject jobj = QJsonDocument::fromJson(dreply->readAll()).object();

        jobj = jobj["fields"].toObject();

        dp.app_key = jobj["app_key"].toObject().value("stringValue").toString().toUtf8();
        dp.app_sec = jobj["app_sec"].toObject().value("stringValue").toString().toUtf8();
        dp.ref_tok = jobj["ref_tok"].toObject().value("stringValue").toString().toUtf8();

        emit get_dropbox_key_finished();
    });

    //LINE
    url = QUrl("https://firestore.googleapis.com/v1/projects/ns-bulletin-board-e0e12/databases/(default)/documents/API_Keys/Discord_Webhook");

    QNetworkReply *lreply = nw.send_request("GET", url, headers, "");

    connect(lreply, &QNetworkReply::finished, this, [this, lreply]{
        QJsonObject jobj = QJsonDocument::fromJson(lreply->readAll()).object();

        jobj = jobj["fields"].toObject();

        // Discord_Webhook = jobj["url"].toObject().value("stringValue").toString().toUtf8();
    });

    //Manager ID
    url = QUrl("https://firestore.googleapis.com/v1/projects/ns-bulletin-board-e0e12/databases/(default)/documents/API_Keys/manager_id");
    QNetworkReply *mreply = nw.send_request("GET", url, headers, "");

    connect(mreply, &QNetworkReply::finished, this, [this, mreply]{
        QJsonObject jobj = QJsonDocument::fromJson(mreply->readAll()).object();

        jobj = jobj["fields"].toObject();

        // Manager_id = jobj["id"].toObject().value("stringValue").toString().toUtf8();
        // Manager_name = jobj["name"].toObject().value("stringValue").toString().toUtf8();

    });
}


void firebase::refresh_refToken(){
    QUrl url("https://securetoken.googleapis.com/v1/token?key=" + key);
    QStringList headers;
    headers << "Content-Type,application/json";

    QJsonObject jobj;
    jobj["grant_type"] = "refresh_token";
    jobj["refresh_token"] = em.getLocalStorage(prefix + "refreshToken");

    QNetworkReply *reply = nw.send_request("POST", url, headers, QJsonDocument(jobj).toJson());

    connect(reply, &QNetworkReply::finished, this, [this, reply]{
        if(reply->error() != QNetworkReply::NoError) {
            emit signIn_failed();
            return;
        }
        QJsonObject jobj = QJsonDocument::fromJson(reply->readAll()).object();
        em.setLocalStorage(prefix + "idToken", jobj["id_token"].toString());
        em.setLocalStorage(prefix + "refreshToken", jobj["refresh_token"].toString());
        signIn_with_refToken();
    });
}

void firebase::signIn_with_refToken(){
    QUrl url("https://identitytoolkit.googleapis.com/v1/accounts:lookup?key=" + key);
    QStringList headers;
    headers << "Content-Type,application/json";
    QJsonObject jobj;
    jobj["idToken"] = em.getLocalStorage(prefix + "idToken");

    QNetworkReply *reply = nw.send_request("POST", url, headers, QJsonDocument(jobj).toJson());

    connect(reply, &QNetworkReply::finished, this, [this, reply]{
        QJsonObject jobj = QJsonDocument::fromJson(reply->readAll()).object();

        if(reply->error() == QNetworkReply::NoError){
            jobj = jobj["users"].toArray().at(0).toObject();
            is_mail_verified = jobj["emailVerified"].toBool();
            email            = jobj["email"].toString();
            username         = jobj["displayName"].toString();
            QString group    = user_group_info["fields"].toObject()[username].toObject()["stringValue"].toString();
            emit signIn_succeeded(email, username, group, is_mail_verified);
        }
        else{
            is_mail_verified = false;
            email            = "";
        }

        // if(!is_mail_verified){
        //     emit signIn_failed();
        // }
    });
}

void firebase::signUp_signIn(int mode, QString id, QString displayName, QString group, QString pass){
    QString UpIn = mode == 0 ? "signUp" : "signInWithPassword";
    QUrl url("https://identitytoolkit.googleapis.com/v1/accounts:" + UpIn + "?key=" + key);
    QStringList headers;
    headers << "Content-Type,application/json";

    QJsonObject jobj;
    jobj["email"] = id;
    jobj["displayName"] = displayName;
    jobj["password"] = pass;
    jobj["returnSecureToken"] = true;

    QNetworkReply *reply = nw.send_request("POST", url, headers, QJsonDocument(jobj).toJson());

    connect(reply, &QNetworkReply::finished, this, [this, reply, mode, displayName, group]{
        QJsonObject jobj = QJsonDocument::fromJson(reply->readAll()).object();

        if(reply->error() == QNetworkReply::NoError){
            em.setLocalStorage(prefix + "idToken", jobj["idToken"].toString());
            em.setLocalStorage(prefix + "refreshToken", jobj["refreshToken"].toString());
            if(mode == 0){
                emit signUp_succeeded();
                add_user_group_info(displayName, group);
                send_verify_mail();
            }
            else if(mode == 1)
                signIn_with_refToken();
        }
        else{
            if(mode == 0){
                emit signUp_failed(jobj["error"].toObject()["message"].toString());
            }
            else if(mode == 1)
                emit logIn_failed(jobj["error"].toObject()["message"].toString());
        }
    });
}

void firebase::add_user_group_info(QString displayName, QString group){
    QNetworkReply *reply =
        nw.send_request("GET",
                        QUrl("https://firestore.googleapis.com/v1/projects/"
                             "ns-bulletin-board-e0e12/databases/(default)/documents/public/user_group"),
                        QStringList(), "");

    connect(reply, &QNetworkReply::finished, this, [this, reply, displayName, group]{
        user_group_info    = QJsonDocument::fromJson(reply->readAll()).object();
        QJsonObject fields = user_group_info["fields"].toObject();
        QJsonObject item;
        item["stringValue"] = group;
        fields[displayName] = item;
        user_group_info["fields"] = fields;

        QStringList headers;
        headers << "Content-Type,application/json";

        nw.send_request("PATCH", reply->url(), headers, QJsonDocument(user_group_info).toJson());
    });
}

void firebase::send_verify_mail(){
    QUrl url("https://identitytoolkit.googleapis.com/v1/accounts:sendOobCode?key=" + key);
    QStringList headers;
    headers << "Content-Type,application/json";

    QJsonObject jobj;
    jobj["requestType"] = "VERIFY_EMAIL";
    jobj["idToken"] = em.getLocalStorage(prefix + "idToken");

    nw.send_request("POST", url, headers, QJsonDocument(jobj).toJson());
}
