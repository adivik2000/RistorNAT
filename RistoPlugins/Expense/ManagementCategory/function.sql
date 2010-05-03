CREATE TYPE detail_mngm_cost_report AS
(
    Document_date DATE,
    Seller VARCHAR,
    Document_number INTEGER,
    Amount NUMERIC
);

/** @brief Report cost by category from date to date.
  *
  * Use to know how do you spend in each category.
  * @return date of document, seller, document number, sum
  * @param p_date_from Date from
  * @param p_date_to Date to
  * @param p_category Category
  */
CREATE OR REPLACE FUNCTION report_management_category_detailed(p_date_from DATE,
            p_date_to DATE, p_category VARCHAR)
    RETURNS SETOF detail_mngm_cost_report AS $$
        SELECT dpc.document_date, dpc.seller, dpc.document_number, SUM(rpc.price)
        FROM document_purchasing_cost AS dpc, row_purchasing_cost AS rpc WHERE
        rpc.document=dpc.id AND dpc.document_date<=$2 AND
        dpc.document_date>=$1 AND rpc.category=$3 GROUP BY dpc.document_date,
        dpc.seller,dpc.document_number;
$$ LANGUAGE SQL;

CREATE OR REPLACE FUNCTION report_management_category(p_date_from DATE,
            p_date_to DATE, p_category VARCHAR)
    RETURNS NUMERIC AS $$
        SELECT SUM(price) FROM document_purchasing_cost AS d, row_purchasing_cost AS r
        WHERE r.document=d.document_number AND document_date<=$2 AND document_date>=$1;
$$ LANGUAGE SQL;

