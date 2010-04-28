-- The table will contain the categories of the selling goods, like
-- (Primi, Secondi, Antipasti..)
CREATE TABLE "goods_category"
(
  description CHARACTER VARYING(128) NOT NULL,
  PRIMARY KEY(description)
) WITH (OIDS=FALSE);
