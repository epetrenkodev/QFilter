#include "mainwindow.h"
#include "aboutdialog.h"
#include "data/abstractrepository.h"
#include "editdialog.h"
#include "manualdialog.h"
#include "models/wagonmodel.h"
#include "settingdialog.h"
#include "ui_mainwindow.h"
#include <QBarCategoryAxis>
#include <QClipboard>
#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QValueAxis>

MainWindow::MainWindow(AbstractRepository *repository, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), repository(repository) {
  ui->setupUi(this);

  // model
  model = new WagonModel(repository);

  // mainWindow
  restoreGeometry(QSettings().value("Geometry").toByteArray());

  // menu
  connect(ui->actionExit, &QAction::triggered, qApp, &QApplication::closeAllWindows);                           // Выход
  connect(ui->actionManual, &QAction::triggered, this, []() { ManualDialog().exec(); });                        // Краткое руководство
  connect(ui->actionAbout, &QAction::triggered, this, []() { AboutDialog().exec(); });                          // О программе
  connect(ui->actionSettings, &QAction::triggered, this, [repository]() { SettingDialog(repository).exec(); }); // Настройки
  connect(ui->actionAdd, &QAction::triggered, this, &MainWindow::addNew);                                       // Добавить
  connect(ui->actionEdit, &QAction::triggered, this, &MainWindow::editCurrent);                                 // Изменить
  connect(ui->actionMark, &QAction::triggered, this, &MainWindow::markCurrent);                                 // Отметить
  connect(ui->actionRemove, &QAction::triggered, this, &MainWindow::removeCurrent);                             // Удалить
  connect(ui->actionCopy, &QAction::triggered, this, &MainWindow::copyToClipboard);
  connect(ui->actionCopyAll, &QAction::triggered, this, &MainWindow::copyToClipboardAll);
  connect(ui->actionDiagram, &QAction::triggered, this, [=](bool checked) { ui->diagram->setVisible(checked); });

  int colCount = model->columnCount(QModelIndex());
  for (int i = 0; i < colCount; ++i) {
    QAction *action = ui->menuColumns->addAction(model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString());
    action->setCheckable(true);
    action->setChecked(QSettings().value("Columns/Column_" + QString::number(i), true).toBool());
    action->setData(i);
    connect(action, &QAction::triggered, this, [=](bool checed) { (checed) ? ui->tableView->showColumn(action->data().toInt()) : ui->tableView->hideColumn(action->data().toInt()); });
  }

  ui->actionDiagram->setChecked(QSettings().value("Diagram", false).toBool());

  // filterComboBox
  connect(ui->filterComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(displayModeChanged(int)));

  // dateEdit
  ui->dateEdit->setVisible(false);
  ui->dateEdit->setMinimumDate(QDate::currentDate());
  ui->dateEdit->setDate(QDate::currentDate().addDays(1));
  connect(ui->dateEdit, SIGNAL(dateChanged(QDate)), model, SLOT(setDate(QDate)));

  // winterMode
  repository->winterMode() ? ui->winterRadioButton->setChecked(true) : ui->summerRadioButton->setChecked(true);
  connect(ui->winterRadioButton, SIGNAL(toggled(bool)), this, SLOT(setMode(bool)));

  // tableView
  ui->tableView->addAction(ui->actionAdd);
  ui->tableView->addAction(ui->actionCopy);
  ui->tableView->addAction(ui->actionCopyAll);
  ui->tableView->addAction(ui->actionMark);
  ui->tableView->addAction(ui->actionEdit);
  ui->tableView->addAction(ui->actionRemove);
  ui->tableView->setModel(model);
  ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  for (int i = 0; i < model->columnCount(QModelIndex()); ++i) {
      (QSettings().value("Columns/Column_" + QString::number(i), true).toBool()) ? ui->tableView->showColumn(i) : ui->tableView->hideColumn(i);
  }
  connect(ui->tableView, &QTableView::activated, ui->actionMark, &QAction::trigger);

  // Diagram

  wagons = new QBarSet("Вагоны");

  for (int i = 0; i < 30; ++i) {
      wagons->append(0);
  }

  series = new QBarSeries();
  series->append(wagons);

  chart = new QChart();
  chart->addSeries(series);

  QStringList categories;
  QDateTime dateTime = QDateTime::currentDateTime();
  for (int i = 0; i < 30; ++i)
      categories.append(QString::number(dateTime.addDays(i).date().day()));

  QBarCategoryAxis *axisX = new QBarCategoryAxis();
  axisX->append(categories);
  chart->addAxis(axisX, Qt::AlignBottom);
  series->attachAxis(axisX);

  QValueAxis *axisY = new QValueAxis();
  axisY->setLabelFormat("%d");
  chart->addAxis(axisY, Qt::AlignLeft);
  series->attachAxis(axisY);

  ui->diagram->setChart(chart);

  connect(ui->filterComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateDiagram()));
  connect(ui->winterRadioButton, SIGNAL(toggled(bool)), this, SLOT(updateDiagram()));
  connect(repository, SIGNAL(dataUpdated()), this, SLOT(updateDiagram()));
  ui->diagram->setVisible(ui->actionDiagram->isChecked());
  updateDiagram();

  // search
  // connect(ui->searchEdit, &QLineEdit::textChanged, this, [=]() { model->setSearch(ui->searchEdit->text()); });

  // StatusBar
  connect(model, &WagonModel::layoutChanged, this, [=]() { ui->statusbar->showMessage("Всего: " + QString::number(model->rowCount(QModelIndex()))); });

  displayModeChanged(0);
}

