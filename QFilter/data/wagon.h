#ifndef WAGON_H
#define WAGON_H

#include <QDate>

class Wagon {

public:
  Wagon(int wagonNumber);

  int wagonNumber() const;
  QDate replacementDate() const;
  void setReplacementDate(const QDate &newReplacementDate);
  QDate nextReplacementDate() const;
  void setNextReplacementDate(const QDate &newNextReplacementDate);
  QString surname() const;
  void setSurname(const QString &newSurname);
  QString filterType() const;
  void setFilterType(const QString &newFilterType);
  QString wagonType() const;
  void setWagonType(const QString &newWagonType);
  QString comment() const;
  void setComment(const QString &newComment);

private:
  int m_wagonNumber;
  QDate m_replacementDate;
  QDate m_nextReplacementDate;
  QString m_surname;
  QString m_filterType;
  QString m_wagonType;
  QString m_comment;
};

#endif // WAGON_H
