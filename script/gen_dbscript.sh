#!/bin/bash

# Configuration
SCHEMA=Database/DatabaseSchema.sql
FN=Database/DatabaseFn.sql
PREFIX=RistoPlugins

# Worker. DO NOT MODIFY UNLESS YOU KNOW WHAT YOU'RE DOING.

if [[ $1 == "-h" ]]; then
	echo "Usage: gendbscript.sh"
	exit 1
fi

if [[ -f $SCHEMA ]]; then
	rm $SCHEMA
fi

if [[ -f $FN ]]; then
	rm $FN
fi

echo -en "CREATE LANGUAGE plpgsql;" >> $FN

for i in \
    $PREFIX/Expense/UM \
    $PREFIX/Expense/Seller \
    $PREFIX/Expense/ManagementCategory \
    $PREFIX/Sell/BasicGood \
    $PREFIX/Expense/Stock \
    $PREFIX/Sell/GoodsCategory \
    $PREFIX/Sell/SellCategory \
    $PREFIX/Sell/GoodsForSale \
    $PREFIX/Sell/ArticleComponent \
    $PREFIX/Expense/ManagementCost \
    $PREFIX/Work/Lunch \
    $PREFIX/Work/Dinner
do
        cat $i/table.sql >> $SCHEMA
	cat $i/function.sql >> $FN
done

echo -en "CREATE TABLE "db_version"
(
  major_version INTEGER,
  minor_version INTEGER,
  patch_version INTEGER,
  PRIMARY KEY(major_version, minor_version, patch_version)
) WITH (OIDS=FALSE);" >> $SCHEMA

echo -en "CREATE OR REPLACE FUNCTION ristornat_db_version()
  RETURNS SETOF db_version AS \$\$ SELECT * FROM db_version; \$\$ LANGUAGE SQL;" >> $FN
