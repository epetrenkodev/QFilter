#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QBarSeries>
#include <QBarSet>
#include <QChart>
#include <QMainWindow>

using namespace QtCharts;

QT_BEGIN_NAMESPACE

class WagonModel;
class AbstractRepository;

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow {

  Q_OBJECT

public:
  MainWindow(AbstractRepository *repository, QWidget *parent = nullptr);
  ~MainWindow();

private:
  Ui::MainWindow *ui;
  AbstractRepository *repository;
  WagonModel *model;
  int prevPosition;
  QBarSet *wagons;
  QChart *chart;
  QBarSeries *series;

  int currentPosition();
  void setPosition(int position);
  void savePosition();
  void recoverPosition();

private slots:
  void addNew();
  void editCurrent();
  void markCurrent();
  void removeCurrent();
  void displayModeChanged(int index);
  void setMode(bool);
  void copyToClipboard();
  void copyToClipboardAll();
  void updateDiagram();
};

#endif // MAINWINDOW_H
