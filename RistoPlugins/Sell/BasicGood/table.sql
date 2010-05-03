/** @brief
 *
 */

-- This table will contain the description and the code of the basic things
-- used to make advanced goods that can be selled (like pasta, prosciutto,
-- pomodori pelati..)
CREATE TABLE "basic_good"
(
  name CHARACTER VARYING(128) NOT NULL,
  PRIMARY KEY(name)
) WITH (OIDS=FALSE);

