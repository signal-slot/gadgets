#include <QtTest/QtTest>
#include <QDebug>
#include "abstractgadget.h"

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

class Types : public AbstractGadget
{
    Q_GADGET
    Q_PROPERTY(bool boolean READ boolean WRITE setBoolean)
    Q_PROPERTY(QList<bool> booleans READ booleans WRITE setBooleans)
    Q_PROPERTY(int integer READ integer WRITE setInteger)
    Q_PROPERTY(QList<int> integers READ integers WRITE setIntegers)
    Q_PROPERTY(QByteArray data READ data WRITE setData)
    Q_PROPERTY(QByteArrayList datas READ datas WRITE setDatas)
    Q_PROPERTY(QString string READ string WRITE setString)
    Q_PROPERTY(QStringList strings READ strings WRITE setStrings)
    Q_PROPERTY(QJsonObject object READ object WRITE setObject)
    Q_PROPERTY(Enum enumeration READ enumeration WRITE setEnumeration)
    Q_PROPERTY(QList<Enum> enumerations READ enumerations WRITE setEnumerations)
    Q_PROPERTY(Child child READ child WRITE setChild)
    Q_PROPERTY(QList<Child> children READ children WRITE setChildren)
public:
    enum class Enum {
        info,
        debug,
        warning,
        fatal,
    };
    Q_ENUM(Enum)

    Types() : AbstractGadget(new Private) {}
    const QMetaObject* metaObject() const override { return &staticMetaObject; }

#define ACCESSOR(type, Type, name, Name) \
    type name() const { return d<Private>()->name; } \
    void set##Name(Type name) { \
        d<Private>()->name = name; \
    }

    ACCESSOR(bool, bool, boolean, Boolean)
    ACCESSOR(QList<bool>, const QList<bool>&, booleans, Booleans)
    ACCESSOR(int, int, integer, Integer)
    ACCESSOR(QList<int>, const QList<int>&, integers, Integers)
    ACCESSOR(QByteArray, const QByteArray&, data, Data)
    ACCESSOR(QByteArrayList, const QByteArrayList&, datas, Datas)
    ACCESSOR(QString, const QString&, string, String)
    ACCESSOR(QStringList, const QStringList&, strings, Strings)
    ACCESSOR(QJsonObject, const QJsonObject&, object, Object)
    ACCESSOR(Enum, const Enum&, enumeration, Enumeration)
    ACCESSOR(QList<Enum>, const QList<Enum>&, enumerations, Enumerations)
    ACCESSOR(Child, const Child&, child, Child)
    ACCESSOR(QList<Child>, const QList<Child>&, children, Children)

private:
    struct Private : public AbstractGadget::Private {
        bool boolean = false;
        QList<bool> booleans;
        int integer = 0;
        QList<int> integers;
        QByteArray data;
        QByteArrayList datas;
        QString string;
        QStringList strings;
        QJsonObject object;
        Enum enumeration = Enum::info;
        QList<Enum> enumerations;
        Child child;
        QList<Child> children;

        Private *clone() const override { return new Private(*this); }
    };
};

void tst_Gadget::fromToJsonObject()
{
    const auto jsonString = R"({
        "boolean": true,
        "booleans": [true, false, true],
        "integer": 1,
        "integers": [1, 2, 3, 4, 5],
        "data": "data",
        "datas": ["data1", "data2", "data3"],
        "string": "string",
        "strings": ["string1", "string2", "string3"],
        "object": { "key": "value" },
        "enumeration": "debug",
        "enumerations": ["info", "warning"],
        "child": { "value": 1 },
        "children": [ { "value": 2 }, { "value": 3 } ]
    } )";
    QJsonParseError error;
    const auto jsonDocument = QJsonDocument::fromJson(jsonString, &error);
    QVERIFY(!error.error);
    const auto jsonObject = jsonDocument.object();

    Types types;
    QVERIFY(types.fromJsonObject(jsonObject));
    QCOMPARE(types.toJsonObject(), jsonObject);
}

QTEST_MAIN(tst_Gadget)
#include "main.moc"
