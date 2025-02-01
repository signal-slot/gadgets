#ifndef SPECIALTEXT_H
#define SPECIALTEXT_H

#include "textgadget.h"

class SpecialText : public TextGadget
{
    Q_GADGET
    Q_PROPERTY(QString format READ format WRITE setFormat)

public:
    SpecialText() : TextGadget(new Private) {}

    const QMetaObject* metaObject() const override { return &staticMetaObject; }

    QString format() const {
        return d<Private>()->format;
    }

    void setFormat(const QString &format) {
        if (this->format() == format) return;
        d<Private>()->format = format;
    }

private:
    struct Private : public TextGadget::Private {
        QString format;

        Private() = default;
        Private(const Private& other)
            : TextGadget::Private(other)
            , format(other.format)
        {}
    };
};

#endif // SPECIALTEXT_H
