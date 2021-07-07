/**
 * SPDX-FileCopyrightText: 2021 EasyCoding Team and contributors
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
*/

/** @file lib/notificationsmanager/notificationsmanager.cpp
 * This file contains implementation of the NotificationsManager class.
*/

#include "notificationsmanager/notificationsmanager.h"
#include "notificationsmanager/dbusconverters.h"

void NotificationsManager::GetCapabilities()
{
    QDBusInterface DBusInterface(NotifyBusName, NotifyBusPath, NotifyBusInterface, DBusInstance);
    QDBusReply<QStringList> DBusReply = DBusInterface.call(NotifyBusMethodNameCapabilities);
    Capabilities = DBusReply.value();
}

bool NotificationsManager::IsMarkupSupported()
{
    return Capabilities.contains("body-markup");
}

bool NotificationsManager::IsImagesSupported()
{
    return Capabilities.contains("body-images");
}

QImage NotificationsManager::GetNotificationImage(const int size) const
{
    QIcon icon = QIcon::fromTheme(AppConstants::DomainSchemeName, QIcon(":/icons/fallback.png"));
    QPixmap pixmap = icon.pixmap(icon.actualSize(QSize(size, size)));
    return pixmap.toImage();
}

QVariantMap NotificationsManager::CreateHintsStructure()
{
    QVariantMap result;
    if (IsImagesSupported()) result["image-data"] = GetNotificationImage(48);
    result["sound-name"] = "message-new-instant";
    result["desktop-entry"] = AppConstants::DomainSchemeName;
    return result;
}

QList<QVariant> NotificationsManager::CreateArgListStructure(const QString& title, const QString& message)
{
    QList<QVariant> result;
    result << AppConstants::ProductName;
    result << static_cast<uint>(0);
    result << "";
    result << title;
    result << message;
    result << QStringList();
    result << CreateHintsStructure();
    result << static_cast<int>(-1);
    return result;
}

bool NotificationsManager::ShowNotification(const QString& title, const QString& message)
{
    QDBusInterface DBusInterface(NotifyBusName, NotifyBusPath, NotifyBusInterface, DBusInstance);
    QDBusReply<void> DBusReply = DBusInterface.callWithArgumentList(QDBus::AutoDetect, NotifyBusMethodNameNotify, CreateArgListStructure(title, message));
    return DBusReply.isValid();
}

NotificationsManager::NotificationsManager(QObject *parent) : QObject(parent)
{
    if (DBusInstance.isConnected())
    {
        qDBusRegisterMetaType<QImage>();
        GetCapabilities();
    }
}
