#include "abstractgadget.h"

#include <QtCore/QJsonArray>

using namespace Qt::Literals::StringLiterals;

bool AbstractGadget::fromJsonObject(const QJsonObject &object)
{
    const auto mo = metaObject();

    for (int i = 0; i < mo->propertyCount(); i++) {
        const auto property = mo->property(i);
        if (property.isConstant())
            continue;
        const auto propertyType = property.typeName();
        const auto propertyName = QString::fromLatin1(property.name());
        if (!object.contains(propertyName)) {
            if (property.isRequired() && !property.isConstant())
                return false;
            continue;
        }

        const auto value = object.value(propertyName);
        if (value.isUndefined())
            continue;

        switch (value.type()) {
        case QJsonValue::Array: {
            const auto array = value.toArray();
            auto propertyValue = property.readOnGadget(this);
            // qDebug() << "Array" << propertyName << propertyValue << value.toArray();
            QByteArray typeName = property.typeName();
            if (typeName == "QStringList"_ba) {
                QStringList list;
                for (const auto &str : array) {
                    Q_ASSERT(str.isString());
                    list.append(str.toString());
                }
                if (!property.writeOnGadget(this, list))
                    qFatal() << this;

            } else if (typeName.startsWith("QList<"_ba) && typeName.endsWith(">")) {
                typeName = typeName.mid(6).chopped(1);
                const auto mt = QMetaType::fromName(typeName);
                if (!mt.isValid()) {
                    qWarning() << "Unknown type" << typeName << propertyValue;
                } else {
                    const auto mo = mt.metaObject();
                    if (mt.flags() & QMetaType::IsEnumeration) {
                        for (int i = 0; i < mo->enumeratorCount(); i++) {
                            const auto me = mo->enumerator(i);
                            // qDebug() << i << typeName << mo->className() << me.name();
                            if (typeName == QByteArray(mo->className()) + "::" + me.enumName()) {
                                QList<int> *list = reinterpret_cast<QList<int> *>(propertyValue.data());
                                for (const auto &str : array) {
                                    Q_ASSERT(str.isString());
                                    const auto string = str.toString();
                                    auto bytearray = string.toLatin1();
                                    const auto v = me.keyToValue(bytearray.constData());
                                    list->append(v);
                                }
                                break;
                            }
                        }
                    } else {

                        QList<AbstractGadget> *list = reinterpret_cast<QList<AbstractGadget> *>(propertyValue.data());
                        const auto array = value.toArray();
                        for (const auto &v : array) {
                            AbstractGadget base;
                            auto *sub = static_cast<AbstractGadget *>(mt.construct(&base));
                            if (!sub->fromJsonObject(v.toObject()))
                                return false;
                            list->append(*sub);
                        }
                    }
                    if (!property.writeOnGadget(this, propertyValue))
                        qFatal() << this;
                }
            } else {
                qFatal() << typeName;
            }
            break; }
        case QJsonValue::Object: {
            auto propertyValue = property.readOnGadget(this);
            if (propertyValue.canConvert<AbstractGadget>()) {
                auto *gadget = reinterpret_cast<AbstractGadget *>(propertyValue.data());
                if (!gadget->fromJsonObject(value.toObject()))
                    return false;
                if (!property.writeOnGadget(this, propertyValue))
                    qFatal() << gadget;
            } else {
                switch (property.typeId()) {
                case QMetaType::QJsonObject:
                    property.writeOnGadget(this, value.toObject());
                    break;
                case QMetaType::QJsonValue:
                    property.writeOnGadget(this, value);
                    break;
                default:
                    qFatal() << propertyType << "not supporeted for" << mo->className() <<  propertyName << propertyValue;
                    break;
                }
            }
            break; }
        default:
            if (!property.writeOnGadget(this, value.toVariant())) {
                qWarning() << propertyType << mo->className() << propertyName << value;
            }
            break;
        }
    }

    return true;
}

namespace {
QList<int> requiredOrModifiedPropertyIndices(const AbstractGadget *gadget)
{
    QList<int> ret;
    const auto mo = gadget->metaObject();
    auto other = static_cast<AbstractGadget *>(mo->metaType().create());
    for (int i = 0; i < mo->propertyCount(); ++i) {
        const auto property = mo->property(i);
        if (property.isRequired()) {
            ret.append(i);
        } else {
            const auto value1 = property.readOnGadget(gadget);
            const auto value2 = property.readOnGadget(other);
            if (value1 != value2)
                ret.append(i);
        }
    }
    delete other;
    return ret;
}
}

QJsonObject AbstractGadget::toJsonObject() const
{
    QJsonObject ret;
    const auto mo = metaObject();
    const auto indices = requiredOrModifiedPropertyIndices(this);
    for (int i : indices) {
        const auto mp = mo->property(i);
        const auto name = QString::fromLatin1(mp.name());
        auto value = mp.readOnGadget(this);
        if (value.canConvert<AbstractGadget>()) {
            const auto *gadget = reinterpret_cast<const AbstractGadget *>(value.constData());
            const auto object = gadget->toJsonObject();
            value = object.toVariantMap();
        } else {
            auto typeName = QByteArray(mp.typeName());
            if (typeName.startsWith("QList<") && typeName.endsWith('>')) {
                typeName = typeName.mid(6).chopped(1);
                const auto mt = QMetaType::fromName(typeName);
                if (!mt.isValid()) {
                    qWarning() << typeName << " unknown type" << value;
                    value = value.toList();
                } else {
                    const auto mo = mt.metaObject();
                    if (mt.flags() & QMetaType::IsEnumeration) {
                        bool found = false;
                        for (int i = 0; i < mo->enumeratorCount(); i++) {
                            const auto me = mo->enumerator(i);
                            if (typeName == QByteArray(mo->className()) + "::" + me.enumName()) {
                                QVariantList list = value.toList();
                                for (int j = 0; j < list.size(); j++) {
                                    const auto v = list.at(j).toInt();
                                    list.replace(j, QString::fromUtf8(me.valueToKey(v)));
                                }
                                value = list;
                                found = true;
                                break;
                            }
                        }
                        if (!found)
                            qFatal() << typeName;
                    } else {
                        if (QMetaType::canConvert(mt, QMetaType::fromType<AbstractGadget>())) {
                            qDebug() << value;
                            QVariantList list = value.toList();
                            qDebug() << list;
                            for (int j = 0; j < list.size(); j++) {
                                qDebug() << j << list.at(j) << list.at(j).canConvert<AbstractGadget>();
                                const auto v = list.at(j).value<AbstractGadget>();
                                qDebug() << v;
                                const auto object = v.toJsonObject();
                                qDebug() << object;
                                list.replace(j, object);
                            }
                            value = list;
                        } else {
                            value = value.toList();
                        }
                    }
                }
            } else {
                value = QJsonValue::fromVariant(value);
            }
        }
        ret.insert(name, value.toJsonValue());
    }
    return ret;
}
