-- NAMES: DREW GARRISON AND NICK FUNKHOUSER
-- DATE: 3/31/2022

USE TaylorPizzeria;

-- inserting data into baseprice table
INSERT INTO baseprice VALUES ('small', 'Thin', 3, 0.5);
INSERT INTO baseprice VALUES ('small', 'Original', 3, 0.75);
INSERT INTO baseprice VALUES ('small', 'Pan', 3.5, 1);
INSERT INTO baseprice VALUES ('small', 'Gluten-Free', 4, 2);
INSERT INTO baseprice VALUES ('medium', 'Thin', 5, 1);
INSERT INTO baseprice VALUES ('medium', 'Original', 5, 1.5);
INSERT INTO baseprice VALUES ('medium', 'Pan', 6, 2.25);
INSERT INTO baseprice VALUES ('medium', 'Gluten-Free', 6.25, 3);
INSERT INTO baseprice VALUES ('Large', 'Thin', 8, 1.25);
INSERT INTO baseprice VALUES ( 'Large', 'Original', 8, 2);
INSERT INTO baseprice VALUES ( 'Large', 'Pan', 9, 3);
INSERT INTO baseprice VALUES ( 'Large', 'Gluten-Free', 9.5, 4);
INSERT INTO baseprice VALUES ( 'X-Large', 'Thin', 10, 2);
INSERT INTO baseprice VALUES ( 'X-Large', 'Original', 10, 3);
INSERT INTO baseprice VALUES ( 'X-Large', 'Pan', 11.5, 4.5);
INSERT INTO baseprice VALUES ( 'X-Large', 'Gluten-Free', 12.5, 6); 

SET SQL_SAFE_UPDATES = 0;

-- inserting data into topping table
INSERT INTO TOPPING VALUES (1, 'Pepperoni', 1.25, 0.2, 100, 2, 2.75, 3.5, 4.5);
INSERT INTO TOPPING VALUES (2, 'Sausage', 1.25, 0.15, 100, 2.5, 3, 3.5, 4.25);
INSERT INTO TOPPING VALUES (3, 'Ham', 1.5, 0.15, 78, 2, 2.5, 3.25, 4);
INSERT INTO TOPPING VALUES (4, 'Chicken', 1.75, 0.25, 56, 1.5, 2, 2.5, 3);
INSERT INTO TOPPING VALUES (5, 'Green Pepper', 0.5, 0.02, 79, 1, 1.5, 2, 2.5);
INSERT INTO TOPPING VALUES (6, 'Onion', 0.5, 0.02, 85, 1, 1.5, 2, 2.75);
INSERT INTO TOPPING VALUES (7, 'Roma tomato', 0.75, 0.3, 86, 2, 3, 3.5, 4.5);
INSERT INTO TOPPING VALUES (8, 'Mushrooms', 0.75, 0.1, 52, 1.5, 2, 2.5, 3);
INSERT INTO TOPPING VALUES (9, 'Black Olives', 0.6, 0.1, 39, .75, 1, 1.5, 2);
INSERT INTO TOPPING VALUES (10, 'Pineapple', 1, .25, 15, 1, 1.25, 1.75, 2);
INSERT INTO TOPPING VALUES (11, 'Jalapenos', 0.5, 0.05, 64, .5, .75, 1.25, 1.75);
INSERT INTO TOPPING VALUES (12, 'Banana Peppers', 0.5, 0.05, 36, .6, 1, 1.3, 1.75);
INSERT INTO TOPPING VALUES (13, 'Regular Cheese', 1.5, 0.12, 250, 2, 3.5, 5, 7);
INSERT INTO TOPPING VALUES (14, 'Four Cheese Blend', 2, 0.15, 150, 2, 3.5, 5, 7);
INSERT INTO TOPPING VALUES (15, 'Feta Cheese', 2, 0.18, 75, 1.75, 3, 4, 5.5);
INSERT INTO TOPPING VALUES (16, 'Goat Cheese', 2, 0.2, 54, 1.6, 2.75, 4, 5.5);
INSERT INTO TOPPING VALUES (17, 'Bacon', 1.5, 0.25, 89, 1, 1.5, 2, 3);

-- inserting discount data table

INSERT INTO discount VALUES ('employee', .15, NULL);
INSERT INTO discount VALUES ('Lunch Special Medium', NULL, 1);
INSERT INTO discount VALUES ('Lunch Special Large', NULL, 2);
INSERT INTO discount VALUES ('Specialty Pizza', NULL, 1.5);
INSERT INTO discount VALUES ('Gameday Special', .20, NULL);

-- inserting order data




