/*

    Copyright (c) 2010 by Stanislav (proDOOMman) Kosolapov <prodoomman@shell.tor.hu>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef QMLAPPLICATIONVIEW_H
#define QMLAPPLICATIONVIEW_H

#include <QtNetwork>

#ifdef QMLINSPECTOR
#include <qdeclarativedesignview.h>
class QmlApplicationView : public QmlViewer::QDeclarativeDesignView
#else // QMLINSPECTOR
#include <QtDeclarative/QDeclarativeView>
class QmlApplicationView : public QDeclarativeView
#endif // QMLINSPECTOR
{
    QNetworkAccessManager *net;
    Q_OBJECT
public:
    enum Orientation {
        LockPortrait,
        LockLandscape,
        Auto
    };

    explicit QmlApplicationView(const QString &mainQmlFile, QWidget *parent = 0);
    virtual ~QmlApplicationView();

    void addImportPath(const QString &importPath);

private:
    class QmlApplicationViewPrivate *m_d;
public slots:
    void updateSize(QSize size);
    void loadSettings();
};

#endif // QMLAPPLICATIONVIEW_H
