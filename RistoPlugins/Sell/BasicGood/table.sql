/** @brief
 *
 */

-- This table will contain the description and the code of the basic things
-- used to make advanced goods that can be selled (like pasta, prosciutto,
-- pomodori pelati..)
CREATE TABLE "basic_good"
(
  name CHARACTER VARYING(128) NOT NULL,
  um CHARACTER VARYING(5) REFERENCES unit_of_measurement(name) MATCH SIMPLE
    ON UPDATE CASCADE ON DELETE RESTRICT NOT NULL,
  average_cost NUMERIC NULL,
  PRIMARY KEY(name)
) WITH (OIDS=FALSE);

