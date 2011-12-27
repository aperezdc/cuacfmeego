/*
 * cfm-controller.cc
 * Copyright (C) 2011 Adrian Perez <aperez@igalia.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "cfm-controller.h"
#include <QDeclarativeContext>


CFMController::CFMController(QDeclarativeContext* context):
    _context(context)
{
    Q_ASSERT(context);
    _context->setContextProperty("controller", this);
}


CFMController::~CFMController()
{
}