-- Order #1
INSERT INTO customer VALUES(NULL, NULL, NULL, NULL);
INSERT INTO customer_order VALUES (NULL, 1, 'Lunch Special Large' , 'dinein', '12:03', '2022-03-05', NULL, NULL, true);
INSERT INTO pizza VALUES (NULL, 1, 'Pepperoni, Sausage, Extra Cheese' ,'Thin Crust', 'Large' , 13.50, 3.68);
INSERT INTO pizza_order  VALUES (NULL, 1, 1, NULL, NULL, 1);
INSERT INTO dinein VALUES (1, 1, '1,2,3', 14);

-- updating price with discount
UPDATE pizza_order 
SET Price = (Quantity * (SELECT Price FROM pizza WHERE OrderID = 1)) - (Quantity * (SELECT DollarOFF FROM discount WHERE DiscountType = 'Lunch Special Large'))
WHERE PizzaOrderID = 1;

UPDATE pizza_order 
SET Cost = (Quantity * (SELECT Cost FROM pizza WHERE OrderID = 1)) 
WHERE PizzaOrderID = 1;

-- calculating final price and cost
UPDATE customer_order SET TotalPrice = (SELECT SUM(Price)  FROM pizza_order WHERE OrderID = 1 GROUP BY OrderID) WHERE OrderID = 1;
UPDATE customer_order SET TotalCost = (SELECT SUM(Cost) FROM pizza_order WHERE OrderID = 1 GROUP BY OrderID) WHERE OrderID = 1;





-- Order #2


-- 1st customer for order #2
INSERT INTO customer VALUES(NULL, NULL, NULL, NULL);
INSERT INTO customer VALUES(NULL, NULL, NULL, NULL);
INSERT INTO customer_order VALUES (NULL, 2, 'Lunch Special Medium, Specialty Pizza', 'dinein', '12:05', '2022-03-03', NULL, NULL, true);
INSERT INTO pizza VALUES (NULL, 2, 'Feta Cheese, Black Olives, Roma Tomatoes, Mushrooms, Banana Peppers', 'Pan', 'Medium', 10.60, 3.23);
INSERT INTO pizza_order  VALUES (NULL, 2, 2, NULL, NULL, 1);
INSERT INTO dinein VALUES (2, 2, '1', 4);

-- 2nd customer for order #2
INSERT INTO customer_order VALUES (NULL, 3, NULL, 'dinein', '12:05', '2022-03-03', NULL, NULL, true);
INSERT INTO pizza VALUES (NULL, 3, 'Regular Cheese, Chicken, Banana Peppers', 'Original', 'Small', 6.75, 1.40);
INSERT INTO pizza_order  VALUES (NULL, 3, 3, NULL, NULL, 1);
INSERT INTO dinein VALUES (3, 3, '2', 4);

-- Calculating 1st pizza price and cost
UPDATE pizza_order 
SET Price = (Quantity * (SELECT Price FROM pizza WHERE OrderID = 2)) - (Quantity * (SELECT DollarOFF FROM discount WHERE DiscountType = 'Lunch Special Large'))
WHERE PizzaOrderID = 2;

-- Applying first discount
UPDATE pizza_order 
SET Price = Price - (SELECT DollarOFF FROM discount WHERE DiscountType = 'Specialty Pizza')
WHERE OrderID = 2;

-- Applying 2nd discount
UPDATE pizza_order 
SET Cost = (Quantity * (SELECT Cost FROM pizza WHERE OrderID = 2)) 
WHERE PizzaOrderID = 2;

UPDATE customer_order SET TotalPrice = (SELECT SUM(Price) FROM pizza_order WHERE OrderID = 2 GROUP BY OrderID) WHERE OrderID = 2;
UPDATE customer_order SET TotalCost = (SELECT SUM(Cost) FROM pizza_order WHERE OrderID = 2 GROUP BY OrderID) WHERE OrderID =2;

-- Calculating 2nd pizza price and cost
UPDATE pizza_order 
SET Price = (Quantity * (SELECT Price FROM pizza WHERE OrderID = 3))
WHERE PizzaOrderID = 3;

UPDATE pizza_order 
SET Cost = (Quantity * (SELECT Cost FROM pizza WHERE OrderID = 3)) 
WHERE OrderID = 3;

-- calculating final price and cost
UPDATE customer_order SET TotalPrice = (SELECT SUM(Price) FROM pizza_order WHERE OrderID = 3 GROUP BY OrderID) WHERE OrderID = 3;
UPDATE customer_order SET TotalCost = (SELECT SUM(Cost) FROM pizza_order WHERE OrderID = 3 GROUP BY OrderID) WHERE OrderID =3;



-- Order #3
INSERT INTO customer VALUES (NULL,'Andrew Wilkes-Krier', NULL, '864-254-5861');

INSERT INTO customer_order VALUES (NULL, 4, NULL, 'pickup', '21:30', '2022-03-03', NULL, NULL, true);
INSERT INTO pickup VALUES (4, 4);

