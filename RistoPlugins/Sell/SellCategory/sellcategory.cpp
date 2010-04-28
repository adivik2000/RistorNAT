#include <QtPlugin>
#include <simpletable.h>
#include "sellcategory.h"

sellCategory::sellCategory(QWidget *parent):pluginInterface(parent)
{
    simpleTable *table = new simpleTable(parent);
    table->setTableName("sell_category");

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(table);

    setLayout(layout);
}
Q_EXPORT_PLUGIN2(sellcategory, sellCategory);
