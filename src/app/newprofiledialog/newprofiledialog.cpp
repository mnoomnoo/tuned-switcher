/** @file app/newprofiledialog/newprofiledialog.cpp
 * This file contains implementation of the NewProfileDialog class.
*/

#include "newprofiledialog/newprofiledialog.h"
#include "ui_newprofiledialog.h"

NewProfileDialog::NewProfileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewProfileDialog)
{
    ui -> setupUi(this);

}

NewProfileDialog::~NewProfileDialog()
{
    delete ui;

}