INSERT INTO pizza VALUES (NULL , 4, 'Pepperoni, Regular Cheese', 'Original', 'Large', 10.75, 3.30);

INSERT INTO pizza_order  VALUES (NULL, 4, 4, NULL, NULL, 6);
UPDATE pizza_order SET Price = ((SELECT Price FROM pizza WHERE OrderID = 4) * Quantity) WHERE PizzaOrderID = 4;
UPDATE pizza_order SET Cost = ((SELECT Cost FROM pizza WHERE OrderID = 4) * Quantity) WHERE PizzaOrderID = 4;

-- calculating final price and cost
UPDATE customer_order SET TotalPrice = (SELECT SUM(Price) FROM pizza_order WHERE OrderID = 4 GROUP BY OrderID) WHERE OrderID = 4;
UPDATE customer_order SET TotalCost = (SELECT SUM(Cost) FROM pizza_order WHERE OrderID = 4 GROUP BY OrderID) WHERE OrderID = 4;






-- Order #4

INSERT INTO customer_order VALUES (NULL, 4, 'Gameday Special, Specialty Pizza', 'delivery', '19:11', '2022-03-05', NULL, NULL, true);
UPDATE customer SET CustomerAddress = '115 Party Blvd, Anderson, SC 29621' WHERE CustomerID = 4;
INSERT INTO delivery VALUES (5, 4);

-- 1st Pizza
INSERT INTO pizza VALUES (NULL, 5, 'Pepperoni, Sausage, Four Cheese Blend', 'Original', 'XLarge', 14.50, 5.59);
INSERT pizza_order VALUES (NULL, 5, 5, NULL, NULL, 1);

-- 2nd Pizza
INSERT INTO pizza VALUES (NULL, 5, 'Ham(extra), Pineapple(extra), Four Cheese Blend', 'Original', 'XLarge', 17.00, 5.59);
INSERT pizza_order VALUES (NULL, 5, 6, NULL, NULL, 1);

-- 3rd Pizza
INSERT INTO pizza VALUES (NULL, 5, 'Jalapeno, Bacon, Four Cheese Blend', 'Original', 'XLarge', 14.00, 5.68);
INSERT pizza_order VALUES (NULL, 5, 7, NULL, NULL, 1);

-- Applying single pizza discount
UPDATE pizza
SET Price = Price - (SELECT DollarOff FROM discount WHERE DiscountType = 'Specialty Pizza') 
WHERE PizzaID = 6;

-- Calculating pizza price before discount
UPDATE pizza_order SET Price = ((SELECT Price FROM pizza WHERE PizzaID = 5) * Quantity) WHERE PizzaOrderID = 5;
UPDATE pizza_order SET Cost = ((SELECT Cost FROM pizza WHERE PizzaID = 5) * Quantity) WHERE PizzaOrderID = 5;

UPDATE pizza_order SET Price = ((SELECT Price FROM pizza WHERE PizzaID = 6) * Quantity) WHERE PizzaOrderID = 6;
UPDATE pizza_order SET Cost = ((SELECT Cost FROM pizza WHERE PizzaID = 6) * Quantity) WHERE PizzaOrderID = 6;

UPDATE pizza_order SET Price = ((SELECT Price FROM pizza WHERE PizzaID = 7) * Quantity) WHERE PizzaOrderID = 7;
UPDATE pizza_order SET Cost = ((SELECT Cost FROM pizza WHERE PizzaID = 7) * Quantity) WHERE PizzaOrderID = 7;

-- calculating final price and cost
UPDATE customer_order SET TotalPrice = (SELECT SUM(Price) FROM pizza_order WHERE OrderID = 5 GROUP BY OrderID) WHERE OrderID = 5;
UPDATE customer_order SET TotalPrice = TotalPrice - (TotalPrice * (SELECT PercentOFF FROM discount WHERE DiscountType = 'Gameday Special') / 100) WHERE OrderID = 5;

UPDATE customer_order SET TotalCost = (SELECT SUM(Cost) FROM pizza_order WHERE OrderID = 5 GROUP BY OrderID) WHERE OrderID = 5;




-- order #5

INSERT INTO customer VALUES (NULL,'Matt Engers', NULL, '864-474-9953');

INSERT INTO customer_order VALUES (NULL, 5, 'Specialty Pizza', 'pickup', '17:30', '2022-03-02', NULL, NULL, true);
INSERT INTO pickup VALUES (6, 5);

INSERT INTO pizza VALUES (NULL , 6, 'Green Pepper, Onion, Roma Tomatoes, Mushrooms, Black Olives, Goat Cheese', 'Gluten-Free', 'XLarge', 16.85, 7.85);

INSERT INTO pizza_order  VALUES (NULL, 6, 8, NULL, NULL, 1);

