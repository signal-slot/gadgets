#ifndef ABSTRACTGADGET_H
#define ABSTRACTGADGET_H

#include <QtCore/QDebug>
#include <QtCore/QMetaProperty>
#include <QtCore/QObject>
#include <QtCore/QSharedData>
#include <QtCore/QString>
#include <QtCore/QSharedDataPointer>

class AbstractGadget
{
protected:
    class AbstractData;
    explicit AbstractGadget(AbstractData *d) : data(d) {}
public:
    AbstractGadget() {}
    AbstractGadget(const AbstractGadget &) = default;
    virtual ~AbstractGadget() = default;
    AbstractGadget &operator=(const AbstractGadget &) = default;

    virtual const QMetaObject* metaObject() const = 0;

    template <class T>
    bool operator!=(const T &other) const {
        return !operator==(other);
    }

    template <class T>
    bool operator==(const T &other) const {
        static const auto mo = metaObject();
        for (int i = 0; i < mo->propertyCount(); ++i) {
            const auto property = mo->property(i);
            const auto value = property.readOnGadget(&other);
            if (value != property.readOnGadget(this)) {
                return false;
            }
        }
        return true;
    }

protected:
    template<typename DerivedData>
    const DerivedData* d() const {
        return static_cast<const DerivedData*>(data.constData());
    }

    template<typename DerivedData>
    DerivedData* d() {
        // do detach() manually
        // Do not use d->ref or not call d->clone(). They detach implicitly.
        if (data.constData()->ref.loadRelaxed() != 1) {
            data.reset(data.constData()->clone());
        }
        return static_cast<DerivedData*>(data.data());
    }

    class AbstractData : public QSharedData {
    public:
        AbstractData() = default;
        AbstractData(const AbstractData &other) : QSharedData(other) {}
        virtual AbstractData* clone() const { return nullptr; }
    };

    template<typename T>
    class Private : public AbstractData {
    public:
        Private() : AbstractData() {}
        Private(const Private &other) : AbstractData(other) {}
        Private* clone() const override { return new T(*static_cast<const T*>(this)); }
    };

private:
    QSharedDataPointer<AbstractData> data;

    template <typename T>
    friend auto operator<<(QDebug debug, const T &gadget) -> std::enable_if_t<std::is_base_of_v<AbstractGadget, T>, QDebug> {
        QDebugStateSaver saver(debug);
        static const auto mo = gadget.metaObject();
        debug.nospace() << mo->className() << '(';
        for (int i = 0; i < mo->propertyCount(); ++i) {
            if (i > 0) {
                debug.nospace() << ", ";
            }
            const auto property = mo->property(i);
            const auto value = property.readOnGadget(&gadget);
            debug.nospace() << property.name() << ": " << value;
        }
        debug.nospace() << ')';
        return debug;
    }
};

#endif // ABSTRACTGADGET_H
