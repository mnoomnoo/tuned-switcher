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

/**
 *
 */
void RegisterMetaTypes();

/**
 * Check if the process is authorized to perform the policykit.exec action
 * @returns true / false
 * @retval true if the process is authorized to perform the action
 * @retval false if the process is not authorized to perform the action
 */
bool CheckAuthorization();


#endif
