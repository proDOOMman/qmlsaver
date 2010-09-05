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

#include "settingswidget.h"
#include <QDeclarativeContext>

#define QMLHOST QString("http://qmlsaver.shell.tor.hu/")

SettingsWidget::SettingsWidget(QWidget *parent) :
    QWidget(parent){
    net = new QNetworkAccessManager(this);
    setupUi(this);
    qmlapp = new QmlApplicationView("",previewPlaceHolder);
    connect(qmlapp,SIGNAL(sceneResized(QSize)),this,SLOT(sceneResized(QSize)));
    emit updateList();
    ((QVBoxLayout*)previewPlaceHolder->layout())->addWidget(qmlapp);
}

void SettingsWidget::sceneResized(QSize s)
{
    qDebug() << s;
    qmlapp->updateSize(s);
}

void SettingsWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void SettingsWidget::updateList()
{
    QNetworkRequest req(QMLHOST+"list.php");
    QNetworkReply *rep = net->get(req);
    QByteArray result;
    while(!rep->isFinished())
    {
        rep->waitForReadyRead(100);
        qApp->processEvents();
    }
    result = rep->readAll();
    rep->deleteLater();
    QList<QString> urllist = QString(result).split('\n',QString::SkipEmptyParts);
    foreach(QString s,urllist)
    {
        QListWidgetItem *item = new QListWidgetItem(quirkList);
        item->setText("[web] "+s);
        item->setData(Qt::UserRole,QMLHOST+"qmls/"+s);
    }

    QSettings s("qmlsaver");
    QString qmldir = s.value("dir","").toString();
    QStringList dirs;
    dirs << "/usr/share/qmlsaver/qmls/" << QFileInfo(s.fileName()).absoluteDir().absolutePath()+"/qmlsaver/";
    foreach(QString s, dirs)
    {
        QDir d(s);
        if(!d.exists())
            continue;
        foreach(QFileInfo fi, d.entryInfoList(QDir::AllDirs|QDir::NoDotAndDotDot))
        {
            if(QDir(fi.absoluteFilePath()).exists("main.qml"))
            {
                QListWidgetItem *item = new QListWidgetItem(quirkList);
                item->setText("[local] "+fi.fileName());
                item->setData(Qt::UserRole,QUrl::fromLocalFile(fi.absoluteFilePath()));
                if(!fi.absoluteFilePath().compare(qmldir))
                    quirkList->setCurrentItem(item);
            }
        }
    }
}

void SettingsWidget::on_quirkList_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous)
{
    if(previous)
        saveSettings(previous);
    createSettings(current);
    loadSettings(current);
    qmlapp->setSource(current->data(Qt::UserRole).toUrl().toString()+"/main.qml");
    qmlapp->loadSettings();
}

void SettingsWidget::saveSettings(QListWidgetItem* it)
{
    QSettings s("qmlsaver");
    s.beginGroup((it->data(Qt::UserRole).toUrl().toString()+"/main.qml").toLocal8Bit().toPercentEncoding());
    foreach(SetItem item, settingsItems)
    {
        if(!item.type.compare("integer"))
        {
            s.setValue(item.name,qobject_cast<QSpinBox*>(item.widget)->value());
        }
        else if(!item.type.compare("float"))
        {
            s.setValue(item.name,qobject_cast<QDoubleSpinBox*>(item.widget)->value());
        }
        else  if(!item.type.compare("color"))
        {
            QPalette p = qobject_cast<QToolButton*>(item.widget)->palette();
            s.setValue(item.name,p.color(QPalette::Button));
        }
        else  if(!item.type.compare("file"))
        {
            QLineEdit *le = qobject_cast<QLineEdit*>(item.widget->layout()->itemAt(0)->widget());
//            QToolButton *tb = qobject_cast<QToolButton*>(item.widget->layout()->itemAt(1)->widget());d
            s.setValue(item.name,le->text());
        }
        else if(!item.type.compare("string"))
        {
            s.setValue(item.name,qobject_cast<QLineEdit*>(item.widget)->text());
        }
    }
}

