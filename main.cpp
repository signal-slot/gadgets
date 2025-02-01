#include <QCoreApplication>
#include <QDebug>
#include "advancedgadget.h"
#include "compositegadget.h"
#include "documentgadget.h"

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


int main(int argc, char *argv[])
{
    qSetMessagePattern("[%{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif}] %{function}:%{line} - %{message}");
    QCoreApplication app(argc, argv);
    
    // Test basic gadget functionality
    {
        AdvancedGadget adv1;
        adv1.setColor(Qt::blue);

        // Implicit sharing test
        AdvancedGadget adv2 = adv1;
        adv2.setColor(Qt::red);

        qDebug() << "adv1:" << adv1;
        qDebug() << "adv2:" << adv2;
    }

    // Test composite gadget
    {
        CompositeGadget composite1;
        composite1.setPriority(100);

        // Set up initial advanced gadget
        AdvancedGadget advanced1;
        advanced1.setColor(Qt::blue);
        composite1.setAdvanced(advanced1);

        // Create a copy of the composite
        CompositeGadget composite2 = composite1;

        // Modify the copy's advanced gadget
        AdvancedGadget advanced2 = composite2.advanced();
        advanced2.setColor(Qt::red);
        composite2.setAdvanced(advanced2);

        // Verify original is unchanged
        qDebug() << "Original:" << composite1;
        qDebug() << "Modified:" << composite2;
    }

    {
        DocumentGadget doc1;
        doc1.header().setContent("Title");
        doc1.body().setContent("Hello World");

        DocumentGadget doc2 = doc1;

        doc2.header().setContent("New Title");

        qDebug() << doc1;
        qDebug() << doc2;
    }

    {
        CompositeGadget composite;
        debugRecursive(&composite);

        DocumentGadget doc;
        doc.header().setContent("Title");
        doc.body().setContent("Hello World");
        debugRecursive(&doc);
    }
    return 0;
}
