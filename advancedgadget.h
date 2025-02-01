#ifndef ADVANCEDGADGET_H
#define ADVANCEDGADGET_H

#include "abstractgadget.h"
#include <QtGui/QColor>

class AdvancedGadget : public AbstractGadget
{
    Q_GADGET
    Q_PROPERTY(QColor color READ color WRITE setColor)

public:
    AdvancedGadget() : AbstractGadget(new Private) {}
    const QMetaObject* metaObject() const override { return &staticMetaObject; }

    QColor color() const {
        return d<Private>()->color;
    }

    QColor &color() {
        return d<Private>()->color;
    }

    void setColor(const QColor& color) {
        if (this->color() == color) return;
        d<Private>()->color = color;
    }

private:
    struct Private : public AbstractGadget::Private {
        Private() = default;
        Private(const Private &other)
            : AbstractGadget::Private(other)
            , color(other.color)
        {}

        QColor color = Qt::black;
    };
};

#endif // ADVANCEDGADGET_H
