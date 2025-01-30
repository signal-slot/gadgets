#ifndef DOCUMENTGADGET_H
#define DOCUMENTGADGET_H

#include "textgadget.h"

// DocumentGadget.h
class DocumentGadget : public AbstractGadget {
    Q_GADGET
    Q_PROPERTY(TextGadget header READ header WRITE setHeader)
    Q_PROPERTY(TextGadget body READ body WRITE setBody)

public:
    DocumentGadget() : AbstractGadget(new Data) {}
    const QMetaObject* metaObject() const override { return &staticMetaObject; }

    TextGadget header() const {
        return D<Data>()->header;
    }

    void setHeader(const TextGadget& header) {
        if (this->header() == header) return;
        D<Data>()->header = header;
    }

    TextGadget body() const {
        return D<Data>()->body;
    }

    void setBody(const TextGadget& body) {
        if (this->body() == body) return;
        D<Data>()->body = body;
    }

private:
    struct Data : public AbstractGadget::Data<Data> {
        TextGadget header;
        TextGadget body;

        Data() = default;
        Data(const Data& other)
            : AbstractGadget::Data<Data>(other)
            , header(other.header)
            , body(other.body)
        {}
    };
};

Q_DECLARE_METATYPE(DocumentGadget)

#endif // DOCUMENTGADGET_H