UPDATE pizza_order SET Price = ((SELECT Price FROM pizza WHERE OrderID = 6) * Quantity) WHERE PizzaOrderID = 8;
UPDATE pizza_order SET Cost = ((SELECT Cost FROM pizza WHERE OrderID = 6) * Quantity) WHERE PizzaOrderID = 8;

-- calculating final price and cost with discount
UPDATE customer_order SET TotalPrice = (SELECT SUM(Price) FROM pizza_order WHERE OrderID = 6 GROUP BY OrderID) WHERE OrderID = 6;
UPDATE customer_order SET TotalPrice = TotalPrice - (SELECT DollarOff FROM discount WHERE DiscountType = 'Specialty Pizza') WHERE OrderID = 6;

UPDATE customer_order SET TotalCost = (SELECT SUM(Cost) FROM pizza_order WHERE OrderID = 6 GROUP BY OrderID) WHERE OrderID = 6;




-- order #6

INSERT INTO customer VALUES (NULL,'Frank Turner', '6745 Wessex St, Anderson, SC 29621', '864-474-9953');

INSERT INTO customer_order VALUES (NULL, 6, 'Specialty Pizza', 'delivery', '18:17', '2022-03-02', NULL, NULL, true);
INSERT INTO delivery VALUES (7, 6);

INSERT INTO pizza VALUES (NULL , 7, 'Chicken, Green Peppers, Onions, Mushrooms, Four Cheese Blend', 'Thin', 'Large', 13.25, 3.20);

INSERT INTO pizza_order  VALUES (NULL, 7, 9, NULL, NULL, 1);

UPDATE pizza_order SET Price = ((SELECT Price FROM pizza WHERE PizzaID = 9) * Quantity) WHERE PizzaOrderID = 9;
UPDATE pizza_order SET Cost = ((SELECT Cost FROM pizza WHERE PizzaID = 9) * Quantity) WHERE PizzaOrderID = 9;

-- updating final price with discount
UPDATE customer_order SET TotalPrice = (SELECT SUM(Price) FROM pizza_order WHERE OrderID = 7 GROUP BY OrderID) WHERE OrderID = 7;
UPDATE customer_order SET TotalPrice = TotalPrice - (SELECT DollarOff FROM discount WHERE DiscountType = 'Specialty Pizza') WHERE OrderID = 7;

-- updating final cost
UPDATE customer_order SET TotalCost = (SELECT SUM(Cost) FROM pizza_order WHERE OrderID = 7 GROUP BY OrderID) WHERE OrderID = 7;




-- order #7

INSERT INTO customer VALUES (NULL,'Milo Auckerman', '8879 Suburban Home, Anderson, SC 29621', '864-878-5679');

INSERT INTO customer_order VALUES (NULL, 7, 'employee', 'delivery', '20:32', '2022-03-06', NULL, NULL, true);
INSERT INTO delivery VALUES (8, 7);

-- pizza #1
INSERT INTO pizza VALUES (NULL , 8, '4 Cheese Blend(extra)', 'Thin', 'Large', 12.00, 3.75);

INSERT INTO pizza_order  VALUES (NULL, 8, 10, NULL, NULL, 1);

-- updating price and cost
UPDATE pizza_order SET Price = (Quantity * (SELECT Price FROM pizza WHERE PizzaID = 10)) WHERE PizzaOrderID = 10;
UPDATE pizza_order SET Cost = (Quantity * (SELECT Cost FROM pizza WHERE PizzaID = 10)) WHERE PizzaOrderID = 10;

-- pizza #2
INSERT INTO pizza VALUES (NULL , 8, 'Pepperoni(Extra), Regular Cheese', 'Thin', 'Large', 12.00, 2.55);
INSERT INTO pizza_order  VALUES (NULL, 8, 11, NULL, NULL, 1);

-- updating the price
UPDATE pizza_order 
SET Price = (Quantity * (SELECT Price FROM pizza WHERE PizzaID = 11)) 
WHERE PizzaOrderID = 11;

-- updating the cost
UPDATE pizza_order 
SET Cost = (Quantity * (SELECT Cost FROM pizza WHERE PizzaID = 11)) 
WHERE PizzaOrderID = 11;

-- calculating final price
UPDATE customer_order 
SET TotalPrice = (SELECT SUM(Price) FROM pizza_order WHERE OrderID = 8 GROUP BY OrderID) 
WHERE OrderID = 8;

-- calculating final price with discount
UPDATE customer_order 
SET TotalPrice = TotalPrice - (TotalPrice * (SELECT PercentOFF FROM discount WHERE DiscountType = 'employee') / 100) 
WHERE OrderID = 8;

-- calculating final cost 
UPDATE customer_order 
SET TotalCost = (SELECT SUM(Cost) FROM pizza_order WHERE OrderID = 8 GROUP BY OrderID) 
WHERE OrderID = 8;


