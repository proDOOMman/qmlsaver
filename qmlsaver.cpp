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

#include <stdlib.h>
#include "qmlsaver.h"
#include "qmlapplicationview.h"
#include <QApplication>
#include <QSettings>

extern "C"
{
    const char *kss_applicationName = "qmlsaver.kss";
    const char *kss_version = "0.0.1";

    QMLSaver *kss_create( WId id )
    {
        return new QMLSaver( id );
    }
}

QMLSaver::QMLSaver( WId id ) : KScreenSaver( id )
{
    QSettings s("qmlsaver");
    qmlapp = new QmlApplicationView(s.value("dir","/usr/share/qmlsaver/qmls/segment-clock").toString()+"/main.qml",this);
    embed(qmlapp);
    qmlapp->resize(this->size());
    qmlapp->show();
}

QMLSaver::~QMLSaver()
{
    delete qmlapp;
}
