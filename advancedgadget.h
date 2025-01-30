#ifndef ADVANCEDGADGET_H
#define ADVANCEDGADGET_H

#include "abstractgadget.h"
#include <QtGui/QColor>

class AdvancedGadget : public AbstractGadget
{
    Q_GADGET
    Q_PROPERTY(QColor color READ color WRITE setColor)

public:
    AdvancedGadget() : AbstractGadget(new Data) {}
    const QMetaObject* metaObject() const override { return &staticMetaObject; }

    QColor color() const {
        return D<Data>()->color;
    }

    void setColor(const QColor& color) {
        if (this->color() == color) return;
        D<Data>()->color = color;
    }

private:
    struct Data : public AbstractGadget::Data<Data> {
        Data() = default;
        Data(const Data &other)
            : AbstractGadget::Data<Data>(other)
            , color(other.color)
        {}

        QColor color = Qt::black;
    };
};

#endif // ADVANCEDGADGET_H
