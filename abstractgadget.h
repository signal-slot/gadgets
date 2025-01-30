#ifndef ABSTRACTGADGET_H
#define ABSTRACTGADGET_H

#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtCore/QSharedData>
#include <QtCore/QString>
#include <QtCore/QSharedDataPointer>

class AbstractGadget
{
    Q_GADGET
protected:
    class AbstractData;
    explicit AbstractGadget(AbstractData *d) : d(d) {}
public:
    AbstractGadget(const AbstractGadget&) = default;
    virtual ~AbstractGadget() = default;

    AbstractGadget& operator=(const AbstractGadget &other) {
        d = other.d;
        return *this;
    }

protected:
    template<typename DerivedData>
    const DerivedData* D() const {
        return static_cast<const DerivedData*>(d.constData());
    }

    template<typename DerivedData>
    DerivedData* D() {
        // do detach() manually
        if (d->ref.loadRelaxed() != 1) {
            d.reset(d->clone());
        }
        return static_cast<DerivedData*>(d.data());
    }

    class AbstractData : public QSharedData {
    public:
        virtual AbstractData* clone() const { return new AbstractData; }
    };

    template<typename T>
    class Data : public AbstractData {
    public:
        Data* clone() const override {
            return new T(*static_cast<const T*>(this));
        }
    };

private:
    QSharedDataPointer<AbstractData> d;
};

#endif // ABSTRACTGADGET_H
