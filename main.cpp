#include <QtTest/QtTest>
#include <QDebug>
#include "abstractgadget.h"

void debugRecursive(const AbstractGadget *gadget)
{
    const auto mo = gadget->metaObject();
    for (int i = 0; i < mo->propertyCount(); i++) {
        const auto property = mo->property(i);
        const auto type = property.typeName();
        const auto name = property.name();
        const auto value = property.readOnGadget(gadget);
        if (value.canConvert<AbstractGadget>()) {
            const auto *gadget = reinterpret_cast<const AbstractGadget *>(value.constData());
            qDebug() << type << name << gadget;
            debugRecursive(gadget);
        } else {
            qDebug() << type << name << value;
        }
    }
};

class tst_Gadget : public QObject
{
    Q_OBJECT

private slots:
    void basic();
    void child();
    void children();
};

class Gadget : public AbstractGadget
{
    Q_GADGET
    Q_PROPERTY(int value READ value WRITE setValue)
public:
    Gadget() : AbstractGadget(new Private) {}
    const QMetaObject* metaObject() const override { return &staticMetaObject; }

    int value() const { return d<Private>()->value; }
    void setValue(int value) {
        d<Private>()->value = value;
    }
private:
    struct Private : public AbstractGadget::Private {
        int value = 0;

        Private *clone() const override { return new Private(*this); }
    };
};

void tst_Gadget::basic()
{
    int value = 1234;
    Gadget gadget;
    gadget.setValue(value);
    QCOMPARE(gadget.value(), value);
}

class Child : public AbstractGadget
{
    Q_GADGET
    Q_PROPERTY(int value READ value WRITE setValue)
public:
    Child() : AbstractGadget(new Private) {}
    const QMetaObject* metaObject() const override { return &staticMetaObject; }

    int value() const { return d<Private>()->value; }
    void setValue(int value) {
        d<Private>()->value = value;
    }
private:
    struct Private : public AbstractGadget::Private {
        int value = 0;

        Private *clone() const override { return new Private(*this); }
    };
};

class ParentWithChild : public AbstractGadget
{
    Q_GADGET
    Q_PROPERTY(Child child READ child WRITE setChild)
public:
    ParentWithChild() : AbstractGadget(new Private) {}
    const QMetaObject* metaObject() const override { return &staticMetaObject; }

    Child child() const { return d<Private>()->child; }
    void setChild(const Child &child) {
        d<Private>()->child = child;
    }
private:
    struct Private : public AbstractGadget::Private {
        Child child;

        Private *clone() const override { return new Private(*this); }
    };
};

void tst_Gadget::child()
{
    ParentWithChild parent;

    Child child;
    child.setValue(1);
    parent.setChild(child);

    QCOMPARE(parent.child(), child);
}

class ParentWithChildren : public AbstractGadget
{
    Q_GADGET
    Q_PROPERTY(QList<Child> children READ children WRITE setChildren)
public:
    ParentWithChildren() : AbstractGadget(new Private) {}
    const QMetaObject* metaObject() const override { return &staticMetaObject; }

    QList<Child> children() const { return d<Private>()->children; }
    void setChildren(const QList<Child> &children) {
        d<Private>()->children = children;
    }
private:
    struct Private : public AbstractGadget::Private {
        QList<Child> children;

        Private *clone() const override { return new Private(*this); }
    };
};

void tst_Gadget::children()
{
    ParentWithChildren parent;

    Child child1;
    child1.setValue(1);
    Child child2;
    child2.setValue(2);
    QList<Child> children { child1, child2 };
    parent.setChildren(children);

    QCOMPARE(parent.children(), children);
}

QTEST_MAIN(tst_Gadget)
#include "main.moc"
