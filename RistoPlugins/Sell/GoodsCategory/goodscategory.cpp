
#include "goodscategory.h"

#include <QGridLayout>

#include <simpletable.h>

goodsCategory::goodsCategory(QWidget *parent):pluginInterface(parent)
{
    simpleTable *table = new simpleTable(parent);
    table->setTableName("goods_category");

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(table);

    setLayout(layout);
}

Q_EXPORT_PLUGIN2(goodscategory, goodsCategory);
