#include "editdialog.h"
#include "data/abstractrepository.h"
#include "data/wagon.h"
#include "ui_editdialog.h"
#include <QCompleter>
#include <QMessageBox>
#include <QSqlQueryModel>

EditDialog::EditDialog(AbstractRepository *repository, QWidget *parent) : EditDialog(repository, Wagon(0), Create, parent) {}

EditDialog::EditDialog(AbstractRepository *repository, Wagon wagon, Mode mode, QWidget *parent) : QDialog(parent), ui(new Ui::EditDialog), repository(repository), mode(mode) {
  ui->setupUi(this);

  QSqlQueryModel *surnameModel = new QSqlQueryModel(this);
  surnameModel->setQuery("SELECT DISTINCT Surname FROM Wagons ORDER BY Surname;");
  ui->surnameComboBox->setModel(surnameModel);
  ui->surnameComboBox->completer()->setCompletionMode(QCompleter::PopupCompletion);

  QSqlQueryModel *filterTypeModel = new QSqlQueryModel(this);
  filterTypeModel->setQuery("SELECT DISTINCT FilterType FROM Wagons ORDER BY FilterType;");
  ui->filterTypeComboBox->setModel(filterTypeModel);
  ui->filterTypeComboBox->completer()->setCompletionMode(QCompleter::PopupCompletion);

  QSqlQueryModel *wagonTypeModel = new QSqlQueryModel(this);
  wagonTypeModel->setQuery("SELECT DISTINCT WagonType FROM Wagons ORDER BY WagonType;");
  ui->wagonTypeComboBox->setModel(wagonTypeModel);
  ui->wagonTypeComboBox->completer()->setCompletionMode(QCompleter::PopupCompletion);

  ui->wagonNumberEdit->setText(QString::number(wagon.wagonNumber()).rightJustified(5, '0'));
  ui->surnameComboBox->setEditText(wagon.surname());
  ui->filterTypeComboBox->setEditText(wagon.filterType());
  ui->wagonTypeComboBox->setEditText(wagon.wagonType());
  ui->commentEdit->setText(wagon.comment());
  switch (mode) {
  case Create:
    setWindowTitle("Добавить");
    ui->wagonNumberEdit->clear();
    ui->replacementDateEdit->setDate(QDate::currentDate());
    ui->nextReplacementDateEdit->setDate(nextDate());
    break;
  case Edit:
    setWindowTitle("Изменить");
    ui->wagonNumberEdit->setReadOnly(true);
    ui->replacementDateEdit->setDate(wagon.replacementDate());
    ui->nextReplacementDateEdit->setDate(wagon.nextReplacementDate());
    break;
  case Mark:
    setWindowTitle("Отметить");
    ui->wagonNumberEdit->setReadOnly(true);
    ui->replacementDateEdit->setReadOnly(true);
    ui->replacementDateEdit->setDate(QDate::currentDate());
    ui->nextReplacementDateEdit->setDate(nextDate());
    break;
  }
}

EditDialog::~EditDialog() {
  delete ui;
}

QDate EditDialog::nextDate() {
  int currentPeriod = repository->winterMode() ? repository->winterPeriod() : repository->summerPeriod();
  QDate nextDate = QDate::currentDate().addDays(currentPeriod);
  return nextDate;
}

void EditDialog::accept() {
  int wagonNumber = ui->wagonNumberEdit->text().toInt();
  QDate replacementDate = ui->replacementDateEdit->date();
  QDate nextReplacementDate = ui->nextReplacementDateEdit->date();
  QString surname = ui->surnameComboBox->currentText();
  QString filterType = ui->filterTypeComboBox->currentText();
  QString wagonType = ui->wagonTypeComboBox->currentText();
  QString comment = ui->commentEdit->text();

  if (wagonNumber < 1 || wagonNumber > 99999) {
    QMessageBox::warning(this, "Внимание", "Неверный номер вагона");
    return;
  }

  Wagon wagon(wagonNumber);
  wagon.setReplacementDate(replacementDate);
  wagon.setNextReplacementDate(nextReplacementDate);
  wagon.setSurname(surname);
  wagon.setFilterType(filterType);
  wagon.setWagonType(wagonType);
  wagon.setComment(comment);

  if (mode == Create)
    repository->insert(wagon);
  else
    repository->update(wagon);

  QDialog::accept();
}
