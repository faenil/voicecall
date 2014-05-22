/*
 * This file is a part of the Voice Call Manager project
 *
 * Copyright (C) 2011-2012  Tom Swindell <t.swindell@rubyx.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */
#include "common.h"
#include "voicecallhandlerdbusadapter.h"

#include "abstractvoicecallprovider.h"

/*!
  \class VoiceCallHandlerDBusAdapter
  \brief The D-Bus adapter for the voice call manager service.
*/
class VoiceCallHandlerDBusAdapterPrivate
{
    Q_DECLARE_PUBLIC(VoiceCallHandlerDBusAdapter)

public:
    VoiceCallHandlerDBusAdapterPrivate(VoiceCallHandlerDBusAdapter *q, AbstractVoiceCallHandler *pHandler)
        : q_ptr(q), handler(pHandler)
    {/*...*/}

    VoiceCallHandlerDBusAdapter *q_ptr;
    AbstractVoiceCallHandler *handler;

};

/*!
  Constructs a new DBus adapter for the provided voice call handler. \a handler
*/
VoiceCallHandlerDBusAdapter::VoiceCallHandlerDBusAdapter(AbstractVoiceCallHandler *handler)
    : QDBusAbstractAdaptor(handler),
      d_ptr(new VoiceCallHandlerDBusAdapterPrivate(this, handler))
{
    TRACE
    Q_D(VoiceCallHandlerDBusAdapter);

    QObject::connect(d->handler, SIGNAL(statusChanged(VoiceCallStatus)), SLOT(onStatusChanged()));
    QObject::connect(d->handler, SIGNAL(lineIdChanged(QString)), SIGNAL(lineIdChanged(QString)));
    QObject::connect(d->handler, SIGNAL(startedAtChanged(QDateTime)), SIGNAL(startedAtChanged(QDateTime)));
    QObject::connect(d->handler, SIGNAL(durationChanged(int)), SIGNAL(durationChanged(int)));
    QObject::connect(d->handler, SIGNAL(emergencyChanged(bool)), SIGNAL(emergencyChanged(bool)));
    QObject::connect(d->handler, SIGNAL(multipartyChanged(bool)), SIGNAL(multipartyChanged(bool)));
    QObject::connect(d->handler, SIGNAL(forwardedChanged(bool)), SIGNAL(forwardedChanged(bool)));
}

VoiceCallHandlerDBusAdapter::~VoiceCallHandlerDBusAdapter()
{
    TRACE
    Q_D(VoiceCallHandlerDBusAdapter);

    delete d;
}

/*!
  Returns this voice calls' provider id.
 */
QString VoiceCallHandlerDBusAdapter::providerId() const
{
    TRACE
    Q_D(const VoiceCallHandlerDBusAdapter);
    return d->handler->provider()->providerId();
}

/*!
  Returns this voice calls' unique handler id, this is generated by the voice call manager.
*/
QString VoiceCallHandlerDBusAdapter::handlerId() const
{
    TRACE
    Q_D(const VoiceCallHandlerDBusAdapter);
    return d->handler->handlerId();
}

/*!
  Returns this voice calls' line id property.
*/
QString VoiceCallHandlerDBusAdapter::lineId() const
{
    TRACE
    Q_D(const VoiceCallHandlerDBusAdapter);
    return d->handler->lineId();
}

/*!
  Returns this voice calls' started at property.
*/
QDateTime VoiceCallHandlerDBusAdapter::startedAt() const
{
    TRACE
    Q_D(const VoiceCallHandlerDBusAdapter);
    return d->handler->startedAt();
}

/*!
  Returns this voice calls' duration property.
*/
int VoiceCallHandlerDBusAdapter::duration() const
{
    TRACE
    Q_D(const VoiceCallHandlerDBusAdapter);
    return d->handler->duration();
}

