#ifndef DROPBOX_H
#define DROPBOX_H

#include <QObject>

#include "network.h"

class dropbox : public QObject
{
    Q_OBJECT
public:
    dropbox();

    network nw;

    QByteArray app_key, app_sec, ref_tok, access_token;

    void generate_token();
    void list_file_folder();

    void get_note_list();

signals:
    void access_token_ready();
};

#endif // DROPBOX_H
