/**
 * SPDX-FileCopyrightText: 2021-2023 EasyCoding Team and contributors
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef POLICYKIT_H
#define POLICYKIT_H

/** @file lib/policykit/policykit.h
 * Functions for polkit authorization
*/

#include <QString>

/**
 *
 */
bool CreateNewProfile(const QString& profileName, const QString& profileContent);


#endif
