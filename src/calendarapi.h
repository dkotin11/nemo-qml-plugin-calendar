/*
 * Copyright (C) 2013 Jolla Ltd.
 * Contact: Aaron Kennedy <aaron.kennedy@jollamobile.com>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Nemo Mobile nor the names of its contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

#ifndef CALENDARAPI_H
#define CALENDARAPI_H

#include <QStringList>
#include <QDateTime>
#include <QObject>

class QJSEngine;
class QQmlEngine;
class CalendarEvent;
class CalendarEventModification;

class CalendarApi : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList excludedNotebooks READ excludedNotebooks WRITE setExcludedNotebooks NOTIFY excludedNotebooksChanged)
    Q_PROPERTY(QString defaultNotebook READ defaultNotebook WRITE setDefaultNotebook NOTIFY defaultNotebookChanged)

public:
    CalendarApi(QObject *parent = 0);

    Q_INVOKABLE CalendarEventModification *createNewEvent();
    Q_INVOKABLE CalendarEventModification *createModification(CalendarEvent *sourceEvent);

    Q_INVOKABLE void remove(const QString &uid, const QString &recurrenceId = QString(),
                            const QDateTime &time = QDateTime());
    Q_INVOKABLE void removeAll(const QString &uid); // remove all instances an event, all recurrenceIds

    QStringList excludedNotebooks() const;
    void setExcludedNotebooks(const QStringList &);

    QString defaultNotebook() const;
    void setDefaultNotebook(const QString &notebook);

    static QObject *New(QQmlEngine *, QJSEngine *);

signals:
    void excludedNotebooksChanged();
    void defaultNotebookChanged();
};

#endif // CALENDARAPI_H
