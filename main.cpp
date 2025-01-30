#include <QCoreApplication>
#include <QDebug>
#include "abstractgadget.h"
#include "advancedgadget.h"
#include "compositegadget.h"

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

        qDebug() << "adv1 color:" << adv1.color(); // Should be blue
        qDebug() << "adv2 color:" << adv2.color(); // Should be red
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
        qDebug() << "Original priority:" << composite1.priority();
        qDebug() << "Original color:" << composite1.advanced().color().name();
        qDebug() << "Modified color:" << composite2.advanced().color().name();
    }

    return 0;
}
