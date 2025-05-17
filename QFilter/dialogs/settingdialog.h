#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>

class AbstractRepository;

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog {

  Q_OBJECT

public:
  explicit SettingDialog(AbstractRepository *repository, QWidget *parent = nullptr);
  ~SettingDialog();

private slots:
  void saveSettings();
  void openDbFile();

private:
  Ui::SettingDialog *ui;
  AbstractRepository *repository;
};

#endif // SETTINGDIALOG_H