MainWindow::~MainWindow() {
  QSettings().setValue("Geometry", saveGeometry());

  for (int i = 0; i < model->columnCount(QModelIndex()); ++i) {
    QSettings().setValue("Columns/Column_" + QString::number(i), !ui->tableView->isColumnHidden(i));
  }

  QSettings().setValue("Diagram", ui->actionDiagram->isChecked());

  delete model;
  delete ui;
}

void MainWindow::addNew() {
  EditDialog dialog(repository);
  dialog.exec();
}

void MainWindow::editCurrent() {
  savePosition();
  int number = model->data(model->index(currentPosition(), 0)).toInt();
  Wagon wagon = repository->getByNumber(number);
  EditDialog dialog(repository, wagon);
  dialog.exec();
  recoverPosition();
}

void MainWindow::markCurrent() {
  savePosition();
  int number = model->data(model->index(currentPosition(), 0)).toInt();
  Wagon wagon = repository->getByNumber(number);
  EditDialog dialog(repository, wagon, EditDialog::Mark);
  dialog.exec();
  recoverPosition();
  connect(ui->winterRadioButton, SIGNAL(toggled(bool)), this, SLOT(setMode(bool)));
}

void MainWindow::removeCurrent() {
  int number = model->data(model->index(currentPosition(), 0)).toInt();
  if (QMessageBox::question(this, "Удалить", "Вы уверены, что хотите удалить вагон " + QString::number(number)) == QMessageBox::Yes) {
    savePosition();
    repository->remove(number);
    recoverPosition();
  }
}

void MainWindow::displayModeChanged(int index) {
  ui->dateEdit->setVisible(index == 1);
  switch (index) {
  case 0:
    model->setMode(WagonModel::NeedChange);
    break;
  case 1:
    model->setMode(WagonModel::OnDate);
    break;
  case 2:
    model->setMode(WagonModel::All);
    break;
  }
  ui->tableView->setFocus();
  setPosition(0);
}

int MainWindow::currentPosition() {
  return ui->tableView->currentIndex().row();
}

void MainWindow::setPosition(int position) {
  ui->tableView->selectRow(position);
}

void MainWindow::savePosition() {
  prevPosition = currentPosition();
}

void MainWindow::recoverPosition() {
  setPosition(std::min(prevPosition, model->rowCount(QModelIndex()) - 1));
}

void MainWindow::setMode(bool winterMode) {
  repository->setWinterMode(winterMode);
}

void MainWindow::copyToClipboard() {
  QString copyText;
  QString space;
  QString format = QSettings().value("Copy/Format", "nft").toString();
  QModelIndex currentIndex = ui->tableView->currentIndex();
  int i = 0;
  for (auto c : format) {
    space = (i++ != 0) ? "\t" : "";
    switch (c.unicode()) {
    case 'n':
      copyText += space + currentIndex.siblingAtColumn(0).data().toString();
      break;
    case 'd':
      copyText += space + currentIndex.siblingAtColumn(1).data().toString();
      break;
    case 'D':
      copyText += space + currentIndex.siblingAtColumn(2).data().toString();
      break;
    case 'f':
      copyText += space + currentIndex.siblingAtColumn(3).data().toString();
      break;
    case 't':
      copyText += space + currentIndex.siblingAtColumn(4).data().toString();
      break;
    case 'T':
      copyText += space + currentIndex.siblingAtColumn(5).data().toString();
      break;
    case 'k':
      copyText += space + currentIndex.siblingAtColumn(6).data().toString();
      break;
    default:
      copyText += space;
      break;
    }
  }
  QApplication::clipboard()->setText(copyText);
}

void MainWindow::copyToClipboardAll() {
  QString copyText;
  QString space;
  QString format = QSettings().value("Copy/Format", "nft").toString();
  for (int i = 0; i < model->rowCount(QModelIndex()); ++i) {
    if (i > 0)
      copyText += "\n";
    int t = 0;
    for (auto c : format) {
      space = (t++ != 0) ? "\t" : "";
      switch (c.unicode()) {
      case 'n':
        copyText += space + model->index(i, 0).data().toString();
        break;
      case 'd':
        copyText += space + model->index(i, 1).data().toString();
        break;
      case 'D':
        copyText += space + model->index(i, 2).data().toString();
        break;
      case 'f':
        copyText += space + model->index(i, 3).data().toString();
        break;
      case 't':
        copyText += space + model->index(i, 4).data().toString();
        break;
      case 'T':
        copyText += space + model->index(i, 5).data().toString();
        break;
      case 'k':
        copyText += space + model->index(i, 6).data().toString();
        break;
      default:
        copyText += space;
        break;
      }
    }
  }
  QApplication::clipboard()->setText(copyText);
}

void MainWindow::updateDiagram()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    for (int i = 0; i < 30; ++i) {
        QDate date = dateTime.addDays(i).date();
        int countOfDate = repository->getOnDate(date).count();
        wagons->replace(i, countOfDate);
    }

    qreal maxVal = 0;
    for (int i = 0; i < wagons->count(); ++i)
        maxVal = qMax(maxVal, wagons->at(i));
    QValueAxis *axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
    axisY->setRange(0, maxVal);
}
