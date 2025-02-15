/*
 * Copyright (C) 2014 Jolla Ltd.
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

#ifndef CALENDARWORKER_H
#define CALENDARWORKER_H

#include "calendardata.h"

#include <QObject>
#include <QHash>

// mkcal
#include <extendedstorage.h>

// libaccounts-qt
namespace Accounts { class Manager; }

class CalendarInvitationQuery;

class CalendarWorker : public QObject, public mKCal::ExtendedStorageObserver
{
    Q_OBJECT
    
public:
    CalendarWorker();
    ~CalendarWorker();

    /* mKCal::ExtendedStorageObserver */
    void storageModified(mKCal::ExtendedStorage *storage, const QString &info);
    void storageProgress(mKCal::ExtendedStorage *storage, const QString &info);
    void storageFinished(mKCal::ExtendedStorage *storage, bool error, const QString &info);

public slots:
    void init();
    void save();

    void saveEvent(const CalendarData::Event &eventData, bool updateAttendees,
                   const QList<CalendarData::EmailContact> &required,
                   const QList<CalendarData::EmailContact> &optional);
    void replaceOccurrence(const CalendarData::Event &eventData, const QDateTime &startTime, bool updateAttendees,
                           const QList<CalendarData::EmailContact> &required,
                           const QList<CalendarData::EmailContact> &optional);
    void deleteEvent(const QString &uid, const QDateTime &recurrenceId, const QDateTime &dateTime);
    void deleteAll(const QString &uid);
    bool sendResponse(const CalendarData::Event &eventData, const CalendarEvent::Response response);
    QString convertEventToICalendar(const QString &uid, const QString &prodId) const;

    QList<CalendarData::Notebook> notebooks() const;
    void setNotebookColor(const QString &notebookUid, const QString &color);
    void setExcludedNotebooks(const QStringList &list);
    void excludeNotebook(const QString &notebookUid, bool exclude);
    void setDefaultNotebook(const QString &notebookUid);

    void loadData(const QList<CalendarData::Range> &ranges,
                  const QStringList &instanceList, bool reset);

    CalendarData::EventOccurrence getNextOccurrence(const QString &uid, const QDateTime &recurrenceId,
                                                    const QDateTime &startTime) const;
    QList<CalendarData::Attendee> getEventAttendees(const QString &uid, const QDateTime &recurrenceId);

    void findMatchingEvent(const QString &invitationFile);

signals:
    void storageModifiedSignal(const QString &info);

    void eventNotebookChanged(const QString &oldEventUid, const QString &newEventUid, const QString &notebookUid);

    void excludedNotebooksChanged(const QStringList &excludedNotebooks);
    void notebookColorChanged(const CalendarData::Notebook &notebook);
    void notebooksChanged(const QList<CalendarData::Notebook> &notebooks);

    void dataLoaded(const QList<CalendarData::Range> &ranges,
                    const QStringList &instanceList,
                    const QMultiHash<QString, CalendarData::Event> &events,
                    const QHash<QString, CalendarData::EventOccurrence> &occurrences,
                    const QHash<QDate, QStringList> &dailyOccurrences,
                    bool reset);

    void occurrenceExceptionFailed(const CalendarData::Event &eventData, const QDateTime &startTime);
    void occurrenceExceptionCreated(const CalendarData::Event &eventData, const QDateTime &startTime,
                                    const QDateTime &newRecurrenceId);

    void findMatchingEventFinished(const QString &invitationFile,
                                   const CalendarData::Event &eventData);

private:
    void setEventData(KCalendarCore::Event::Ptr &event, const CalendarData::Event &eventData);
    void loadNotebooks();
    QStringList excludedNotebooks() const;
    bool saveExcludeNotebook(const QString &notebookUid, bool exclude);

    bool setRecurrence(KCalendarCore::Event::Ptr &event, CalendarEvent::Recur recur, CalendarEvent::Days days);
    bool setReminder(KCalendarCore::Event::Ptr &event, int seconds, const QDateTime &dateTime);
    bool needSendCancellation(KCalendarCore::Event::Ptr &event) const;
    void updateEventAttendees(KCalendarCore::Event::Ptr event, bool newEvent,
                              const QList<CalendarData::EmailContact> &required,
                              const QList<CalendarData::EmailContact> &optional,
                              const QString &notebookUid);
    QString getNotebookAddress(const QString &notebookUid) const;
    QString getNotebookAddress(const KCalendarCore::Event::Ptr &event) const;

    CalendarData::Event createEventStruct(const KCalendarCore::Event::Ptr &event,
                                          mKCal::Notebook::Ptr notebook = mKCal::Notebook::Ptr()) const;
    QHash<QString, CalendarData::EventOccurrence> eventOccurrences(const QList<CalendarData::Range> &ranges) const;
    QHash<QDate, QStringList> dailyEventOccurrences(const QList<CalendarData::Range> &ranges,
                                                    const QList<CalendarData::EventOccurrence> &occurrences);

    Accounts::Manager *mAccountManager;

    mKCal::ExtendedCalendar::Ptr mCalendar;
    mKCal::ExtendedStorage::Ptr mStorage;

    // mDeletedEvents is used to make sure
    // that we are sending a cancellation email for events only
    // when user actually saved (so truly deleted) changes by calling of save()
    QList<QPair<QString, QDateTime>> mDeletedEvents;

    QHash<QString, CalendarData::Notebook> mNotebooks;

    // Tracks which events have been already passed to manager. Maps Uid -> RecurrenceId
    QMultiHash<QString, QDateTime> mSentEvents;
};

#endif // CALENDARWORKER_H
