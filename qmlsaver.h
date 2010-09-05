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

#ifndef QMLSAVER_H__
#define QMLSAVER_H__

#include "kscreensaver.h"

class QmlApplicationView;

class QMLSaver : public KScreenSaver
{
    Q_OBJECT
    QmlApplicationView *qmlapp;
public:
    QMLSaver( WId drawable );
    virtual ~QMLSaver();
};

#endif
