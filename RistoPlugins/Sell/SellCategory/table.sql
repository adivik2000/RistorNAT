-- The table will contain the selling categories of the goods,
-- like Restaurant, Pizzeria, Beverage..
CREATE TABLE "sell_category"
(
  description CHARACTER VARYING(128) NOT NULL,
  PRIMARY KEY(description)
) WITH (OIDS=FALSE);
