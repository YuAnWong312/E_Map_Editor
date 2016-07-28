
#include <MapFramework/DatabaseWidget.h>
#include <osg/MatrixTransform>
#include <osg/ValueObject>

using namespace MapGUI;

/*
DatabaseWidget::DatabaseWidget(QWidget *parent) :
    DataBase::DatabaseConfig(parent)
{
}

void DatabaseWidget::save()
{
    DataBase::RecordMap::Iterator itr = _map.begin();

    for( itr; itr != _map.end(); itr++)
    {
        osg::MatrixTransform* mt = (osg::MatrixTransform*)itr.key();
        if(mt)
        {
          QPersistentModelIndex* storeIndex = itr.value();
          if(storeIndex->isValid())
          {
              int row = storeIndex->row();

              //QModelIndex line_modelIndex    = _model->index(row, 0);
              //QModelIndex station_modelIndex = _model->index(row, 1);
              QModelIndex device_modelIndex  = _model->index(row, 2);

              //int line_ID = _model->data(line_modelIndex, Qt::UserRole).toInt();
              //int station_ID = _model->data(station_modelIndex, Qt::UserRole).toInt();
              int device_ID = _model->data(device_modelIndex, Qt::UserRole).toInt();

              mt->setUserValue<int>("DBID", device_ID);
          }
        }
    }
}

void DatabaseWidget::addRow(void *point)
{
    osg::MatrixTransform* mt = (osg::MatrixTransform*)point;
    if(mt)
    {
        int type = 0;
        mt->getUserValue<int>("TYPE", type);
        if(type == 0)
            return;

        bool success = _model->insertRow(_model->rowCount());
        if(success)
        {
            QModelIndex tmpIndex = _model->index(_model->rowCount()-1, 0);
            QPersistentModelIndex* storeIndex = new QPersistentModelIndex(tmpIndex);
            int row = storeIndex->row();
            qDebug()<<"add row="<<row;
            _map.insert(point, storeIndex);

            //set the model data
            //_model->setVerticalHeaderItem(_model->rowCount()-1, new QStandardItem("OK"));

            int ID = INT_MIN;
            mt->getUserValue<int>("DBID", ID);

            QModelIndex line_modelIndex    = _model->index(row, 0);
            QModelIndex station_modelIndex = _model->index(row, 1);
            QModelIndex device_modelIndex  = _model->index(row, 2);

            //int line_ID    = _model->data(line_modelIndex, Qt::UserRole).toInt();
            //int station_ID = _model->data(station_modelIndex, Qt::UserRole).toInt();
            //int device_ID  = _model->data(device_modelIndex, Qt::UserRole).toInt();

            _model->setData(line_modelIndex, INT_MIN, Qt::UserRole);
            _model->setData(line_modelIndex, INT_MIN, Qt::DisplayRole);

            _model->setData(station_modelIndex, INT_MIN, Qt::UserRole);
            _model->setData(station_modelIndex, INT_MIN, Qt::DisplayRole);

            _model->setData(device_modelIndex, INT_MIN, Qt::UserRole);
            _model->setData(device_modelIndex, INT_MIN, Qt::DisplayRole);

        }
    }
}
*/


