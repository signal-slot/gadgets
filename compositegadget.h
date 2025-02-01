#ifndef COMPOSITEGADGET_H
#define COMPOSITEGADGET_H

#include "abstractgadget.h"
#include "advancedgadget.h"

class CompositeGadget : public AbstractGadget
{
    Q_GADGET
    Q_PROPERTY(AdvancedGadget advanced READ advanced WRITE setAdvanced)
    Q_PROPERTY(int priority READ priority WRITE setPriority)

public:
    CompositeGadget() : AbstractGadget(new Private) {}

    const QMetaObject* metaObject() const override { return &staticMetaObject; }

    AdvancedGadget advanced() const {
        return d<Private>()->advanced;
    }

    AdvancedGadget &advanced() {
        return d<Private>()->advanced;
    }

    void setAdvanced(const AdvancedGadget& advanced) {
        if (this->advanced() == advanced) return;
        d<Private>()->advanced = advanced;
    }

    int priority() const {
        return d<Private>()->priority;
    }

    void setPriority(int priority) {
        if (this->priority() == priority) return;
        d<Private>()->priority = priority;
    }

private:
    struct Private : public AbstractGadget::Private {
        AdvancedGadget advanced;
        int priority = 0;

        Private() = default;
        Private(const Private& other)
            : AbstractGadget::Private(other)
            , advanced(other.advanced)
            , priority(other.priority)
        {}
    };
};

#endif // COMPOSITEGADGET_H