void SettingsWidget::loadSettings(QListWidgetItem*)
{
    QSettings s("qmlsaver");
    s.beginGroup((quirkList->currentItem()->data(Qt::UserRole).toUrl().toString()+"/main.qml").toLocal8Bit().toPercentEncoding());
    foreach(SetItem item, settingsItems)
    {
        QVariant value = s.value(item.name,item.def);
        if(!item.type.compare("integer"))
        {
            (qobject_cast<QSpinBox*>(item.widget))->setValue(value.toInt());
        }
        else if(!item.type.compare("float"))
        {
            (qobject_cast<QDoubleSpinBox*>(item.widget))->setValue(value.toFloat());
        }
        else  if(!item.type.compare("color"))
        {
            QToolButton *pb = qobject_cast<QToolButton*>(item.widget);
            QPalette p = pb->palette();
            p.setColor(QPalette::Button,QColor(value.toString()));
            pb->setPalette(p);
        }
        else  if(!item.type.compare("file"))
        {
            QLineEdit *le = qobject_cast<QLineEdit*>(item.widget->layout()->itemAt(0)->widget());
//            QToolButton *tb = qobject_cast<QToolButton*>(item.widget->layout()->itemAt(1)->widget());d
            le->setText(value.toString());
        }
        else if(!item.type.compare("string"))
        {
            (qobject_cast<QLineEdit*>(item.widget))->setText(value.toString());
        }
    }
    setSettings();
}

void SettingsWidget::createSettings(QListWidgetItem* item)
{
    settingsItems.clear();
    QFormLayout *l = (QFormLayout*)settingsAreaContents->layout();
    for(int i = l->count()-1; i>=0; i--)
    {
        delete l->itemAt(i)->widget();
        l->removeItem(l->itemAt(i));
    }
    QUrl url = item->data(Qt::UserRole).toUrl();
    QDomDocument doc;
    if(!url.scheme().compare("file"))
    {
        //local file
        QString dirname = url.toString(QUrl::RemoveScheme);
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
        //network
        QNetworkReply* rep = net->get(QNetworkRequest(QUrl(url.toString()+"/settings.xml")));
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
        SetItem sItem;
        sItem.name = name;
        sItem.widget = 0;
        sItem.type = elem.attribute("type","string");
        sItem.def = elem.attribute("default","");
        sItem.text = elem.attribute(tr("trans_en","Attribute that contains localized string"),elem.text());
        if(!sItem.type.compare("integer"))
        {
            QSpinBox *sb = new QSpinBox();
            sb->setMaximum(99999);
            sb->setMinimum(-9999);
            sItem.widget = (QWidget*)sb;
            connect(sb,SIGNAL(valueChanged(int)),this,SLOT(setSettings()));
        }
        else if(!sItem.type.compare("float"))
        {
            QDoubleSpinBox *sb = new QDoubleSpinBox();
            sb->setMaximum(9999);
            sb->setMinimum(-9999);
            sItem.widget = (QWidget*)sb;
            connect(sb,SIGNAL(valueChanged(double)),this,SLOT(setSettings()));
        }
        else  if(!sItem.type.compare("color"))
        {
            QToolButton *tb = new QToolButton();
            tb->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
            tb->setAutoRaise(true);
            tb->setAutoFillBackground(true);
            connect(tb,SIGNAL(released()),this,SLOT(setToolButtonColor()));
            sItem.widget = tb;
        }
        else  if(!sItem.type.compare("file"))
        {
            sItem.widget = new QWidget();
            sItem.widget->setLayout(new QHBoxLayout());
            sItem.widget->layout()->addWidget(new QLineEdit());
            QToolButton *tb = new QToolButton(sItem.widget);
            connect(tb,SIGNAL(released()),this,SLOT(setLineEditFile()));
            tb->setText("...");
            sItem.widget->layout()->addWidget(tb);
        }
        else if(!sItem.type.compare("string"))
        {
            QLineEdit *le = new QLineEdit();
            sItem.widget = le;
            connect(le,SIGNAL(textChanged(QString)),this,SLOT(setSettings()));
        }
        if(!sItem.widget)
            continue;
        l->addRow(sItem.text,sItem.widget);
        settingsItems[name] = sItem;
    }
}

