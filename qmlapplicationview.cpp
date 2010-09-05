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

#include "qmlapplicationview.h"

#include <QtXml>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtDeclarative/QDeclarativeComponent>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeContext>
#include <QSettings>

class QmlApplicationViewPrivate
{
    QString mainQmlFile;
    friend class QmlApplicationView;
};

QmlApplicationView::QmlApplicationView(const QString &mainQmlFile, QWidget *parent) :
#ifdef QMLINSPECTOR
    QmlViewer::QDeclarativeDesignView()
#else
    QDeclarativeView()
#endif
    , m_d(new QmlApplicationViewPrivate)
{
    this->setBackgroundBrush(Qt::black);
    m_d->mainQmlFile = mainQmlFile;
    net = new QNetworkAccessManager(this);
//    QNetworkDiskCache *diskCache = new QNetworkDiskCache(this);
//    diskCache->setCacheDirectory("~/.config/qmlsaver/cache/");
//    net->setCache(diskCache);
    updateSize(parent->size());
    loadSettings();
    addImportPath(QString(m_d->mainQmlFile).remove(QRegExp("main.qml$")));
//    setResizeMode(QDeclarativeView::SizeViewToRootObject);
    setSource(QUrl(m_d->mainQmlFile));
    connect(engine(), SIGNAL(quit()), SLOT(close()));
}

QmlApplicationView::~QmlApplicationView()
{
    delete m_d;
}

void QmlApplicationView::addImportPath(const QString &importPath)
{
    engine()->addImportPath(importPath);
}

void QmlApplicationView::updateSize(QSize size)
{
    rootContext()->setContextProperty("sceneWidth",size.width());
    rootContext()->setContextProperty("sceneHeight",size.height());
}

void QmlApplicationView::loadSettings()
{
    if(m_d->mainQmlFile.isEmpty())
        return;
    QSettings s("qmlsaver");
    s.beginGroup(m_d->mainQmlFile.toLocal8Bit().toPercentEncoding());
    QDomDocument doc;
    if(m_d->mainQmlFile.startsWith("file"))
    {
        QString dirname = QString(m_d->mainQmlFile).remove(QRegExp("^file://")).remove(QRegExp("/main.qml$"));
        if(!QFile::exists(dirname+"/settings.xml"))
            return;
        QFile f(dirname+"/settings.xml");
        if(!f.open(QIODevice::ReadOnly|QIODevice::Text))
            return;
        if(!doc.setContent(f.readAll()))
            return;
    }
    else
    {
        QNetworkReply *rep = net->get(QNetworkRequest(QUrl(QString(m_d->mainQmlFile).remove(QRegExp("/main.qml$"))+"/settings.xml")));
        while(!rep->isFinished())
        {
            rep->waitForReadyRead(100);
            qApp->processEvents();
        }
        doc.setContent(rep->readAll());
        rep->deleteLater();
    }
    QDomNode rootNode = doc.documentElement();
    QDomNode node = rootNode.firstChild();
    while(!node.isNull())
    {
        QDomElement elem = node.toElement();
        node = node.nextSibling();
        if(elem.tagName().compare("item"))
            continue;
        QString name = elem.attribute("name");
        if(name.isEmpty())
            continue;
        rootContext()->setContextProperty(name,s.value(name,elem.attribute("default","")));
    }
}
