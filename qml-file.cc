/*
 * qml-file.cc
 * Copyright (C) 2012 Adrian Perez <aperez@igalia.com>
 *
 * Distributed under terms of the GPLv3 license.
 */

#include "qml-file.h"
#include <QTextStream>
#include <QFile>

static const QString noPathErrorString("No 'path' was set before attempting to read");
static const QString notExistsErrorString("File '%1' does not exist");
static const QString openForReadErrorString("File '%1' could not be opened for reading");
static const QString openForWriteErrorString("File '%1' could not be opened for writing");


QmlFile::QmlFile(QObject* parent)
    : QObject(parent)
    , _file()
{
}

QmlFile::~QmlFile()
{
}

QString QmlFile::read()
{
    const QString path(_file.fileName());

    if (path.isEmpty()) {
        emit error(noPathErrorString);
        return QString();
    }
    if (!_file.exists()) {
        emit error(notExistsErrorString.arg(path));
        return QString();
    }
    if (!_file.open(QFile::ReadOnly)) {
        emit error(openForReadErrorString.arg(path));
        return QString();
    }

    QTextStream stream(&_file);
    QString data = stream.readAll();
    _file.close();
    return data;
}

bool QmlFile::write(const QString& data)
{
    const QString path(_file.fileName());

    if (path.isEmpty()) {
        emit error(noPathErrorString);
        return false;
    }
    if (!_file.open(QFile::WriteOnly | QFile::Truncate)) {
        emit error(openForWriteErrorString.arg(path));
        return false;
    }

    QTextStream stream(&_file);
    stream << data;
    _file.close();
    emit contentChanged(data);
    return true;
}
