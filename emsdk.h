#ifndef EMSDK_H
#define EMSDK_H

#include <emscripten.h>
#include <QString>

class emsdk
{
public:
    emsdk();

    void setLocalStorage(const QString& key, const QString& value);
    QString getLocalStorage(const QString& key);
};

#endif // EMSDK_H
