#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AboutDialog) {
  ui->setupUi(this);
  setFixedSize(320, 240);
  ui->versoinLabel->setText(QCoreApplication::applicationName() + " " + QCoreApplication::applicationVersion());
}

AboutDialog::~AboutDialog() {
  delete ui;
}
