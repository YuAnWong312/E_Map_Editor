
#include <QComboBox>
#include <QLineEdit>
#include <QSet>
#include <QMessageBox>
#include <Database/ComboBoxDelegate.h>

#include <kdfdb.h>

using namespace DataBase;

namespace
{
QList<KTableLine> _lines;
QList<KTableStation> _stations;
QList<KTableDevice> _devices;
QSet<int> _set;
}

template<class List>
void setComboBoxValue(List& values, QComboBox* comboBox, int ID )
{
    typename List::iterator itr;

    /*
    for( itr = values.begin(); itr != values.end(); itr++ )
    {
        comboBox->addItem( (*itr).line_name );
    }
    */

    int index = 0;
    for( itr = values.begin(); itr != values.end(); itr++)
    {
        index++;

        if( (*itr).id == ID )
        {
            //if( index < comboBox->count() )
            //{

            comboBox->setCurrentIndex(index);

            //}
            break;
        }
    }
}

ComboBoxDelegate::ComboBoxDelegate(QObject *parent ) : QItemDelegate(parent), _database(0), _defaultShow(tr(""))
{
}

void ComboBoxDelegate::setDatabase(KCSDB* db)
{
    _database = db;
}

QWidget *ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem& option , const QModelIndex& index ) const
{
    int column = index.column();
    bool useComboBox = column==0 || column==1 || column==2;
    if(useComboBox)
    {
        QComboBox* editor = new QComboBox(parent);
        editor->addItem(_defaultShow);
        return editor;
    }

    bool useEditor = column==3;
    if(useEditor)
    {
        QLineEdit* editor = new QLineEdit(parent);
        editor->setText(_defaultShow);
        return editor;
    }
    return 0;
}

