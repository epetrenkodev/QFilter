#include "data/sqliterepository.h"
#include "dialogs/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication application(argc, argv);
  QCoreApplication::setOrganizationName(APP_COMPANY);
  QCoreApplication::setApplicationName(APP_NAME);
  QCoreApplication::setApplicationVersion(APP_VERSION);
  SqliteRepository repository;
  MainWindow mainWindos(&repository);
  mainWindos.show();
  return application.exec();
}
