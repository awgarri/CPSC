-- NAMES: DREW GARRISON AND NICK FUNKHOUSER
-- DATE: 3/31/2022

CREATE SCHEMA IF NOT EXISTS TaylorPizzeria;

USE TaylorPizzeria;

CREATE TABLE IF NOT EXISTS customer_order (
OrderID  INT NOT NULL AUTO_INCREMENT ,
CustomerID INT ,
DiscountType VARCHAR(255) ,
OrderType VARCHAR(255) ,
TimeOfOrder VARCHAR(255) ,
OrderDate  DATE ,
TotalCost DOUBLE(10,2)  ,
TotalPrice DOUBLE(10,2),
Completed BOOLEAN  , 
PRIMARY KEY (OrderID)
);

CREATE TABLE IF NOT EXISTS customer (
CustomerID INT NOT NULL AUTO_INCREMENT,
CustName VARCHAR(255),
CustomerAddress VARCHAR(255),
CustomerPhone VARCHAR(255),
PRIMARY KEY (CustomerID)
);

CREATE TABLE IF NOT EXISTS pizza_order (
PizzaOrderID INT NOT NULL AUTO_INCREMENT,
OrderID INT,
PizzaID INT,
Price DOUBLE (10,2),
Cost DOUBLE (10,2),
Quantity INT,
PRIMARY KEY(PizzaOrderID)
);

CREATE TABLE IF NOT EXISTS discount (
DiscountType  VARCHAR(255) ,
PercentOff DOUBLE(6,2) , 
DollarOff DOUBLE(6,2) ,
PRIMARY KEY (DiscountType)
);

CREATE TABLE IF NOT EXISTS pizza (
PizzaID  INT NOT NULL AUTO_INCREMENT ,
OrderID INT,
Topping VARCHAR(255) ,
Crust VARCHAR(255) ,
Size VARCHAR(255) ,
Price DOUBLE(6,2),
Cost DOUBLE(6,2),
PRIMARY KEY (PizzaID)
);

CREATE TABLE IF NOT EXISTS TOPPING (
Topping_ID INT,
Topping_Name VARCHAR(255),
Topping_Price DOUBLE(5,2),
Cost DOUBLE(5,2),
InventoryLevel DOUBLE(5,1),
Small DOUBLE(5,2),
Med DOUBLE(5,2),
Large DOUBLE(5,2),
XLarge DOUBLE(5,2),
PRIMARY KEY(Topping_ID)
);



CREATE TABLE IF NOT EXISTS baseprice (
PizzaSize VARCHAR(255) ,
CrustType VARCHAR(255) , 
Price  DOUBLE(5,2) ,
Cost DOUBLE(5,2)
);

CREATE TABLE IF NOT EXISTS delivery (
OrderID INT ,
CustomerID INT,
PRIMARY KEY (OrderID) 
);

CREATE TABLE IF NOT EXISTS pickup (
OrderID  INT ,
CustomerID INT,
PRIMARY KEY (OrderID) 
);

CREATE TABLE IF NOT EXISTS dinein (
OrderID  INT ,
CustomerID INT,
SeatNumber VARCHAR(255),
TableNumber INT,
PRIMARY KEY (OrderID) 
);



ALTER TABLE pizza_order
ADD FOREIGN KEY (OrderID) REFERENCES customer_order(OrderID),
ADD FOREIGN KEY (PizzaID) REFERENCES pizza(PizzaID);

ALTER TABLE delivery
ADD FOREIGN KEY (OrderID) REFERENCES customer_order(OrderID),
ADD FOREIGN KEY (CustomerID) REFERENCES customer(CustomerID);

ALTER TABLE pickup
ADD FOREIGN KEY (OrderID) REFERENCES customer_order(OrderID),
ADD FOREIGN KEY (CustomerID) REFERENCES customer(CustomerID);

ALTER TABLE dinein
ADD FOREIGN KEY (OrderID) REFERENCES customer_order(OrderID),
ADD FOREIGN KEY (CustomerID) REFERENCES customer(CustomerID);
