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
    void fromToJsonObject();
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

class Parent : public AbstractGadget
{
    Q_GADGET
    Q_PROPERTY(QString lastName READ lastName WRITE setLastName)
protected:
    Parent(Private *d) : AbstractGadget(d) {}
public:
    const QMetaObject* metaObject() const override { return &staticMetaObject; }

    QString lastName() const { return d<Private>()->lastName; }
    void setLastName(const QString &lastName) {
        d<Private>()->lastName = lastName;
    }
protected:
    struct Private : public AbstractGadget::Private {
        QString lastName;

        Private *clone() const override { return new Private(*this); }
    };
};

class ParentWithChild : public Parent
{
    Q_GADGET
    Q_PROPERTY(Child child READ child WRITE setChild)
public:
    ParentWithChild() : Parent(new Private) {
        qRegisterMetaType<Child>();
    }
    const QMetaObject* metaObject() const override { return &staticMetaObject; }

    Child child() const { return d<Private>()->child; }
    void setChild(const Child &child) {
        d<Private>()->child = child;
    }
private:
    struct Private : public Parent::Private {
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

class ParentWithChildren : public Parent
{
    Q_GADGET
    Q_PROPERTY(QList<Child> children READ children WRITE setChildren)
public:
    ParentWithChildren() : Parent(new Private) {
        qRegisterMetaType<Child>();
    }
    const QMetaObject* metaObject() const override { return &staticMetaObject; }

    QList<Child> children() const { return d<Private>()->children; }
    void setChildren(const QList<Child> &children) {
        d<Private>()->children = children;
    }

    bool fromJsonObject(const QJsonObject &object) override {
        auto object2 = object;
        if (object2.contains("children")) {
            const auto array = object2.take("children").toArray();
            for (const auto &value : array) {
                Child child;
                if (child.fromJsonObject(value.toObject())) {
                    d<Private>()->children.append(child);
                } else {
                    return false;
                }
            }
        }
        return Parent::fromJsonObject(object2);
    }

    QJsonObject toJsonObject() const override {
        QJsonObject ret = Parent::toJsonObject();
        const auto children = d<Private>()->children;
        if (!children.isEmpty()) {
            QJsonArray array;
            for (const auto &child : children) {
                array.append(child.toJsonObject());
            }
            ret.insert("children", array);
        }
        return ret;
    }

private:
    struct Private : public Parent::Private {
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

void tst_Gadget::fromToJsonObject()
{
    const auto jsonString = R"( { "lastName": "Smith", "children": [ { "value": 2 }, { "value": 3 } ] } )";
    QJsonParseError error;
    const auto jsonDocument = QJsonDocument::fromJson(jsonString, &error);
    QVERIFY(!error.error);
    const auto jsonObject = jsonDocument.object();

    ParentWithChildren parent;
    QVERIFY(parent.fromJsonObject(jsonObject));

    QCOMPARE(parent.lastName(), "Smith");

    QCOMPARE(parent.children().length(), 2);

    QCOMPARE(parent.children().at(0).value(), 2);
    QCOMPARE(parent.children().at(1).value(), 3);

    QCOMPARE(parent.toJsonObject(), jsonObject);
}

QTEST_MAIN(tst_Gadget)
#include "main.moc"
