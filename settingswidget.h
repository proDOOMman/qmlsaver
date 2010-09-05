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

#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include "ui_settingswidget.h"
#include "qmlapplicationview.h"
#include <QtCore>
#include <QtGui>
#include <QtXml>
#include <QLayout>
#include <QtNetwork>

struct SetItem{
    QString name;
    QString type;
    QString def;
    QString text;
    QWidget *widget;
};

class SettingsWidget : public QWidget, private Ui::SettingsWidget
{
    Q_OBJECT
    QmlApplicationView *qmlapp;
    QHash<QString, SetItem> settingsItems;
    QNetworkAccessManager *net;
public:
    explicit SettingsWidget(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);
private slots:
    void on_quirkList_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
    void updateList();
    void saveSettings(QListWidgetItem*);
    void loadSettings(QListWidgetItem*);
    void createSettings(QListWidgetItem*);
    void on_buttonBox_clicked(QAbstractButton* button);
    void setToolButtonColor();
    void setLineEditFile();
    void setSettings();
    void sceneResized(QSize s);
};

#endif // SETTINGSWIDGET_H
