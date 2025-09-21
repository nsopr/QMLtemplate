#ifndef FIREBASE_H
#define FIREBASE_H

#include <QObject>
#include <QtNetwork>

#include "emsdk.h"
#include "dropbox.h"
#include "network.h"

class firebase : public QObject
{
    Q_OBJECT
public:
    firebase();

    emsdk em;
    dropbox dp;
    network nw;

    QString key = "AIzaSyB6JDHVf0dt9utAlU3C5AitccUR9cpq5FM";
    QString prefix = "NSbb_";

    //user info
    bool is_mail_verified;
    QString email, username;
    QJsonObject user_group_info;

    //QML
    QStringListModel group_type, user_list;

    void get_public_data();
    void add_user_group_info(QString displayName, QString group);

    void refresh_refToken();
    void signIn_with_refToken();
    void send_verify_mail();

    void get_keys();

signals:
    void signUp_succeeded();
    void signIn_succeeded(QString email, QString username, QString group, bool is_mail_verified);
    void signUp_failed(QString err);
    void signIn_failed();
    void logIn_failed(QString err);

    void get_dropbox_key_finished();

public slots:
    void signUp_signIn(int mode, QString id, QString displayName, QString group, QString pass);
};

#endif // FIREBASE_H
