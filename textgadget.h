#ifndef TEXTGADGET_H
#define TEXTGADGET_H

#include "abstractgadget.h"

// 新しい派生クラス例: TextGadget
class TextGadget : public AbstractGadget
{
    Q_GADGET
    Q_PROPERTY(QString content READ content WRITE setContent)

public:
    TextGadget() : AbstractGadget(new Private) {}
protected:
    TextGadget(TextGadget::Private *d) : AbstractGadget(d) {}
public:
    const QMetaObject* metaObject() const override { return &staticMetaObject; }

    QString content() const {
        return d<Private>()->content;
    }

    QString &content() {
        return d<Private>()->content;
    }

    void setContent(const QString& content) {
        if (this->content() == content) return;
        d<Private>()->content = content;
    }

protected:
    struct Private : public AbstractGadget::Private {
        QString content;

        Private() = default;
        Private(const Private& other)
            : AbstractGadget::Private(other)
            , content(other.content)
        {}
    };
};

Q_DECLARE_METATYPE(TextGadget)
#endif // TEXTGADGET_H
