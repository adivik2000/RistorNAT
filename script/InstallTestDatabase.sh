#!/bin/bash

psql -U johnny postgres -c "CREATE DATABASE ristornat_test WITH ENCODING='UTF8' OWNER=johnny;"
psql -U johnny ristornat_test -f ../Database/DatabaseSchema.sql
psql -U johnny ristornat_test -f ../Database/DatabaseFn.sql
