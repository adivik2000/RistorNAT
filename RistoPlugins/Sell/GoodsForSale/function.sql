CREATE OR REPLACE FUNCTION average_cost(p_article VARCHAR)
    RETURNS NUMERIC AS $$
    DECLARE
        component RECORD;
        cost NUMERIC;
        single_cost NUMERIC;
        multiplicator NUMERIC;
        basic VARCHAR;
    BEGIN
    cost := 0;
    FOR component IN SELECT * FROM article_made_up(p_article) LOOP
        SELECT multi INTO multiplicator FROM unit_of_measurement
            WHERE short_name=component.um;
        SELECT base INTO basic FROM unit_of_measurement
            WHERE short_name=component.um;
        SELECT average_cost INTO single_cost
            FROM basic_good WHERE name=component.basic_component;
            cost := cost + (component.quantity*single_cost);
    END LOOP;
    END;
$$ LANGUAGE 'plpgsql';
