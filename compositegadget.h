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
    CompositeGadget() : AbstractGadget(new Data) {}

    const QMetaObject* metaObject() const override { return &staticMetaObject; }

    // Return by value to ensure proper copy-on-write
    AdvancedGadget advanced() const {
        return D<Data>()->advanced;
    }

    void setAdvanced(const AdvancedGadget& advanced) {
        if (this->advanced() == advanced) return;
        D<Data>()->advanced = advanced;
    }

    int priority() const {
        return D<Data>()->priority;
    }

    void setPriority(int priority) {
        if (this->priority() == priority) return;
        D<Data>()->priority = priority;
    }

private:
    struct Data : public AbstractGadget::Data<Data> {
        AdvancedGadget advanced;
        int priority = 0;

        Data() = default;
        Data(const Data& other)
            : AbstractGadget::Data<Data>(other)
            , advanced(other.advanced)
            , priority(other.priority)
        {}
    };
};

#endif // COMPOSITEGADGET_H
