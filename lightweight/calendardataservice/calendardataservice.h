/*
 * Copyright (C) 2015 Jolla Ltd.
 * Contact: Petri M. Gerdt <petri.gerdt@jollamobile.com>
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

#ifndef CALENDARDATASERVICE_H
#define CALENDARDATASERVICE_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QTimer>

#include "../common/eventdata.h"

class CalendarAgendaModel;

class CalendarDataService : public QObject
{
    Q_OBJECT

public:
    explicit CalendarDataService(QObject *parent = 0);

    void fetchEvents();

signals:
    void getEventsResult(const QString &transactionId, const EventDataList &eventDataList);

public slots:
    QString getEvents(const QString &startDate, const QString &endDate);

private slots:
    void updated();
    void shutdown();
    void processQueue();

private:
    struct DataRequest {
        QDate start;
        QDate end;
        QString transactionId;
    };

    void initialize();

    CalendarAgendaModel *mAgendaModel;
    QTimer mKillTimer;
    int mTransactionIdCounter;
    QList<DataRequest> mDataRequestQueue;
    DataRequest mCurrentDataRequest;
};

#endif // CALENDARDATASERVICE_H
