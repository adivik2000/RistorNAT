/* @brief Unit of measurement
 *
 */

CREATE TABLE "basic_unit"
(
    short_name CHARACTER VARYING(5) NOT NULL,
    long_name CHARACTER VARYING(128) NOT NULL,
    PRIMARY KEY(short_name)
);

CREATE TABLE "unit_of_measurement"
(
    short_name CHARACTER VARYING(5) NOT NULL,
    long_name CHARACTER VARYING(128) NOT NULL,
    multi NUMERIC NOT NULL,
    base CHARACTER VARYING(5) REFERENCES basic_unit(short_name) MATCH SIMPLE ON
        UPDATE CASCADE ON DELETE RESTRICT,
    PRIMARY KEY(short_name)
) WITH (OIDS=FALSE);
