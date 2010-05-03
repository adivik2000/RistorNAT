/** @brief Article Component
 *
 * Dep: BasicGoods, GoodsForSale, UM
 */
-- This table will contain the reference if a selling article is maked from one
-- or more basic component.
CREATE TABLE "article_component"
(
  basic_component CHARACTER VARYING(128) REFERENCES basic_good(description) NOT NULL,
  selling_component INTEGER REFERENCES good_for_sale(id)
    MATCH SIMPLE ON UPDATE CASCADE ON DELETE RESTRICT,
  quantity NUMERIC NOT NULL,
  um CHARACTER VARYING(5) REFERENCES unit_of_measurement(short_name) MATCH SIMPLE
    ON UPDATE CASCADE ON DELETE RESTRICT,
  PRIMARY KEY(basic_component,selling_component)
) WITH (OIDS=TRUE);
