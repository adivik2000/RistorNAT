/* @brief Document for puchasing cost
 *
 * Dep: Seller, ManagementCategory, BasicGood, UM
 *
 */
CREATE SEQUENCE "row_purchasing_cost_sequence";
CREATE SEQUENCE "row_goods_cost_sequence";
CREATE SEQUENCE "document_goods_cost_sequence";
CREATE SEQUENCE "document_purchasing_cost_sequence";

CREATE TABLE "document_purchasing_cost"
(
  id INTEGER NOT NULL DEFAULT nextval('document_purchasing_cost_sequence'),
  document_date DATE NOT NULL,
  document_number INTEGER NOT NULL,
  seller CHARACTER VARYING REFERENCES supplier(name) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT,
  amount NUMERIC NULL,
  PRIMARY KEY(id)
) WITH (OIDS=FALSE);

CREATE TABLE "row_purchasing_cost"
(
  id INTEGER NOT NULL DEFAULT nextval('row_purchasing_cost_sequence'),
  document INTEGER REFERENCES document_purchasing_cost(id) MATCH SIMPLE
        ON UPDATE CASCADE ON DELETE RESTRICT,
  category CHARACTER VARYING(128) REFERENCES cost_category(description)
        MATCH SIMPLE ON UPDATE CASCADE ON DELETE RESTRICT,
  price NUMERIC NOT NULL,
  PRIMARY KEY(id)
) WITH (OIDS=FALSE);

CREATE TABLE "document_goods_cost"
(
  id INTEGER NOT NULL DEFAULT nextval('document_goods_cost_sequence'),
  document_date DATE NOT NULL,
  document_number INTEGER NOT NULL,
  seller CHARACTER VARYING REFERENCES supplier(name) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT,
  amount NUMERIC NULL,
  PRIMARY KEY(id)
) WITH (OIDS=TRUE);

CREATE TABLE "row_goods_cost"
(
  id INTEGER NOT NULL DEFAULT nextval('row_goods_cost_sequence'),
  document INTEGER REFERENCES document_goods_cost(id) MATCH SIMPLE
      ON UPDATE CASCADE ON DELETE RESTRICT,
  good CHARACTER VARYING(128) REFERENCES basic_good(name)
    MATCH SIMPLE ON UPDATE CASCADE ON DELETE RESTRICT,
  quantity NUMERIC NOT NULL,
  um CHARACTER VARYING(5) REFERENCES unit_of_measurement(short_name) MATCH SIMPLE
    ON UPDATE CASCADE ON DELETE RESTRICT,
  single_price NUMERIC NOT NULL,
  PRIMARY KEY(id)
) WITH (OIDS=FALSE);