void SettingsWidget::on_buttonBox_clicked(QAbstractButton* button)
{
    QSettings s("qmlsaver");
    switch(buttonBox->standardButton(button))
    {
    case QDialogButtonBox::Save:
        s.setValue("dir",quirkList->currentItem()->data(Qt::UserRole).toUrl().toString());
        saveSettings(quirkList->currentItem());
        qApp->exit(0);
        break;
    case QDialogButtonBox::Discard:
        qApp->exit(0);
        break;
    case QDialogButtonBox::Reset:
        foreach(SetItem item, settingsItems)
        {
            QVariant value = item.def;
            if(!item.type.compare("integer"))
            {
                (qobject_cast<QSpinBox*>(item.widget))->setValue(value.toInt());
            }
            else if(!item.type.compare("float"))
            {
                (qobject_cast<QDoubleSpinBox*>(item.widget))->setValue(value.toFloat());
            }
            else  if(!item.type.compare("color"))
            {
                QToolButton *pb = qobject_cast<QToolButton*>(item.widget);
                QPalette p = pb->palette();
                p.setColor(QPalette::Button,QColor(value.toString()));
                pb->setPalette(p);
            }
            else  if(!item.type.compare("file"))
            {
                QLineEdit *le = qobject_cast<QLineEdit*>(item.widget->layout()->itemAt(0)->widget());
                le->setText(value.toString());
            }
            else if(!item.type.compare("string"))
            {
                (qobject_cast<QLineEdit*>(item.widget))->setText(value.toString());
            }
        }
        setSettings();
        break;
    default:
        break;
    }
}

void SettingsWidget::setToolButtonColor()
{
    QToolButton *tb = qobject_cast<QToolButton*>(sender());
    QPalette p = tb->palette();
    p.setColor(QPalette::Button,QColorDialog::getColor(p.color(QPalette::Button),
                                                       this,
                                                       tr("Select color"),QColorDialog::ShowAlphaChannel));
    tb->setPalette(p);
    setSettings();
}

void SettingsWidget::setLineEditFile()
{
    QWidget *w = qobject_cast<QWidget*>(sender()->parent());
    QLineEdit *le = qobject_cast<QLineEdit*>(w->layout()->itemAt(0)->widget());
    le->setText("file://"+QFileDialog::getOpenFileName(this,tr("Select file"),QFileInfo(le->text()).absoluteDir().path()));
    setSettings();
}

void SettingsWidget::setSettings()
{
    foreach(SetItem item, settingsItems)
    {
        if(!item.type.compare("integer"))
        {
            qmlapp->rootContext()->setContextProperty(item.name,qobject_cast<QSpinBox*>(item.widget)->value());
        }
        else if(!item.type.compare("float"))
        {
            qmlapp->rootContext()->setContextProperty(item.name,qobject_cast<QDoubleSpinBox*>(item.widget)->value());
        }
        else  if(!item.type.compare("color"))
        {
            QPalette p = qobject_cast<QToolButton*>(item.widget)->palette();
            qmlapp->rootContext()->setContextProperty(item.name,p.color(QPalette::Button));
        }
        else  if(!item.type.compare("file"))
        {
            QLineEdit *le = qobject_cast<QLineEdit*>(item.widget->layout()->itemAt(0)->widget());
            qmlapp->rootContext()->setContextProperty(item.name,le->text());
        }
        else if(!item.type.compare("string"))
        {
            qmlapp->rootContext()->setContextProperty(item.name,qobject_cast<QLineEdit*>(item.widget)->text());
        }
    }
}
