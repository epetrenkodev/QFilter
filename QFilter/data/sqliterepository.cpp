#include "sqliterepository.h"
#include "wagon.h"
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

SqliteRepository::SqliteRepository(QObject *parent) : AbstractRepository{parent} {
  QSqlDatabase::addDatabase("QSQLITE");
  openDb();
}

void SqliteRepository::reopenDb() {
  openDb();
  emit dataUpdated();
}

void SqliteRepository::insert(Wagon wagon) {
  QSqlQuery query;
  query.prepare("INSERT INTO Wagons (wagonNumber, replacementDate, nextReplacementDate, surname, filterType, wagonType, comment) "
                "VALUES (:wagonNumber, :replacementDate, :nextReplacementDate, :surname, :filterType, :wagonType, :comment)");
  query.bindValue(":wagonNumber", wagon.wagonNumber());
  query.bindValue(":replacementDate", wagon.replacementDate());
  query.bindValue(":nextReplacementDate", wagon.nextReplacementDate());
  query.bindValue(":surname", wagon.surname());
  query.bindValue(":filterType", wagon.filterType());
  query.bindValue(":wagonType", wagon.wagonType());
  query.bindValue(":comment", wagon.comment());
  query.exec();
}

void SqliteRepository::update(Wagon wagon) {
  QSqlQuery query;
  query.prepare("UPDATE Wagons "
                "SET replacementDate = :replacementDate, "
                "nextReplacementDate = :nextReplacementDate, "
                "surname = :surname, "
                "filterType = :filterType, "
                "wagonType = :wagonType, "
                "comment = :comment "
                "WHERE wagonNumber = :wagonNumber");
  query.bindValue(":wagonNumber", wagon.wagonNumber());
  query.bindValue(":replacementDate", wagon.replacementDate());
  query.bindValue(":nextReplacementDate", wagon.nextReplacementDate());
  query.bindValue(":surname", wagon.surname());
  query.bindValue(":filterType", wagon.filterType());
  query.bindValue(":wagonType", wagon.wagonType());
  query.bindValue(":comment", wagon.comment());
  query.exec();
}

void SqliteRepository::remove(int number) {
  QSqlQuery query;
  query.prepare("DELETE FROM Wagons WHERE wagonNumber = :wagonNumber");
  query.bindValue(":wagonNumber", number);
  query.exec();
}

Wagon SqliteRepository::getByNumber(int number) {
  QSqlQuery query;
  query.prepare("SELECT * FROM Wagons WHERE wagonNumber = :wagonNumber");
  query.bindValue(":wagonNumber", number);
  query.exec();
  query.first();
  return createWagon(query);
}

QList<Wagon> SqliteRepository::getAll() {
  QSqlQuery query("SELECT * FROM Wagons");
  return createList(query);
}

QList<Wagon> SqliteRepository::getNeedChange() {
  QSqlQuery query("SELECT * FROM Wagons WHERE date(nextReplacementDate) <= date('now')");
  return createList(query);
}

QList<Wagon> SqliteRepository::getOnDate(QDate date) {
  int currentPeriod = winterMode() ? winterPeriod() : summerPeriod();
  QSqlQuery query;
  query.prepare("SELECT * FROM Wagons WHERE (strftime('%J', :date) - strftime('%J', nextReplacementDate)) >= 0 AND "
                "(strftime('%J', :date) - strftime('%J', nextReplacementDate)) % :currentPeriod = 0");
  query.bindValue(":date", date.toString(Qt::ISODate));
  query.bindValue(":currentPeriod", currentPeriod);
  query.exec();
  return createList(query);
}

bool SqliteRepository::winterMode() {
  QSqlQuery query("SELECT value FROM Settings WHERE key = 'winterMode'");
  if (query.first())
    return query.value("value").toBool();
  return false;
}

void SqliteRepository::setWinterMode(bool mode) {
  QSqlQuery query;
  query.prepare("INSERT INTO Settings (key, value) VALUES ('winterMode', :mode) ON CONFLICT (key) DO UPDATE SET value = :mode");
  query.bindValue(":mode", mode);
  query.exec();
}

