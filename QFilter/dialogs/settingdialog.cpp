#include "settingdialog.h"
#include "data/abstractrepository.h"
#include "ui_settingdialog.h"
#include <QFileDialog>
#include <QSettings>

SettingDialog::SettingDialog(AbstractRepository *repository, QWidget *parent) : QDialog(parent), ui(new Ui::SettingDialog), repository(repository) {
  ui->setupUi(this);
  ui->winterPeriodEdit->setValue(repository->winterPeriod());
  ui->summerPeriodEdit->setValue(repository->summerPeriod());
  ui->dbPathEdit->setText(QSettings().value("Base/Path", "qfilter.db").toString());
  ui->copyFormatEdit->setText(QSettings().value("Copy/Format", "nft").toString());
  connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &SettingDialog::saveSettings);
  connect(ui->viewButton, &QPushButton::clicked, this, &SettingDialog::openDbFile);
}

SettingDialog::~SettingDialog() {
  delete ui;
}

void SettingDialog::saveSettings() {
  repository->setWinterPeriod(ui->winterPeriodEdit->value());
  repository->setSummerPeriod(ui->summerPeriodEdit->value());
  QSettings().setValue("Base/Path", ui->dbPathEdit->text());
  QSettings().setValue("Copy/Format", ui->copyFormatEdit->text());
  repository->reopenDb();
}

void SettingDialog::openDbFile() {
  ui->dbPathEdit->setText(QFileDialog::getOpenFileName());
}
