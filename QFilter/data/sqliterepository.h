#ifndef SQLITEREPOSITORY_H
#define SQLITEREPOSITORY_H

#include "abstractrepository.h"

#define DEFAULT_WINTER_PERIOD 30
#define DEFAULT_SUMMER_PERIOD 15

class QSqlQuery;

class SqliteRepository : public AbstractRepository {

  Q_OBJECT

public:
  explicit SqliteRepository(QObject *parent = nullptr);

  virtual void reopenDb() override;

  virtual void insert(Wagon wagon) override;
  virtual void update(Wagon wagon) override;
  virtual void remove(int number) override;
  virtual Wagon getByNumber(int number) override;
  virtual QList<Wagon> getAll() override;
  virtual QList<Wagon> getNeedChange() override;
  virtual QList<Wagon> getOnDate(QDate date) override;

  virtual bool winterMode() override;
  virtual void setWinterMode(bool mode) override;
  virtual int winterPeriod() override;
  virtual void setWinterPeriod(int period) override;
  virtual int summerPeriod() override;
  virtual void setSummerPeriod(int period) override;

private:
  void openDb();
  Wagon createWagon(QSqlQuery query);
  QList<Wagon> createList(QSqlQuery query);
};

#endif // SQLITEREPOSITORY_H
