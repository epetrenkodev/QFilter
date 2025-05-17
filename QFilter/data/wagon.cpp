#include "wagon.h"

Wagon::Wagon(int wagonNumber) : m_wagonNumber(wagonNumber) {}

int Wagon::wagonNumber() const { return m_wagonNumber; }

QDate Wagon::replacementDate() const { return m_replacementDate; }

void Wagon::setReplacementDate(const QDate &newReplacementDate) { m_replacementDate = newReplacementDate; }

QDate Wagon::nextReplacementDate() const { return m_nextReplacementDate; }

void Wagon::setNextReplacementDate(const QDate &newNextReplacementDate) { m_nextReplacementDate = newNextReplacementDate; }

QString Wagon::surname() const { return m_surname; }

void Wagon::setSurname(const QString &newSurname) { m_surname = newSurname; }

QString Wagon::filterType() const { return m_filterType; }

void Wagon::setFilterType(const QString &newFilterType) { m_filterType = newFilterType; }

QString Wagon::wagonType() const { return m_wagonType; }

void Wagon::setWagonType(const QString &newWagonType) { m_wagonType = newWagonType; }

QString Wagon::comment() const { return m_comment; }

void Wagon::setComment(const QString &newComment) { m_comment = newComment; }
