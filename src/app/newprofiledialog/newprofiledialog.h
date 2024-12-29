/**
 * SPDX-FileCopyrightText: 2021-2023 EasyCoding Team and contributors
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef NEWPROFILEDIALOG_H
#define NEWPROFILEDIALOG_H

/** @file app/newprofiledialog/newprofiledialog.h
 * This file contains NewProfileDialog class structure.
*/

#include <QDialog>

/**
 * UI namespace.
*/
namespace Ui
{
    /**
     * GUI application form elements.
    */
    class NewProfileDialog;
}

/**
 *
*/
class NewProfileDialog : public QDialog
{
    Q_OBJECT
public:
    /**
     * Main constructor of the NewProfileDialog class.
     * @param parent Parent widget.
    */
    explicit NewProfileDialog(QWidget *parent = nullptr);

    /**
     * Destructor of the NewProfileDialog class.
    */
    virtual ~NewProfileDialog();

protected slots:


private slots:


private:
    /**
     * Stores the GUI application form instance.
    */
    Ui::NewProfileDialog *ui;

};

#endif // MAINWINDOW_H