/*!
  Returns this voice calls' incoming call flag property.
*/
bool VoiceCallHandlerDBusAdapter::isIncoming() const
{
    TRACE
    Q_D(const VoiceCallHandlerDBusAdapter);
    return d->handler->isIncoming();
}

/*!
  Returns this voice calls' multiparty flag property.
*/
bool VoiceCallHandlerDBusAdapter::isMultiparty() const
{
    TRACE
    Q_D(const VoiceCallHandlerDBusAdapter);
    return d->handler->isMultiparty();
}

/*!
  Returns this voice calls' forwarded flag property.
*/
bool VoiceCallHandlerDBusAdapter::isForwarded() const
{
    TRACE
    Q_D(const VoiceCallHandlerDBusAdapter);
    return d->handler->isForwarded();
}

/*!
  Returns this voice calls' emergency flag property.
*/
bool VoiceCallHandlerDBusAdapter::isEmergency() const
{
    TRACE
    Q_D(const VoiceCallHandlerDBusAdapter);
    return d->handler->isEmergency();
}

/*!
  Returns this voice calls' call status property.
*/
int VoiceCallHandlerDBusAdapter::status() const
{
    TRACE
    Q_D(const VoiceCallHandlerDBusAdapter);
    return (int)d->handler->status();
}

/*!
  Returns this voice calls' call status property as a symbolic string.
*/
QString VoiceCallHandlerDBusAdapter::statusText() const
{
    TRACE
    Q_D(const VoiceCallHandlerDBusAdapter);
    return d->handler->statusText();
}

/*!
  Initiates the answering of this voice call, if its' an incoming call.

  \sa status(), hangup(), deflect()
*/
bool VoiceCallHandlerDBusAdapter::answer()
{
    TRACE
    Q_D(VoiceCallHandlerDBusAdapter);
    d->handler->answer();
    return true;
}

/*!
  Initiates hanging up this voice call, if its' currently not disconnected.

  \sa status(), answer(), hold(), deflect()
*/
bool VoiceCallHandlerDBusAdapter::hangup()
{
    TRACE
    Q_D(VoiceCallHandlerDBusAdapter);
    d->handler->hangup();
    return true;
}

/*!
  Initiates holding this voice call, if its' currently not disconnected.

  \sa status(), answer(), hangup(), deflect()
*/
bool VoiceCallHandlerDBusAdapter::hold(bool on)
{
    TRACE
    Q_D(VoiceCallHandlerDBusAdapter);
    d->handler->hold(on);
    return true;
}

/*!
  Initiates deflecting this call to the provided target msisdn (phone number).

  \sa status(), answer(), hangup()
*/
bool VoiceCallHandlerDBusAdapter::deflect(const QString &target)
{
    TRACE
    Q_D(VoiceCallHandlerDBusAdapter);
    d->handler->deflect(target);
    return true;
}

void VoiceCallHandlerDBusAdapter::sendDtmf(const QString &tones)
{
    TRACE
    Q_D(VoiceCallHandlerDBusAdapter);
    d->handler->sendDtmf(tones);
}

QVariantMap VoiceCallHandlerDBusAdapter::getProperties()
{
    TRACE
    Q_D(VoiceCallHandlerDBusAdapter);
    QVariantMap props;

    props.insert("handlerId", QVariant(handlerId()));
    props.insert("providerId", QVariant(providerId()));
    props.insert("status", QVariant(status()));
    props.insert("statusText", QVariant(statusText()));
    props.insert("lineId", QVariant(lineId()));
    props.insert("startedAt", QVariant(startedAt().toMSecsSinceEpoch()));
    props.insert("duration", QVariant(duration()));
    props.insert("isIncoming", QVariant(isIncoming()));
    props.insert("isEmergency", QVariant(isEmergency()));
    props.insert("isMultiparty", QVariant(isMultiparty()));
    props.insert("isForwarded", QVariant(isForwarded()));

    return props;
}

void VoiceCallHandlerDBusAdapter::onStatusChanged()
{
    emit statusChanged(status(), statusText());
}
