#ifndef DOCUMENTGADGET_H
#define DOCUMENTGADGET_H

#include "textgadget.h"

// DocumentGadget.h
class DocumentGadget : public AbstractGadget {
    Q_GADGET
    Q_PROPERTY(TextGadget header READ header WRITE setHeader)
    Q_PROPERTY(TextGadget body READ body WRITE setBody)

public:
    DocumentGadget() : AbstractGadget(new Private) {}
    const QMetaObject* metaObject() const override { return &staticMetaObject; }

    TextGadget header() const {
        return d<Private>()->header;
    }

    TextGadget &header() {
        return d<Private>()->header;
    }

    void setHeader(const TextGadget& header) {
        if (this->header() == header) return;
        d<Private>()->header = header;
    }

    TextGadget body() const {
        return d<Private>()->body;
    }

    TextGadget &body() {
        return d<Private>()->body;
    }

    void setBody(const TextGadget& body) {
        if (this->body() == body) return;
        d<Private>()->body = body;
    }

private:
    struct Private : public AbstractGadget::Private {
        TextGadget header;
        TextGadget body;

        Private() = default;
        Private(const Private& other)
            : AbstractGadget::Private(other)
            , header(other.header)
            , body(other.body)
        {}
    };
};

Q_DECLARE_METATYPE(DocumentGadget)

#endif // DOCUMENTGADGET_H