void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    ComboBoxDelegate* tmp = (ComboBoxDelegate*)this;

    int row = index.row();
    int column = index.column();

    bool useComboBox = column==0 || column==1 || column==2;

    if(useComboBox)
    {
        QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
        QModelIndex line_modelIndex = index.model()->index(row, 0);
        QModelIndex station_modelIndex = index.model()->index(row, 1);
        QModelIndex device_modelIndex = index.model()->index(row, 2);

        int line_ID = index.model()->data(line_modelIndex, Qt::UserRole).toInt();
        int station_ID = index.model()->data(station_modelIndex, Qt::UserRole).toInt();
        int device_ID = index.model()->data(device_modelIndex, Qt::UserRole).toInt();

        /*
        qDebug()<<"Line_ID="<<line_ID;
        qDebug()<<"Station_ID="<<station_ID;
        qDebug()<<"Device_ID="<<device_ID;
        */

        switch(column)
        {
        case 0:
        {
            _lines = _database->LineShow();

            /*
            int size = tmp->_lines.size();
            qDebug()<<"line size="<<size;
            */

            for(QList<KTableLine>::iterator itr = _lines.begin(); itr != _lines.end(); itr++)
            {
                comboBox->addItem( (*itr).line_name );
            }

            /*
            int lineIndex = 0;
            for( QList<KTableLine>::iterator itr = _lines.begin(); itr != _lines.end(); itr++)
            {
                lineIndex++;

                if( (*itr).id == line_ID )
                {
                    if( lineIndex < comboBox->count() )
                    {
                        comboBox->setCurrentIndex(lineIndex);
                    }
                    break;
                }
            }
            */
            setComboBoxValue(_lines, comboBox, line_ID);
        }
            break;
        case 1:
        {

            _stations = _database->StationShow(line_ID);

            /*
            int size = tmp->_stations.size();
            qDebug()<<"station size="<<size;
            */

            for(QList<KTableStation>::iterator itr = _stations.begin(); itr != _stations.end(); itr++)
            {
                comboBox->addItem( (*itr).station_name );
            }

            /*
            int stationIndex = 0;
            for( QList<KTableStation>::iterator itr = _stations.begin(); itr != _stations.end(); itr++)
            {
                stationIndex++;

                if( (*itr).id == station_ID )
                {
                    if( stationIndex < comboBox->count() )
                    {
                        comboBox->setCurrentIndex(stationIndex);
                    }
                    break;
                }
            }
            */
            setComboBoxValue(_stations, comboBox, station_ID);

        }
            break;
        case 2:
        {

            _devices = _database->DeviceShow(station_ID);

            /*
            int size = tmp->_devices.size();
            qDebug()<<"device size="<<size;
            */

            for(QList<KTableDevice>::iterator itr = _devices.begin(); itr != _devices.end(); itr++)
            {
                comboBox->addItem( QString("%1").arg((*itr).id) );
            }
            /*
            int deviceIndex = 0;
            for( QList<KTableDevice>::iterator itr = _devices.begin(); itr != _devices.end(); itr++)
            {
                deviceIndex++;

                if( (*itr).id == device_ID )
                {
                    if( deviceIndex < comboBox->count() )
                    {
                        comboBox->setCurrentIndex(deviceIndex);
                    }
                    break;
                }
            }
            */
            setComboBoxValue(_devices, comboBox, device_ID);
        }
            break;
        default:
            break;
        }

    }

    bool useEditor = column==3;
    if(useEditor)
    {
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor);

        QModelIndex device_modelIndex = index.model()->index(row, 3);
        QString device_ID = index.model()->data(device_modelIndex, Qt::UserRole).toString();
        lineEdit->setText(device_ID);
    }
}

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    int row = index.row();
    int column = index.column();

    bool useComboBox = column==0 || column==1 || column==2;
    if(useComboBox)
    {
        QComboBox* comboBox = qobject_cast<QComboBox*>(editor);

        QModelIndex line_modelIndex = index.model()->index(row, 0);
        QModelIndex station_modelIndex = index.model()->index(row, 1);
        QModelIndex device_modelIndex = index.model()->index(row, 2);

        int line_ID = index.model()->data(line_modelIndex, Qt::UserRole).toInt();
        int station_ID = index.model()->data(station_modelIndex, Qt::UserRole).toInt();
        int device_ID = index.model()->data(device_modelIndex, Qt::UserRole).toInt();

        int ID = INT_MIN;
        int selectIndex = comboBox->currentIndex()-1;

        /*
        if(-1 == selectIndex)
        {
            ID = INT_MIN;

            switch(column)
            {
            case 0:
            {
                model->setData(station_modelIndex, _defaultShow, Qt::EditRole);
                model->setData(station_modelIndex, INT_MIN, Qt::UserRole);

                model->setData(device_modelIndex, _defaultShow, Qt::EditRole);
                model->setData(device_modelIndex, INT_MIN, Qt::UserRole);
            }
                break;
            case 1:
            {
                model->setData(device_modelIndex, _defaultShow, Qt::EditRole);
                model->setData(device_modelIndex, INT_MIN, Qt::UserRole);
            }
                break;
            case 2:
            {

            }
                break;

            default:
                break;
            }

        }
        //else*/

        {
            switch(column)
            {
            case 0:
            {
                //int size = _lines.size();
                //if(selectIndex < size)
                //{
                if(selectIndex != -1)
                {
                    KTableLine line = _lines[selectIndex];
                    ID = line.id;
                }

                if(ID != line_ID)
                {
                    model->setData(station_modelIndex, _defaultShow, Qt::EditRole);
                    model->setData(station_modelIndex, INT_MIN, Qt::UserRole);

                    model->setData(device_modelIndex, _defaultShow, Qt::EditRole);
                    model->setData(device_modelIndex, INT_MIN, Qt::UserRole);

                    _set.remove(device_ID);
                }
                //}

            }
                break;
            case 1:
            {
                //int size = _stations.size();
                //if(selectIndex < size)
                //{
                if(selectIndex != -1)
                {
                    KTableStation station = _stations[selectIndex];
                    ID = station.id;
                }

                if(ID != station_ID)
                {
                    model->setData(device_modelIndex, _defaultShow, Qt::EditRole);
                    model->setData(device_modelIndex, INT_MIN, Qt::UserRole);

                    _set.remove(device_ID);
                }
                //}

            }
                break;
            case 2:
            {
                //int size = _devices.size();
                //if(selectIndex < size)
                //{
                if(selectIndex != -1)
                {
                    KTableDevice device = _devices[selectIndex];
                    ID = device.id;
                }

                if(ID != device_ID)
                {
                    _set.remove(device_ID);
                }
                //}
            }
                break;

            default:
                break;
            }
        }

        if( column==2 )
        {
            if(_set.contains(ID))
            {
                QWidget* parent = qobject_cast<QWidget*>(this->parent());
                QMessageBox::warning(parent, tr("警告"), tr("编号已经使用"));
                return;
            }
            else
            {

                if(ID != INT_MIN)
                    _set.insert(ID);
            }
        }

        QString text = comboBox->currentText();
        model->setData(index, text, Qt::EditRole);
        model->setData(index, ID, Qt::UserRole);
    }

    bool useEditor = column==3;
    if(useEditor)
    {
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor);
        QModelIndex device_modelIndex = index.model()->index(row, 3);

        QString device_ID = lineEdit->text();
        model->setData(device_modelIndex, device_ID, Qt::DisplayRole);
        model->setData(device_modelIndex, device_ID, Qt::UserRole);
    }

    /*
    foreach (int value, _set)
        qDebug() << value;
    */
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
