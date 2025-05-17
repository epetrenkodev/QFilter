#include "wagonmodel.h"
#include "data/abstractrepository.h"

WagonModel::WagonModel(AbstractRepository *repository, QObject *parent) : QAbstractTableModel(parent), repository(repository) {
  date = QDate::currentDate().addDays(1);
  connect(repository, SIGNAL(dataUpdated()), this, SLOT(update()));
}

int WagonModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent)
  return list.count();
}

int WagonModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent)
  return COLUMN_COUNT;
}

QVariant WagonModel::data(const QModelIndex &index, int role) const {
  if (role == Qt::DisplayRole) {
    int row = index.row();
    int col = index.column();
    Wagon wagon = list.at(row);
    switch (col) {
    case 0:
      return QString::number(wagon.wagonNumber()).rightJustified(5, '0');
    case 1:
      return wagon.replacementDate();
    case 2:
      return wagon.nextReplacementDate();
    case 3:
      return wagon.surname();
    case 4:
      return wagon.filterType();
    case 5:
      return wagon.wagonType();
    case 6:
      return wagon.comment();
    default:
      return QVariant();
    }
  } else {
    return QVariant();
  }
}

QVariant WagonModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
    switch (section) {
    case 0:
      return QString("Номер вагона");
    case 1:
      return QString("Дата замены");
    case 2:
      return QString("Дата следующей замены");
    case 3:
      return QString("Фамилия");
    case 4:
      return QString("Тип фильтра");
    case 5:
      return QString("Тип вагона");
    case 6:
      return QString("Коментарий");
    }
  }
  return QVariant();
}

void WagonModel::setMode(Mode mode) {
  this->mode = mode;
  update();
}

void WagonModel::setDate(QDate date) {
  this->date = date;
  if (mode == OnDate)
    update();
}

void WagonModel::update() {
  switch (mode) {
  case All:
    list = repository->getAll();
    break;
  case OnDate:
    list = repository->getOnDate(date);
    break;
  case NeedChange:
    list = repository->getNeedChange();
    break;
  }
  emit layoutChanged();
}
