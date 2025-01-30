#ifndef TEXTGADGET_H
#define TEXTGADGET_H

#include "abstractgadget.h"

// 新しい派生クラス例: TextGadget
class TextGadget : public AbstractGadget
{
    Q_GADGET
    Q_PROPERTY(QString content READ content WRITE setContent)

public:
    TextGadget() : AbstractGadget(new Data) {}
    TextGadget(const TextGadget&) = default;
    TextGadget& operator=(const TextGadget&) = default;

    QString content() const {
        return D<Data>()->content;
    }

    void setContent(const QString& str) {
        D<Data>()->content = str;
    }

private:
    struct Data : public AbstractGadget::Data<Data> {
        QString content;

        Data() = default;
        Data(const Data& other)  // 明示的なコピーコンストラクタ
            : AbstractGadget::Data<Data>(other),
            content(other.content)
        {}
    };
};

Q_DECLARE_METATYPE(TextGadget)
#endif // TEXTGADGET_H
