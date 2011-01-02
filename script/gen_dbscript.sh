#!/bin/bash

# Configuration
SCHEMA=Database/DatabaseSchema.sql
FN=Database/DatabaseFn.sql
PREFIX=RistoPlugins

# Worker. DO NOT MODIFY UNLESS YOU KNOW WHAT YOU'RE DOING.

if [[ $1 == "-h" || $1 == "--help" ]]; then
	echo -en "Usage: gendbscript.sh\n"
	echo -en "\tGenerate db script. Launch from topdir\n\t"
	echo -en "and it will saves the script under Database\n"
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

echo -en "\nCREATE TABLE "db_version"
(
  major_version INTEGER,
  minor_version INTEGER,
  patch_version INTEGER,
  PRIMARY KEY(major_version, minor_version, patch_version)
) WITH (OIDS=FALSE);\n" >> $SCHEMA

echo -en "CREATE OR REPLACE FUNCTION ristornat_db_version()
  RETURNS SETOF db_version AS \$\$ SELECT * FROM db_version;\n\$\$ LANGUAGE SQL;\n" >> $FN

echo -en "\nCREATE SEQUENCE "db_log_id_seq";\n" >> $SCHEMA

echo -en "\nCREATE TABLE "db_log"
(
  id bigint DEFAULT NEXTVAL('db_log_id_seq') NOT NULL,
  level character(1) NOT NULL,
  object character varying(100) NOT NULL,
  message character varying(1000) NOT NULL,
  eventdate timestamp without time zone DEFAULT now() NOT NULL,
  CONSTRAINT level_allowed_values CHECK (((("level" = 'I'::bpchar) OR ("level" =
  'W'::bpchar)) OR ("level" = 'E'::bpchar)))
);\n" >> $SCHEMA

echo -en "\nCREATE OR REPLACE FUNCTION ristornat_log(p_lvl CHARACTER,
                                                     p_obj VARCHAR,
                                                     p_msg VARCHAR)
  RETURNS VOID AS \$\$
     INSERT INTO db_log(level,object,message) VALUES (\$1,\$2,\$3);
\$\$ LANGUAGE SQL;\n" >> $FN

