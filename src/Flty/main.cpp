#include <queue>
#include <list>
#include <iostream>
#include <type_traits>
#include <memory>

#include "Main.hpp"
#include "LWindow.h"
#include "LLabel.h"
#include "SkColor.h"
#include "LApplication.h"


int Main(void* platformData, int argc, lstring *argv)
{
    lwindow_sptr win = std::make_shared<LWindow>(platformData);
    lwidget_sptr widget{ new LWidget() };
    lwidget_sptr widget1{ new LWidget() };
    llabel_sptr widget2{ new LLabel() };

    widget2->setBackgroundColor(SK_ColorGREEN);
    widget2->setText("12345678910");

    widget1->setBackgroundColor(SK_ColorBLUE);
    widget1->setSizePolicy(LSizePolicy::Expanding, LSizePolicy::Expanding);
    widget1->setBorder(2, LBorderStyle::Solid, SK_ColorBLACK);
    widget1->setBorderRadius(8);
    widget1->setMargin({10});

    widget->setBackgroundColor(SK_ColorRED);
    widget->setFixedSize(SkISize::Make(800, 600));

    widget->addChildWidget(widget1);
    widget->addChildWidget(widget2);
    widget->setBox(LBoxType::Horizontal);
    widget->setBorder(12, LBorderStyle::Solid, SK_ColorBLACK);
    widget->setPadding({10, 1, 20, 4});
    win->addRootChild(widget);

    win->show();
    return lApp->exec();
}