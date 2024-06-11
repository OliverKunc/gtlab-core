/* GTlab - Gas Turbine laboratory
 *
 * SPDX-License-Identifier: MPL-2.0+
 * SPDX-FileCopyrightText: 2023 German Aerospace Center (DLR)
 * Source File: gt_doublemonitoringproperty.h
 *
 *  Created on: 14.10.2016
 *  Author: Stanislaus Reitenbach (AT-TW)
 *  Tel.: +49 2203 601 2907
 */

#ifndef GTDOUBLEMONITORINGPROPERTY_H
#define GTDOUBLEMONITORINGPROPERTY_H

#include "gt_core_exports.h"

#include "gt_doubleproperty.h"

using GtDoubleMonitoringProperty = GtDoubleProperty;

namespace gt
{

/**
 * @brief Creates a property factory for monitoring doubles with a default value
 */
GT_CORE_EXPORT
gt::PropertyFactoryFunction makeDoubleMonitoringProperty(double value);

} // namespace gt

#endif // GTDOUBLEMONITORINGPROPERTY_H
