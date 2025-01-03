/**
 * SPDX-FileCopyrightText: 2021-2023 EasyCoding Team and contributors
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
*/

/** @file lib/policykit/policykit.cpp

*/

#include "profiles.h"

#include <QProcess>

bool CreateNewProfile(const QString& profileName, const QString& profileContent)
{
    const QString profileDir = "/etc/tuned/profiles/" + profileName;
    const QString profileConfFilename = profileDir + "/tuned.conf";

    const QString shellCmd =
        "mkdir " + profileDir +
        ";" + "touch " + profileConfFilename +
        ";" + "echo -e \"" + profileContent + "\" > " + profileConfFilename
        ;

    QProcess process;

    QStringList arguments;
    arguments << "bash";
    arguments << "-c";
    arguments << shellCmd;

    process.start("pkexec", arguments);

    // If msecs is -1, then waitForFinished will not time out.
    process.waitForFinished(-1);

    // man pkexec
    // PKEXEC(1)
    // If the calling process is not authorized or an authorization could not be obtained through
    // authentication or an error occured, pkexec exits with a return value of 127.
    return 127 != process.exitCode();
}