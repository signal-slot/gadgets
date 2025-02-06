// Copyright (C) 2025 Signal Slot Inc.
// SPDX-License-Identifier: MIT

#include <QtTest/QtTest>
#include <QDebug>
#include "abstractgadget.h"

class tst_Gadget : public QObject
{
    Q_OBJECT

private slots:
    void fromToJsonObject();
};

class Gadget : public AbstractGadget
{
    Q_GADGET
    Q_PROPERTY(int value READ value WRITE setValue)
public:
    Gadget() : AbstractGadget(new Private) {}
protected:
    Gadget(Private *d) : AbstractGadget(d) {}
public:
    const QMetaObject* metaObject() const override { return &staticMetaObject; }

    int value() const { return d<Private>()->value; }
    void setValue(int value) {
        d<Private>()->value = value;
    }
protected:
    struct Private : public AbstractGadget::Private {
        int value = 0;

        Private *clone() const override { return new Private(*this); }
    };
};

class SubGadget : public Gadget
{
    Q_GADGET
    Q_PROPERTY(int value1 READ value1 WRITE setValue1)
public:
    SubGadget() : Gadget(new Private) {}
    const QMetaObject* metaObject() const override { return &staticMetaObject; }

    int value1() const { return d<Private>()->value1; }
    void setValue1(int value1) {
        d<Private>()->value1 = value1;
    }
private:
    struct Private : public Gadget::Private {
        int value1 = 0;

        Private *clone() const override { return new Private(*this); }
    };
};

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
    Q_PROPERTY(Gadget gadget READ gadget WRITE setGadget)
    Q_PROPERTY(QList<Gadget> gadgets READ gadgets WRITE setGadgets)
    Q_PROPERTY(SubGadget subGadget READ subGadget WRITE setSubGadget)
    Q_PROPERTY(QList<SubGadget> subGadgets READ subGadgets WRITE setSubGadgets)
public:
    enum class Enum {
        info,
        debug,
        warning,
        fatal,
    };
    Q_ENUM(Enum)

    Types() : AbstractGadget(new Private) {
        // qRegisterMetaType<Gadget>();
        // qRegisterMetaType<QList<Gadget>>();
        // qRegisterMetaType<SubGadget>();
        // qRegisterMetaType<QList<SubGadget>>();
    }
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
    ACCESSOR(Gadget, const Gadget&, gadget, Gadget)
    ACCESSOR(QList<Gadget>, const QList<Gadget>&, gadgets, Gadgets)
    ACCESSOR(SubGadget, const SubGadget&, subGadget, SubGadget)
    ACCESSOR(QList<SubGadget>, const QList<SubGadget>&, subGadgets, SubGadgets)

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
        Gadget gadget;
        QList<Gadget> gadgets;
        SubGadget subGadget;
        QList<SubGadget> subGadgets;

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
        "gadget": { "value": 1 },
        "gadgets": [ { "value": 2 }, { "value": 3 } ],
        "subGadget": { "value1": 1 },
        "subGadgets": [ { "value1": 2 }, { "value1": 3 } ]
    } )";
    QJsonParseError error;
    const auto jsonDocument = QJsonDocument::fromJson(jsonString, &error);
    QVERIFY(!error.error);
    const auto jsonObject = jsonDocument.object();

    Types types;
    QVERIFY(types.fromJsonObject(jsonObject));
    qDebug() << types;

    QCOMPARE(types.toJsonObject(), jsonObject);
}

QTEST_MAIN(tst_Gadget)
#include "main.moc"
