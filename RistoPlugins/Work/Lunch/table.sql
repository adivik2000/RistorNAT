/** @brief
 *
 * Dep: GoodsForSale, ArticleComponent
 */

-- This table contain the places and the date for every lunch.
CREATE TABLE "lunch"
(
  lunch_date DATE NOT NULL,
  places INTEGER NOT NULL,
  PRIMARY KEY (lunch_date)
) WITH (OIDS=FALSE);

CREATE TYPE articles_sold AS
(
  Article_Number INTEGER,
  Article VARCHAR,
  Quantity INTEGER,
  Sell_price NUMERIC
);

-- This table contain the reference of what composed goods are sold in a certain
-- lunch
CREATE TABLE "l_sales"
(
  sales_date DATE REFERENCES lunch(lunch_date)
    MATCH SIMPLE ON UPDATE CASCADE ON DELETE RESTRICT,
  article INTEGER REFERENCES good_for_sale(id)
    MATCH SIMPLE ON UPDATE CASCADE ON DELETE RESTRICT,
  quantity_sold INTEGER NOT NULL,
  sell_price NUMERIC NOT NULL,
  PRIMARY KEY(sales_date,article)
) WITH (OIDS=FALSE);

