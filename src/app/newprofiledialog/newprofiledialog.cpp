/** @file app/newprofiledialog/newprofiledialog.cpp
 * This file contains implementation of the NewProfileDialog class.
*/

#include <QFile>

#include "newprofiledialog/newprofiledialog.h"
#include "ui_newprofiledialog.h"
#include "policykit/policykit.h"

const QString noneProfile = "NONE";

NewProfileDialog::NewProfileDialog(const TunedManager *tunedManager, QWidget *parent) :
    QDialog(parent)
    , ui(new Ui::NewProfileDialog)
    , tunedManager(tunedManager)
{
    ui -> setupUi(this);

    ui -> knownProfilesComboBox -> addItem(noneProfile);

    const QStringList availableProfiles = tunedManager -> GetAvailableProfiles();
    ui -> knownProfilesComboBox -> addItems(availableProfiles);

    connect(ui -> knownProfilesComboBox, SIGNAL(activated(int)), this, SLOT(onKnownProfileSelectedEvent(int)));

    ui -> knownProfilesComboBox->setCurrentIndex(0);
}

NewProfileDialog::~NewProfileDialog()
{
    delete ui;

}

void NewProfileDialog::accept()
{
    if( !CheckAuthorization() )
    {
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
    const QString text = ui -> knownProfilesComboBox -> itemText(index);
    const QString fileName = "/usr/lib/tuned/profiles/" + text + "/tuned.conf";

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);
    QString profileContents = in.readAll();

    file.close();

    ui -> newProfilePlainTextEdit -> setPlainText(profileContents);

}