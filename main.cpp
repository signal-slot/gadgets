#include <QCoreApplication>
#include <QDebug>
#include "advancedgadget.h"
#include "compositegadget.h"
#include "documentgadget.h"

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
    }
    return 0;
}
