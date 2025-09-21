#include "emsdk.h"

emsdk::emsdk() {}


void emsdk::setLocalStorage(const QString &key, const QString &value){
    QString js = QString("localStorage.setItem('%1', '%2');").arg(key, value);
    emscripten_run_script(js.toUtf8().constData());
}

QString emsdk::getLocalStorage(const QString &key){
    QString js = QString("localStorage.getItem('%1');").arg(key);
    char* result = emscripten_run_script_string(js.toUtf8().constData());
    return QString(result);
}
