
/*
 * qml-file.h
 * Copyright (C) 2012 Adrian Perez <aperez@igalia.com>
 *
 * Distributed under terms of the GPLv3 license.
 */

#ifndef __qml_file_h__
#define __qml_file_h__

#include <QObject>
#include <QFile>

class QmlFile: public QObject
{
    Q_OBJECT

public:
    explicit QmlFile(QObject *parent = 0);
    virtual ~QmlFile();

    Q_PROPERTY(QString path
               READ    path
               WRITE   setPath
               NOTIFY  pathChanged)

    Q_PROPERTY(QString content
               READ    read
               WRITE   write
               NOTIFY  contentChanged)

    Q_INVOKABLE bool exists() const {
        return _file.exists();
    }

    Q_INVOKABLE QString read();
    Q_INVOKABLE bool write(const QString& data);

    const QString path() const {
        return _file.fileName();
    }

public slots:
    void setPath(const QString& path) {
        _file.setFileName(path);
        emit pathChanged(path);
    }

signals:
    void contentChanged(const QString& newContent) const;
    void pathChanged(const QString& newPath) const;
    void error(const QString& message) const;

private:
    QFile _file;
};

#endif /* !__qml_file_h__ */

