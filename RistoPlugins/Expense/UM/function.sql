CREATE TYPE um_related AS (
    base VARCHAR,
    multi NUMERIC
);

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
