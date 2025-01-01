/**
 * SPDX-FileCopyrightText: 2021-2023 EasyCoding Team and contributors
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
*/

/** @file lib/policykit/policykit.cpp

*/

#include <unistd.h>

#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusMetaType>

#include "policykit/policykit.h"

bool CheckAuthorization()
{
    qDBusRegisterMetaType<QMap<QString, QString>>();

    const QString actionId = "org.freedesktop.policykit.exec";  // Example action ID
	const quint32 pid = getpid();
	const quint32 uid = getuid();

    // Create the method call message
    QDBusMessage message = QDBusMessage::createMethodCall(
        "org.freedesktop.PolicyKit1"
        , "/org/freedesktop/PolicyKit1/Authority"
        , "org.freedesktop.PolicyKit1.Authority"
        , "CheckAuthorization"
    );

	const quint64 startTime = 0;

    // Create PolkitSubject as a QDBusArgument (struct with a string and a dictionary of variants)
    QDBusArgument subjectArg;
    subjectArg.beginStructure();
    subjectArg << QString("unix-process"); //QString::number(getuid());  // User ID as string
	subjectArg << QMap<QString, QVariant>({{"pid", pid}, {"uid", uid}, {"start-time", startTime}});
    subjectArg.endStructure();

    const quint32 flags = 0x00000001;

    message << QVariant::fromValue(subjectArg)
            << actionId        // Action ID (string)
            << QVariant::fromValue(QMap<QString,QString>())          // Details (QVariantMap)
            << flags           // Flags (unsigned integer)
			<< ""
			;


    QDBusConnection theBus = QDBusConnection::systemBus();
    QDBusReply<QString> reply = theBus.call(message);

    if (reply.isValid()) {
        QString result = reply.value();
        if (result == "yes") {
            // std::cout << "User is authorized." << std::endl;
            return true;
        } else if (result == "no") {
            // std::cout << "User is not authorized." << std::endl;
        } else {
            // std::cout << "Authorization result is unknown." << std::endl;
        }
    } else {
        // std::cerr << "D-Bus call failed: " << reply.error().message().toStdString() << std::endl;
    }

    return false;
}

