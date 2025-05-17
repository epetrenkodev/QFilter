#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>

class AbstractRepository;
class Wagon;

namespace Ui {
class EditDialog;
}

class EditDialog : public QDialog {

  Q_OBJECT

public:
  enum Mode { Create,
              Edit,
              Mark };

  EditDialog(AbstractRepository *repository, QWidget *parent = nullptr);
  EditDialog(AbstractRepository *repository, Wagon wagon, Mode mode = Edit, QWidget *parent = nullptr);
  ~EditDialog();

private:
  Ui::EditDialog *ui;
  AbstractRepository *repository;
  Mode mode;

  QDate nextDate();

private slots:
  virtual void accept() override;
};

#endif // EDITDIALOG_H