int SqliteRepository::winterPeriod() {
  QSqlQuery query("SELECT value FROM Settings WHERE key = 'winterPeriod'");
  if (query.first())
    return query.value("value").toInt();
  return DEFAULT_WINTER_PERIOD;
}

void SqliteRepository::setWinterPeriod(int period) {
  QSqlQuery query;
  query.prepare("INSERT INTO Settings (key, value) VALUES ('winterPeriod', :period) ON CONFLICT (key) DO UPDATE SET value = :period");
  query.bindValue(":period", period);
  query.exec();
}

int SqliteRepository::summerPeriod() {
  QSqlQuery query("SELECT value FROM Settings WHERE key = 'summerPeriod'");
  if (query.first())
    return query.value("value").toInt();
  return DEFAULT_SUMMER_PERIOD;
}

void SqliteRepository::setSummerPeriod(int period) {
  QSqlQuery query;
  query.prepare("INSERT INTO Settings (key, value) VALUES ('summerPeriod', :period) ON CONFLICT (key) DO UPDATE SET value = :period");
  query.bindValue(":period", period);
  query.exec();
}

void SqliteRepository::openDb() {
  QSqlDatabase db = QSqlDatabase::database();
  db.setDatabaseName(QSettings().value("Base/Path", "qfilter.db").toString());
  if (!db.open())
    qFatal("%s", qUtf8Printable(db.lastError().text()));

  QSqlQuery query;
  query.exec("CREATE TABLE IF NOT EXISTS Wagons ("
             "  wagonNumber         INTEGER NOT NULL UNIQUE,"
             "  replacementDate     TEXT NOT NULL,"
             "  nextReplacementDate TEXT NOT NULL,"
             "  surname             TEXT,"
             "  filterType          TEXT,"
             "  wagonType           TEXT,"
             "  comment             TEXT,"
             "  PRIMARY KEY(wagonNumber))");

  query.exec("CREATE TABLE IF NOT EXISTS Settings ("
             "  key     TEXT NOT NULL UNIQUE,"
             "  value   TEXT, "
             "  PRIMARY KEY(key))");

  QSqlDriver *driver = db.driver();
  driver->subscribeToNotification("Wagons");
  connect(driver, SIGNAL(notification(QString)), this, SIGNAL(dataUpdated()));
}

Wagon SqliteRepository::createWagon(QSqlQuery query) {
  int wagonNumberIndex = query.record().indexOf("wagonNumber");
  int replacementDateIndex = query.record().indexOf("replacementDate");
  int nextReplacementDateIndex = query.record().indexOf("nextReplacementDate");
  int surnameIndex = query.record().indexOf("surname");
  int filterTypeIndex = query.record().indexOf("filterType");
  int wagonTypeIndex = query.record().indexOf("wagonType");
  int commentIndex = query.record().indexOf("comment");

  int wagonNumber = query.value(wagonNumberIndex).toInt();
  QDate replacementDate = query.value(replacementDateIndex).toDate();
  QDate nextReplacementDate = query.value(nextReplacementDateIndex).toDate();
  QString surname = query.value(surnameIndex).toString();
  QString filterType = query.value(filterTypeIndex).toString();
  QString wagonType = query.value(wagonTypeIndex).toString();
  QString comment = query.value(commentIndex).toString();

  Wagon wagon(wagonNumber);
  wagon.setReplacementDate(replacementDate);
  wagon.setNextReplacementDate(nextReplacementDate);
  wagon.setSurname(surname);
  wagon.setFilterType(filterType);
  wagon.setWagonType(wagonType);
  wagon.setComment(comment);

  return wagon;
}

QList<Wagon> SqliteRepository::createList(QSqlQuery query) {
  QList<Wagon> list;
  while (query.next()) {
    Wagon wagon = createWagon(query);
    list.append(wagon);
  }
  return list;
}
