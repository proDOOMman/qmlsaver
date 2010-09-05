/* This file is part of the KDE libraries

    Copyright (c) 2001  Martin R. Jones <mjones@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include <QPainter>
#include <QTimer>
#include <QApplication>
#include <QX11Info>
#include "kscreensaver.h"
#ifdef Q_WS_X11
#include <X11/Xlib.h>
#else
typedef WId Window;
#endif

#undef KeyPress
#undef KeyRelease

//-----------------------------------------------------------------------------

class KScreenSaverPrivate
{
public:
    QWidget *owner;
};

KScreenSaver::KScreenSaver( WId id ) : QWidget()
{
    Window root;
    int ai;
    unsigned int au;
    unsigned int w = 0;
    unsigned int h = 0;

    d = new KScreenSaverPrivate;
    d->owner = find( id );
    if ( d->owner )
	installEventFilter( this );

    if ( id )
    {
        XGetGeometry(QX11Info::display(), (Drawable)id, &root, &ai, &ai,
            &w, &h, &au, &au);
        create( id, false, true );
    }

    if ( w == 0 ) w = 600;
    if ( h == 0 ) h = 420;
    resize( w, h );
    QPalette p(Qt::black,Qt::black,Qt::black,Qt::black,Qt::black,Qt::black,Qt::black,Qt::black,Qt::black);
    this->setPalette(p);
    this->setAutoFillBackground(true);
    QApplication::sendPostedEvents();
    show();
}

KScreenSaver::~KScreenSaver()
{
    destroy( false, false );
    delete d;
}

void KScreenSaver::embed( QWidget *w )
{
    QApplication::sendPostedEvents();
    XReparentWindow(QX11Info::display(), w->winId(), winId(), 0, 0);
    w->setGeometry( 0, 0, width(), height() );
    QApplication::sendPostedEvents();
}

bool KScreenSaver::eventFilter( QObject *o, QEvent *e )
{
    // make sure events get to the original window owner
    if ( d->owner && o == this ) {
	QApplication::sendEvent( d->owner, e );
	return false;
    }

    return QWidget::eventFilter( o, e );
}

//============================================================================

