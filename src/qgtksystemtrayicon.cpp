/****************************************************************************
**
** Copyright (C) 2017 Crimson AS <info@crimson.no>
** Contact: https://www.crimson.no
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#include "qgtkhelpers.h"
#include "qgtksystemtrayicon.h"
#include "qgtkintegration.h"

#include <QtCore/qdebug.h>
#include <QtGui/qicon.h>
#include <QtGui/private/qguiapplication_p.h>

QGtkSystemTrayIcon::QGtkSystemTrayIcon()
{
}

QGtkSystemTrayIcon::~QGtkSystemTrayIcon()
{
}

void QGtkSystemTrayIcon::init()
{
}

void QGtkSystemTrayIcon::cleanup()
{
}

void QGtkSystemTrayIcon::updateIcon(const QIcon &)
{
}

void QGtkSystemTrayIcon::updateToolTip(const QString &)
{
}

void QGtkSystemTrayIcon::updateMenu(QPlatformMenu *)
{
}

QRect QGtkSystemTrayIcon::geometry() const
{
    return QRect();
}

void action_cb(NotifyNotification*, gchar *, gpointer)
{
    if (qApp->focusWindow()) {
        qApp->focusWindow()->raise();
    }
}

void QGtkSystemTrayIcon::showMessage(const QString &title, const QString &msg,
                 const QIcon& icon, MessageIcon iconType, int msecs)
{
    NotifyNotification *n = notify_notification_new(title.toUtf8().constData(), msg.toUtf8().constData(), nullptr);

    // ### technically, we could delete this after 'msecs'. we need to keep it
    // around to fire action_cb.
    m_notification.reset(n);

    if (!icon.isNull()) {
        QGtkRefPtr<GdkPixbuf> ico = qt_iconToPixbuf(icon);
        notify_notification_set_icon_from_pixbuf(n, ico.get());
    }

    switch (iconType) {
    case QPlatformSystemTrayIcon::NoIcon:
    case QPlatformSystemTrayIcon::Information:
        notify_notification_set_urgency(n, NOTIFY_URGENCY_LOW);
        break;
    case QPlatformSystemTrayIcon::Warning:
        notify_notification_set_urgency(n, NOTIFY_URGENCY_NORMAL);
        break;
    case QPlatformSystemTrayIcon::Critical:
        notify_notification_set_urgency(n, NOTIFY_URGENCY_CRITICAL);
        break;
    }

    notify_notification_set_timeout(n, msecs);
    notify_notification_add_action(n, "default", "default", action_cb, NULL, NULL);

    notify_notification_show(n, NULL);
}

bool QGtkSystemTrayIcon::isSystemTrayAvailable() const
{
    return true;
}

bool QGtkSystemTrayIcon::supportsMessages() const
{
    return true;
}

