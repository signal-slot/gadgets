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
    CompositeGadget(const CompositeGadget &) = default;
    CompositeGadget &operator=(const CompositeGadget &) = default;

    // Return by value to ensure proper copy-on-write
    AdvancedGadget advanced() const {
        return D<Data>()->advanced;
    }

    void setAdvanced(const AdvancedGadget& advanced) {
        D<Data>()->advanced = advanced;
    }

    int priority() const {
        return D<Data>()->priority;
    }

    void setPriority(int val) {
        D<Data>()->priority = val;
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
