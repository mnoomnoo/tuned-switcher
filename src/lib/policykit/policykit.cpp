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

/**
 * Structure for the result of calling CheckAuthorization.
 */
struct PolkitAuthorizationResult
{
    // TRUE if the given Subject is authorized for the given action.
    bool is_authorized;

    // TRUE if the given Subject could be authorized if more information was provided,
    // and CheckAuthorizationFlags.AllowUserInteraction wasn't passed or no suitable authentication agent was available.
    bool is_challenge;

    // Details for the result. Known key/value-pairs include polkit.temporary_authorization_id
    // (if the authorization is temporary, this is set to the opaque temporary authorization id),
    // polkit.retains_authorization_after_challenge (Set to a non-empty string if the authorization will be
    // retained after authentication (if is_challenge is TRUE)), polkit.dismissed (Set to a non-empty string if
    // the authentication dialog was dismissed by the user).
    QMap<QString, QString> details;

    PolkitAuthorizationResult() : is_authorized(false), is_challenge(false) {}

    /**
     * Operator << of the PolkitAuthorizationResult structure.
    */
    friend QDBusArgument &operator<<(QDBusArgument &argument, const PolkitAuthorizationResult &arg)
    {
        argument.beginStructure();
        argument << arg.is_authorized;
        argument << arg.is_challenge;
        argument << arg.details;
        argument.endStructure();

        return argument;
    }

    /**
     * Operator >> of the PolkitAuthorizationResult structure.
    */
    friend const QDBusArgument &operator>>(const QDBusArgument &argument, PolkitAuthorizationResult &arg)
    {
        argument.beginStructure();
        argument >> arg.is_authorized;
        argument >> arg.is_challenge;
        argument >> arg.details;
        argument.endStructure();

        return argument;
    }

};
Q_DECLARE_METATYPE(PolkitAuthorizationResult)


////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void RegisterMetaTypes()
{
    qRegisterMetaType<PolkitAuthorizationResult>("PolkitAuthorizationResult");
    qDBusRegisterMetaType<PolkitAuthorizationResult>();
    qDBusRegisterMetaType<QMap<QString, QString>>();
}

bool CheckAuthorization()
{
    const QString actionId = "org.freedesktop.policykit.exec";
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
    subjectArg << QString("unix-process");
	subjectArg << QMap<QString, QVariant>({{"pid", pid}, {"uid", uid}, {"start-time", startTime}});
    subjectArg.endStructure();

    const quint32 flags = 0x00000001;

    message << QVariant::fromValue(subjectArg)
            << actionId // Action ID (string)
            << QVariant::fromValue(QMap<QString,QString>()) // Details (QVariantMap)
            << flags // Flags (unsigned integer)
			<< ""
			;


    QDBusConnection theBus = QDBusConnection::systemBus();
    QDBusReply<PolkitAuthorizationResult> reply = theBus.call(message);

    if (reply.isValid()) {
        PolkitAuthorizationResult result = reply.value();
        if (result.is_authorized) {
            // std::cout << "User is authorized." << std::endl;
            return true;
        }
    } else {
        // std::cerr << "D-Bus call failed: " << reply.error().message().toStdString() << std::endl;
    }

    return false;
}

