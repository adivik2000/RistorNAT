CREATE OR REPLACE FUNCTION article_made_up(p_selling_good INTEGER)
    RETURNS SETOF article_component AS $$
        SELECT * FROM article_component WHERE selling_component=$1;
$$ LANGUAGE SQL;

CREATE OR REPLACE FUNCTION article_made_up(p_selling_good VARCHAR)
    RETURNS SETOF article_component AS $$
        SELECT * FROM article_component WHERE selling_component=(
            SELECT id FROM good_for_sale WHERE description=$1);
$$ LANGUAGE SQL;
