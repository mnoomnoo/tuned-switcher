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
#include <QActionGroup>

#include "tunedmanager/tunedmanager.h"
#include "profilesyntaxhighlighter.h"

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
    explicit NewProfileDialog(const TunedManager *tunedManager, QWidget *parent = nullptr);

    /**
     * Destructor of the NewProfileDialog class.
    */
    virtual ~NewProfileDialog();

public slots:
    virtual void accept();
    virtual void reject();

protected slots:


private slots:
    void onKnownProfileSelectedEvent(int index);

private:
    QString readProfileContents(const QString& profileName);

private:
    /**
     * Stores the GUI application form instance.
    */
    Ui::NewProfileDialog *ui;

    /**
     *
     */
    const TunedManager *tunedManager;

    /**
     *
     */
    ProfileHighlighter *highlighter;
};

#endif // MAINWINDOW_H
