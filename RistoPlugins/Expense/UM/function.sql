CREATE TYPE um_related AS (
    base VARCHAR,
    multi NUMERIC
);

/** @brief Get div parameter to port first um to second
   *
   * Use this when you need to port values in the first unit to values in the second.
   *
   *
CREATE OR REPLACE FUNCTION get_div_for_um(p_um1 VARCHAR, p_um2 VARCHAR)
    RETURNS NUMERIC AS $$
    DECLARE
        div NUMERIC;
        um1 RECORD;
        um2 RECORD;
    BEGIN
        SELECT * INTO um1 FROM get_basic_and_mult_for_um(p_um1);
        SELECT * INTO um2 FROM get_basic_and_mult_for_um(p_um2);
        IF um1.base != um2.base THEN
            RAISE EXCEPTION 'No compatible base. Aborting';
        END IF;
        div := um1.multi / um2.multi;
        RETURN div;
    END;
$$ LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION get_basic_and_mult_for_um(p_um VARCHAR)
RETURNS um_related AS $$
    DECLARE
        final um_related;
        looping RECORD;
        um VARCHAR;
        tot_multi NUMERIC;
        it INT;
    BEGIN
        um := p_um;
        tot_multi := 1;
        it := 0;

        LOOP
            SELECT * INTO looping FROM unit_of_measurement AS u
                WHERE u.name=um;
            IF (looping.base IS NULL OR looping.multi IS NULL) THEN
                final.multi := tot_multi;
                final.base := looping.name;
                RETURN final;
            END IF;

            um := looping.base;
            tot_multi := looping.multi * tot_multi;
            it := it + 1;
            IF it > 20 THEN
                RAISE EXCEPTION 'Too much iteration for base for um';
            END IF;
        END LOOP;
    END;
$$ LANGUAGE 'plpgsql';
