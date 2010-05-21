CREATE OR REPLACE FUNCTION article_average_cost(p_article VARCHAR,
                                                p_date_from DATE,p_date_to DATE)
    RETURNS NUMERIC AS $$
    DECLARE
        component RECORD;
        good_um VARCHAR;
        good_avg NUMERIC;
        cost NUMERIC;
        div NUMERIC;
        price NUMERIC;
    BEGIN
        cost := 0;

        FOR component IN SELECT * FROM article_made_up(p_article) LOOP
            SELECT um INTO good_um
                FROM basic_good WHERE name=component.basic_component;
            SELECT * INTO div FROM get_div_for_um(component.um,good_um);
            SELECT * INTO good_avg
                FROM good_average_cost(component.basic_component,
                                       p_date_from,p_date_to);
            price := component.quantity * (good_avg / div);
            cost := cost + price;
        END LOOP;

        RETURN cost;
    END;
$$ LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION article_average_cost(p_article INTEGER)
    RETURNS NUMERIC AS $$
        SELECT * FROM
        article_average_cost((SELECT description FROM good_for_sale WHERE id=$1),
        NULL,NULL);
 $$ LANGUAGE SQL;

CREATE OR REPLACE FUNCTION article_average_cost(p_article INTEGER,p_date_from DATE,p_date_to DATE)
    RETURNS NUMERIC AS $$
        SELECT * FROM
        article_average_cost((SELECT description FROM good_for_sale WHERE id=$1),
        $2,$3);
 $$ LANGUAGE SQL;

CREATE OR REPLACE FUNCTION set_article_average_cost(p_article VARCHAR)
    RETURNS VOID AS $$
        UPDATE good_for_sale SET average_cost=(SELECT * FROM article_average_cost($1,NULL,NULL))
            WHERE description=$1;
$$ LANGUAGE SQL;

CREATE OR REPLACE FUNCTION set_article_average_cost(p_article VARCHAR, p_date_from DATE,p_date_to DATE)
    RETURNS VOID AS $$
        UPDATE good_for_sale SET average_cost=(SELECT * FROM article_average_cost($1,$2,$3))
            WHERE description=$1;
$$ LANGUAGE SQL;

CREATE OR REPLACE FUNCTION set_article_average_cost(p_article INTEGER)
    RETURNS VOID AS $$
        UPDATE good_for_sale SET average_cost=(SELECT * FROM article_average_cost($1))
            WHERE id=$1;
$$ LANGUAGE SQL;

CREATE OR REPLACE FUNCTION set_article_average_cost(p_article INTEGER,p_date_from DATE,p_date_to DATE)
    RETURNS VOID AS $$
        UPDATE good_for_sale SET average_cost=(SELECT * FROM article_average_cost($1,$2,$3))
            WHERE id=$1;
$$ LANGUAGE SQL;
