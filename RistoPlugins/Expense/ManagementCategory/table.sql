-- The table will contain the category of the management costs, like water,
-- electricity, etc..
CREATE TABLE "cost_category"
(
  description CHARACTER VARYING(128) NOT NULL,
  PRIMARY KEY (description)
) WITH (OIDS=FALSE);

