CREATE OR REPLACE FUNCTION test_UM_init()
    RETURNS BOOLEAN AS $$
    DECLARE
    BEGIN
       INSERT INTO unit_of_measurement(name,multi) VALUES ('g', 1);
       INSERT INTO unit_of_measurement(name,multi) VALUES ('lt', 1);
       INSERT INTO unit_of_measurement(name,multi) VALUES ('m', 1);
       INSERT INTO unit_of_measurement(name,base,multi) VALUES ('kg','g', 1000);
       INSERT INTO unit_of_measurement(name,base,multi) VALUES ('hg','g', 100);
       INSERT INTO unit_of_measurement(name,base,multi) VALUES ('mg','g', 0.001);
       INSERT INTO unit_of_measurement(name,base,multi) VALUES ('dl','lt', 0.1);
       INSERT INTO unit_of_measurement(name,base,multi) VALUES ('ton','kg', 1000);
       INSERT INTO unit_of_measurement(name,base,multi) VALUES ('m2','m', 1000);
       INSERT INTO unit_of_measurement(name,base,multi) VALUES ('dm2','m2', 1000);

        RETURN TRUE;
   END;
$$ LANGUAGE 'plpgsql';

/** @brief Testsuite for UM
  *
  * Needs to test:
  * *) get_basic_and_mult_for_um(p_um VARCHAR) => um_related
  * *) get_div_for_um(p_um1 VARCHAR, p_um2 VARCHAR)  => NUMERIC
  *
  * The type um_related is (base VARCHAR, multi NUMERIC).
  */
CREATE OR REPLACE FUNCTION test_UM_do()
    RETURNS BOOLEAN AS $$
    DECLARE
        result UM_RELATED;
        r_num NUMERIC;
    BEGIN

        SELECT * INTO r_num FROM get_div_for_um('kg', 'g');
        IF r_num IS NULL OR r_num != 1000 THEN
            RAISE WARNING 'Error in get_div_for_um 1, got % expected 1000', r_num;
            RETURN FALSE;
        END IF;

        SELECT * INTO r_num FROM get_div_for_um('g', 'kg');
        IF r_num IS NULL OR r_num != 0.001 THEN
            RAISE WARNING 'Error in get_div_for_um 2, got % expected 0.001', r_num;
            RETURN FALSE;
        END IF;

        SELECT * INTO r_num FROM get_div_for_um('ton', 'kg');
        IF r_num IS NULL OR r_num != 1000 THEN
            RAISE WARNING 'Error in get_div_for_um 3, got % expected 1000', r_num;
            RETURN FALSE;
        END IF;

        SELECT * INTO r_num FROM get_div_for_um('kg', 'ton');
        IF r_num IS NULL OR r_num != 0.001 THEN
            RAISE WARNING 'Error in get_div_for_um 4, got % expected 0.001', r_num;
            RETURN FALSE;
        END IF;

        SELECT * INTO r_num FROM get_div_for_um('dm2', 'm2');
        IF r_num IS NULL OR r_num != 1000 THEN
            RAISE WARNING 'Error in get_div_for_um 5, got % expected 1000', r_num;
            RETURN FALSE;
        END IF;

        SELECT * INTO result FROM get_basic_and_mult_for_um('kg');
        IF result IS NULL OR result.base != 'g' OR result.multi != 1000 THEN
            RAISE WARNING 'Error in get_basic_and_mult_for_um 1';
            RAISE WARNING 'got <%,%>, expected <g,1000>', result.base, result.multi;
            RETURN FALSE;
        END IF;

        SELECT * INTO result FROM get_basic_and_mult_for_um('g');
        IF result IS NULL OR result.base != 'g' OR result.multi != 1 THEN
            RAISE WARNING 'Error in get_basic_and_mult_for_um 2';
            RAISE WARNING 'got <%,%>, expected <g,1>', result.base, result.multi;
            RETURN FALSE;
        END IF;

        SELECT * INTO result FROM get_basic_and_mult_for_um('dm2');
        IF result IS NULL OR result.base != 'm' OR result.multi != 1000000 THEN
            RAISE WARNING 'Error in get_basic_and_mult_for_um 3';
            RAISE WARNING 'got <%,%>, expected <m2,1000>', result.base, result.multi;
            RETURN FALSE;
        END IF;

        SELECT * INTO result FROM get_basic_and_mult_for_um('ton');
        IF result IS NULL OR result.base != 'g' OR result.multi != 1000000 THEN
            RAISE WARNING 'Error in get_basic_and_mult_for_um 4';
            RAISE WARNING 'got <%,%>, expected <g,1000>', result.base, result.multi;
            RETURN FALSE;
        END IF;

        RETURN TRUE;
    END;
$$ LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION test_UM_clean()
    RETURNS BOOLEAN AS $$
    DECLARE
    BEGIN
        DELETE FROM unit_of_measurement;
        RETURN TRUE;
    END;
$$ LANGUAGE 'plpgsql';
