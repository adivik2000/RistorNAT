/* @brief Unit of measurement
 *
 */

CREATE TABLE "unit_of_measurement"
(
    name CHARACTER VARYING(20) NOT NULL,
    base CHARACTER VARYING(10) REFERENCES unit_of_measurement(name) MATCH SIMPLE ON
        UPDATE CASCADE ON DELETE RESTRICT,
    multi NUMERIC NULL,
    PRIMARY KEY(name)
) WITH (OIDS=FALSE);
