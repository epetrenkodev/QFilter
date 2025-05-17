#ifndef WAGONMODEL_H
#define WAGONMODEL_H

#include "data/wagon.h"
#include <QAbstractTableModel>

#define COLUMN_COUNT 7

class AbstractRepository;

class WagonModel : public QAbstractTableModel {

  Q_OBJECT

public:
  enum Mode { All,
              OnDate,
              NeedChange };

  explicit WagonModel(AbstractRepository *repository, QObject *parent = nullptr);

  virtual int rowCount(const QModelIndex &parent) const override;
  virtual int columnCount(const QModelIndex &parent) const override;
  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

public slots:
  void setMode(Mode mode);
  void setDate(QDate date);
  void update();

private:
  Mode mode{NeedChange};
  QDate date;
  QList<Wagon> list;
  AbstractRepository *repository;
};

#endif // WAGONMODEL_H
