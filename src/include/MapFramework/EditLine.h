
#ifndef EDITLINE_H
#define EDITLINE_H 1

#include <QLineEdit>
#include <MapFramework/Export.h>

namespace MapGUI
{
class MAPGUI_EXPORT EditLine : public QLineEdit
{
    Q_OBJECT
public:
    EditLine(QWidget * parent = 0, bool checkRange = false);

    inline void setMax(float max){ _max = max; }
    inline void setMin(float min){ _min = min; }
    inline void setMinMax(float min, float max)
    {
        _min = min;
        _max = max;
    }
    inline void setCheakRange(bool check){ _checkRange = check; }

    float getValue();

protected:
    virtual void init();

public slots:
    void checkIsValid();

protected:
    bool _checkRange;
    float _min;
    float _max;
    float _value;
};
}

#endif
