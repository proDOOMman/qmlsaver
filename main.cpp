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
#include <stdio.h>
#include <signal.h>

#include <iostream>

#include <QTime>
#include <QFile>
#include <QDebug>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QX11Info>
#include <QApplication>
#include <QMessageBox>
#include "settingswidget.h"

#include "kscreensaver.h"
#include "kscreensaver_vroot.h"

#include "qmlsaver.h"

extern "C"
{
    extern const char *kss_applicationName;
    extern const char *kss_description;
    extern const char *kss_version;
    QMLSaver *kss_create( WId d );
}

//----------------------------------------------------------------------------

class DemoWindow : public QWidget
{
public:
    DemoWindow() : QWidget()
    {
	setFixedSize(600, 420);
    }

protected:
    virtual void keyPressEvent(QKeyEvent *e)
    {
        if (e->key() == Qt::Key_Q)
        {
		QApplication::instance()->quit();
        }
    }

    virtual void closeEvent( QCloseEvent * )
    {
	    QApplication::instance()->quit();
    }
};


//----------------------------------------------------------------------------
#if defined(Q_WS_QWS) || defined(Q_WS_MACX)
typedef WId Window;
#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    DemoWindow *demoWidget = 0;
    Window saveWin = 0;
    QMLSaver *target;

    for(int i = 0; i < QApplication::arguments().count(); i++) {
	    QString a = QApplication::arguments().at(i);
	    if(a == "--root" || a == "-root") {
		saveWin = RootWindow(QX11Info::display(), QX11Info::appScreen());
	    }
	    if(a == "--demo" || a == "-demo") {
		saveWin = 0;
	    }
	    if(a == "--window-id" || a == "-window-id") {
		if(i+1 >= QApplication::arguments().count()) {
		    std::cerr << "--window-id requires a parameter" << std::endl;
		    return 1;
		}
		else {
		    saveWin = QApplication::arguments().at(i+1).toInt();
		}
	    }
	    if(a == "--setup" || a == "-setup") {
		//QMessageBox::critical(0, "Setup not supported", "This version of QMLSaver doesn't have a setup dialog.");
		SettingsWidget *sw = new SettingsWidget();
		sw->show();
		return app.exec();
	    }
	    if(a == "--help" || a == "-help") {
		std::cerr << "options: [-setup] [-demo] [-root] [-window-id ID]" << std::endl;
		return 1;
	    }
    }

    // for gnome-screensaver (and xscreensaver?)
    bool ok = false;
    WId xsw = QByteArray(getenv("XSCREENSAVER_WINDOW")).toInt(&ok,16);
    if(ok)
        saveWin = xsw;
    //

    if (saveWin == 0)
    {
        demoWidget = new DemoWindow();
        demoWidget->setAutoFillBackground(false);
        saveWin = demoWidget->winId();
        app.processEvents();
    }
    else
        qApp->setOverrideCursor( QCursor( Qt::BlankCursor ) );

    target = kss_create( saveWin );

    if ( demoWidget )
    {
        demoWidget->setFixedSize( 600, 420 );
        demoWidget->show();
    }
    app.exec();

    delete target;
    delete demoWidget;

    return 0;
}

