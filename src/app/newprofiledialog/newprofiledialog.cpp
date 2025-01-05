/**
 * SPDX-FileCopyrightText: 2021-2023 EasyCoding Team and contributors
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
*/

/** @file app/newprofiledialog/newprofiledialog.cpp
 * This file contains implementation of the NewProfileDialog class.
*/

#include <QFile>
#include <QDir>

#include "newprofiledialog/newprofiledialog.h"
#include "ui_newprofiledialog.h"
#include "profiles/profiles.h"


const QString noneProfile = "NONE";

NewProfileDialog::NewProfileDialog(const TunedManager *tunedManager, QWidget *parent) :
    QDialog(parent)
    , ui(new Ui::NewProfileDialog)
    , tunedManager(tunedManager)
    , highlighter(nullptr)
{
    ui -> setupUi(this);

    ui -> knownProfilesComboBox -> addItem(noneProfile);

    const QStringList availableProfiles = tunedManager -> GetAvailableProfiles();
    ui -> knownProfilesComboBox -> addItems(availableProfiles);

    connect(ui -> knownProfilesComboBox, SIGNAL(activated(int)), this, SLOT(onKnownProfileSelectedEvent(int)));

    ui -> knownProfilesComboBox->setCurrentIndex(0);
    ui -> newProfileNameLineEdit -> setText("new-profile");

    ProfileHighlighter *highlighter = new ProfileHighlighter(ui -> newProfilePlainTextEdit->document());
}

NewProfileDialog::~NewProfileDialog()
{
    delete ui;

}

void NewProfileDialog::accept()
{
    const QString profileName = ui -> newProfileNameLineEdit -> text();
    const QString profileContents = ui -> newProfilePlainTextEdit -> toPlainText();

    // TODO: sanitize the input!

    if( !CreateNewProfile(profileName, profileContents) ) {
        return;
    }

    QDialog::accept();
}

void NewProfileDialog::reject()
{
    QDialog::reject();
}

void NewProfileDialog::onKnownProfileSelectedEvent(int index)
{
    const QString profileName = ui -> knownProfilesComboBox -> itemText(index);

    // 0 is the default NONE index
    if( 0 == index )
    {
        ui -> newProfileNameLineEdit -> setText("new-profile");
    }
    else
    {
        ui -> newProfileNameLineEdit -> setText("new-" + profileName);
    }

    const QString profileContents = readProfileContents(profileName);
    ui -> newProfilePlainTextEdit -> setPlainText(profileContents);
}

QString NewProfileDialog::readProfileContents(const QString& profileName)
{
    const QString systemProfilefileName = "/usr/lib/tuned/profiles/" + profileName + "/tuned.conf";
    const QString userProfileFileName = "/etc/tuned/profiles/" + profileName + "/tuned.conf";

    QString profileFileName = "";

    if( QFile::exists(systemProfilefileName) )
    {
        profileFileName = systemProfilefileName;
    }
    else if( QFile::exists(userProfileFileName) )
    {
        profileFileName = userProfileFileName;
    }

    if( profileFileName.isEmpty() ) {
        return "";
    }

    QFile file(profileFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return "";
    }

    QTextStream in(&file);
    QString profileContents = in.readAll();

    file.close();

    return profileContents;
}


