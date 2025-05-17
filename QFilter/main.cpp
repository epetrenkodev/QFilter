#include "data/sqliterepository.h"
#include "dialogs/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication application(argc, argv);
  QCoreApplication::setOrganizationName("EPetrenko");
  QCoreApplication::setApplicationName("QFilter");
  SqliteRepository repository;
  MainWindow mainWindos(&repository);
  mainWindos.show();
  return application.exec();
}
