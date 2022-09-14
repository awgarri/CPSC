//Name: Drew Garrison
//Date: 4/21/2022

package cpsc4620;

import java.io.IOException;
import java.sql.*;
import java.util.ArrayList;
import java.sql.Timestamp;
import java.text.SimpleDateFormat;


/*
 * This file is where most of your code changes will occur You will write the code to retrieve
 * information from the database, or save information to the database
 * 
 * The class has several hard coded static variables used for the connection, you will need to
 * change those to your connection information
 * 
 * This class also has static string variables for pickup, delivery and dine-in. If your database
 * stores the strings differently (i.e "pick-up" vs "pickup") changing these static variables will
 * ensure that the comparison is checking for the right string in other places in the program. You
 * will also need to use these strings if you store this as boolean fields or an integer.
 * 
 * 
 */

/**
 * A utility class to help add and retrieve information from the database
 */

public final class DBNinja {
  // enter your user name here
  private static String user = "awgarri";
  // enter your password here
  private static String password = "admin1234";
  // enter your database name here
  private static String database_name = "TaylorPizzeria";
  // Do not change the port. 3306 is the default MySQL port
  private static String url = "jdbc:mysql://dbms.ctxudamjuybt.us-east-1.rds.amazonaws.com";
  private static Connection conn;

  // Change these variables to however you record dine-in, pick-up and delivery, and sizes and
  // crusts
  public final static String pickup = "pickup";
  public final static String delivery = "delivery";
  public final static String dine_in = "dine-in";

  public final static String size_s = "Small";
  public final static String size_m = "Medium";
  public final static String size_l = "Large";
  public final static String size_xl = "X-Large";

  public final static String crust_thin = "Thin";
  public final static String crust_orig = "Original";
  public final static String crust_pan = "Pan";
  public final static String crust_gf = "Gluten-Free";

  private static final SimpleDateFormat sdf3 = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");


  /**
   * This function will handle the connection to the database
   *
   * @return true if the connection was successfully made
   * @throws SQLException
   * @throws IOException
   */
  private static boolean connect_to_db() throws SQLException, IOException {
    try {
      Class.forName("com.mysql.cj.jdbc.Driver");
    } catch (ClassNotFoundException e) {
      System.out.println("Could not load the driver");

      System.out.println("Message     : " + e.getMessage());


      return false;
    }

    conn = DriverManager.getConnection(url + "/" + database_name, user, password);
    return true;
  }

  /**
   * @param o order that needs to be saved to the database
   * @throws SQLException
   * @throws IOException
   * @requires o is not NULL. o's ID is -1, as it has not been assigned yet. The pizzas do not exist
   * in the database yet, and the topping inventory will allow for these pizzas to be made
   * @ensures o will be assigned an id and added to the database, along with all of it's pizzas.
   * Inventory levels will be updated appropriately
   */
  public static void addOrder(Order o) throws SQLException, IOException {
    connect_to_db();
    /*
     * add code to add the order to the DB. Remember to add the pizzas and discounts as well, which
     * will involve multiple tables. Customer should already exist. Toppings will need to be added
     * to the pizzas.
     *
     * It may be beneficial to define more functions to add an individual pizza to a database, add a
     * topping to a pizza, etc.
     *
     * Note: the order ID will be -1 and will need to be replaced to be a fitting primary key.
     *
     * You will also need to add timestamps to your pizzas/orders in your database. Those timestamps
     * are not stored in this program, but you can get the current time before inserting into the
     * database
     *
     * Remember, when a new order comes in the ingredient levels for the topping need to be adjusted
     * accordingly. Remember to check for "extra" of a topping here as well.
     *
     * You do not need to check to see if you have the topping in stock before adding to a pizza.
     * You can just let it go negative.
     */

    Timestamp timestamp = new Timestamp(System.currentTimeMillis());
    int ID = -1;
    int customerID = -1;
    String query = "SELECT MAX(OrderID), MAX(CustomerID) FROM customer_order;";
    Statement myStmt = conn.createStatement();
    try {
      ResultSet rSet = myStmt.executeQuery(query);
      while (rSet.next()) {
        ID = rSet.getInt(1);
        customerID = rSet.getInt(2);
      }
    } catch (SQLException e) {
      System.out.println("Error getting max customer ID");
      while (e != null) {
        System.out.println("Message     : " + e.getMessage());
        e = e.getNextException();
      }

      conn.close();
    }

    ID += 1;
    customerID += 1;
    String orderType = o.getType();
    double totalPrice = o.calcPrice();
    String time = sdf3.format(timestamp);


    String addOrder = "INSERT INTO customer_order(OrderID, CustomerID, OrderType, TimeOfOrder, TotalPrice) VALUES (" + ID + ", " + customerID + ", '" + orderType + "', '" + time + "', '" + totalPrice + "');";
    Statement stmt2 = conn.createStatement();
    try {
      stmt2.executeUpdate(addOrder);
    } catch (SQLException i) {
      System.out.println("Error adding order");
      while (i != null) {
        System.out.println("Message     : " + i.getMessage());
        i = i.getNextException();
      }

      conn.close();

    }

    conn.close();
  }


  /**
   * @param p the new customer to add to the database
   * @throws SQLException
   * @throws IOException
   * @requires p is not null. P's ID is -1 and will need to be assigned
   * @ensures p is given an ID and added to the database
   */
  //added helper function
  public static void addPizza(Pizza p) throws SQLException, IOException {
    connect_to_db();
    int pizzaID = -1;

    String query = "SELECT MAX(PizzaID) FROM pizza;";
    Statement Stmt = conn.createStatement();
    try {
      ResultSet rSet = Stmt.executeQuery(query);
      while (rSet.next()) {
        pizzaID = rSet.getInt(1);
      }
    } catch (SQLException e) {
      System.out.println("Error getting max pizza ID");
      while (e != null) {
        System.out.println("Message     : " + e.getMessage());
        e = e.getNextException();
      }
    }
    int orderID = -1;
    String query2 = "SELECT MAX(OrderID) FROM customer_order;";
    Statement myStmt2 = conn.createStatement();
    try {
      ResultSet rSet = myStmt2.executeQuery(query2);
      while (rSet.next()) {
        orderID = rSet.getInt(1);
      }
    } catch (SQLException e) {
      System.out.println("Error getting max order ID");
      while (e != null) {
        System.out.println("Message     : " + e.getMessage());
        e = e.getNextException();
      }


      conn.close();
    }

    pizzaID += 1;
    String crust = p.getCrust();
    String size = p.getSize();
    orderID += 1;

    String addPizza = "INSERT INTO pizza(PizzaID, OrderID, Crust, Size) VALUES (" + pizzaID + ", '" + orderID + "', '" + crust + "', '" + size + "');";
    Statement myStmt = conn.createStatement();

    try {
      myStmt.executeUpdate(addPizza);
    } catch (SQLException e) {
      System.out.println("Error adding pizza");
      while (e != null) {
        System.out.println("Message     : " + e.getMessage());
        e = e.getNextException();
      }
      conn.close();

    }
    conn.close();

  }

  /**
   * @param c the new customer to add to the database
   * @throws SQLException
   * @throws IOException
   * @requires c is not null. C's ID is -1 and will need to be assigned
   * @ensures c is given an ID and added to the database
   */
  public static void addCustomer(ICustomer c) throws SQLException, IOException {
    connect_to_db();
    /*
     * add code to add the customer to the DB. Note: the ID will be -1 and will need to be replaced
     * to be a fitting primary key Note that the customer is an ICustomer data type, which means c
     * could be a dine in, carryout or delivery customer
     */
    int ID = -1;
    String query = "SELECT MAX(CustomerID) FROM customer;";
    Statement myStmt = conn.createStatement();
    try {
      ResultSet rSet = myStmt.executeQuery(query);
      while (rSet.next()) {
        ID = rSet.getInt(1);
      }
    } catch (SQLException e) {
      System.out.println("Error getting max customer ID");
      while (e != null) {
        System.out.println("Message     : " + e.getMessage());
        e = e.getNextException();
      }

      conn.close();
    }
    ID += 1;
    if (c instanceof DeliveryCustomer) {

      DeliveryCustomer cust = (DeliveryCustomer) c;

      String add = cust.getAddress();
      String name = cust.getName();
      String phone = cust.getPhone();

      String addCust = "INSERT INTO customer (CustomerID, CustName, CustomerAddress, CustomerPhone) VALUES(" + ID + ",'" + name + "', '" + add + "', '" + phone + "');";
      Statement stmt2 = conn.createStatement();
      try {
        stmt2.executeUpdate(addCust);
      } catch (SQLException e) {
        System.out.println("Error adding customer");
        while (e != null) {
          System.out.println("Message     : " + e.getMessage());
          e = e.getNextException();
        }

        conn.close();
      }
    } else if (c instanceof DineOutCustomer) {

      DineOutCustomer cust = (DineOutCustomer) c;

      String phone = cust.getPhone();
      String name = cust.getName();
      String addCust = "INSERT INTO customer(CustomerID, CustomerPhone, CustName) VALUES (" + ID + ", '" + phone + "', '" + name + "');";
      Statement stmt2 = conn.createStatement();
      try {
        stmt2.executeUpdate(addCust);
      } catch (SQLException e) {
        System.out.println("Error adding customer");
        while (e != null) {
          System.out.println("Message     : " + e.getMessage());
          e = e.getNextException();
        }

        conn.close();
      }
    } else if (c instanceof DineInCustomer) {

      DineInCustomer cust = (DineInCustomer) c;

      String addCust = "INSERT INTO customer VALUES (" + ID + ",'','','');";
      Statement stmt2 = conn.createStatement();
      try {
        stmt2.executeUpdate(addCust);
      } catch (SQLException e) {
        System.out.println("Error adding customer");
        while (e != null) {
          System.out.println("Message     : " + e.getMessage());
          e = e.getNextException();
        }

        conn.close();
      }


      cust.getTableNum();
      int tableNum = cust.getTableNum();

      String addDineIn = "INSERT INTO dinein(OrderID, TableNumber) VALUES (" + ID + "" + tableNum + ");";
      Statement stmt3 = conn.createStatement();
      try {
        stmt3.executeUpdate(addDineIn);
      } catch (SQLException e) {
        System.out.println("Error adding customer");
        while (e != null) {
          System.out.println("Message     : " + e.getMessage());
          e = e.getNextException();
        }

        conn.close();
      }
    }
    conn.close();
  }

  public static void CompleteOrder(Order o) throws SQLException, IOException {
    connect_to_db();
		      /*add code to mark an order as complete in the DB. You may have a
          boolean field for this, or maybe a completed time timestamp.
          However you have it, */

    ArrayList<Pizza> pz = new ArrayList<Pizza>();
    pz = o.getPizzas();

    Statement stmt = conn.createStatement();
    try {
      int orderID = o.getID();

      String completeOrder = "UPDATE customer_order SET Completed = true WHERE OrderID = '" + orderID + "';";
      stmt.executeUpdate(completeOrder);

    } catch (SQLException e) {
      System.out.println("Error completing order");
      while (e != null) {
        System.out.println("Message     : " + e.getMessage());
        e = e.getNextException();
      }

      conn.close();
    }
    conn.close();
  }

  /**
   * @param t     the topping whose inventory is being replenished
   * @param toAdd the amount of inventory of t to add
   * @throws SQLException
   * @throws IOException
   * @requires t exists in the database and toAdd > 0
   * @ensures t's inventory level is increased by toAdd
   */
  public static void AddToInventory(Topping t, double toAdd) throws SQLException, IOException {
    connect_to_db();
		      /*add code to add toAdd to the inventory level of T. This is not adding a
          new topping, it is adding a certain amount of stock for a topping. This
          would be used to show that an order was made to replenish the restaurants
          supply of pepperoni, etc*/
    String ID = t.getName();
    String addToInventory = "UPDATE TOPPING SET InventoryLevel = InventoryLevel +" + toAdd + " WHERE Topping_Name = '" + ID + "';";

    Statement stmt = conn.createStatement();
    try {
      stmt.executeUpdate(addToInventory);
    } catch (SQLException e) {
      System.out.println("Error updating inventory");
      while (e != null) {
        System.out.println("Message     : " + e.getMessage());
        e = e.getNextException();
      }

      conn.close();
    }
    conn.close();

  }


    /*
        A function to get the list of toppings and their inventory levels.
        I have left this code "complete" as an example of how to use JDBC to
        get data from the database. This query will not work on your database
        if you have different field or table names, so it will need to be
        changed
        Also note, this is just getting the topping ids and then calling
        getTopping() to get the actual topping. You will need to complete
        this on your own
        You don't actually have to use and write the getTopping() function,
        but it can save some repeated code if the program were to expand,
        and it keeps the functions simpler, more elegant and easy to read.
        Breaking up the queries this way also keeps them simpler.
        I think it's a better way to do it, and many people in the industry
        would agree, but its a suggestion, not a requirement.
    */

  /**
   * @return the List of all toppings in the database
   * @throws SQLException
   * @throws IOException
   * @ensures the returned list will include all toppings and accurate
   * inventory levels
   */
  public static ArrayList<Topping> getInventory() throws SQLException, IOException {

    connect_to_db();
    ArrayList<Topping> ts = new ArrayList<Topping>();

    String query = "SELECT Topping_Name FROM TOPPING;";

    Statement stmt = conn.createStatement();
    try {
      ResultSet rSet = stmt.executeQuery(query);

      while (rSet.next()) {
        String ID = rSet.getString(1);

        ts.add(getTopping(ID));
      }
    } catch (SQLException e) {
      System.out.println("Error loading inventory");
      while (e != null) {
        System.out.println("Message     : " + e.getMessage());
        e = e.getNextException();
      }


      conn.close();
      return ts;
    }

    conn.close();
    return ts;
  }

  /**
   * @return a list of all orders that are currently open in the kitchen
   * @throws SQLException
   * @throws IOException
   * @ensures all currently open orders will be included in the returned list.
   */
  public static ArrayList<Order> getCurrentOrders() throws SQLException, IOException
  {
    connect_to_db();
    ArrayList<Order> os = new ArrayList<Order>();
          /*add code to get a list of all open orders. Only return Orders that
          have not been completed. If any pizzas are not completed, then the
          order is open.*/
    String query = "SELECT OrderID FROM customer_order;";
    Statement stmt = conn.createStatement();


    try {
      ResultSet rset = stmt.executeQuery(query);
      while(rset.next()) {
        int ID = rset.getInt(1);
        String query2 = "SELECT Completed FROM customer_order WHERE OrderID = " + ID + ";";
        Statement stmt2 = conn.createStatement();

        try {

          ResultSet rset2 = stmt2.executeQuery(query2);
          boolean iscomplete = true;
          while(rset2.next()) {
            boolean comp = rset2.getBoolean(1);
            if (comp == false) {
              iscomplete = false;
            }
          }
          if (!iscomplete) {
            os.add(getOrder(ID));
          }

        }
        catch (SQLException e) {
          System.out.println("Error loading pizza for order complete");
          while (e != null) {
            System.out.println("Message     : " + e.getMessage());
            e = e.getNextException();
          }
          conn.close();


          return os;
        }
      }
    }

    catch (SQLException e) {

      while (e != null) {

        e = e.getNextException();
      }
      conn.close();
      return os;
    }
    conn.close();
    return os;
  }

  /**
   *
   * @param size the pizza size
   * @param crust the type of crust
   * @return the base price for a pizza with that size and crust
   * @throws SQLException
   * @throws IOException
   * @requires size = size_s || size_m || size_l || size_xl AND crust = crust_thin || crust_orig || crust_pan || crust_gf
   * @ensures the base price for a pizza with that size and crust is returned
   */
  public static double getBasePrice(String size, String crust) throws SQLException, IOException
  {
    connect_to_db();
    double bp = 0.0;

    String query = "SELECT price FROM baseprice WHERE PizzaSize LIKE '" + size + "' AND CrustType LIKE '" + crust + "';";
    System.out.println(query);
    Statement stmt = conn.createStatement();
    try {
      ResultSet rSet = stmt.executeQuery(query);
      while(rSet.next()) {
        bp = rSet.getDouble(1);
      }
    }
    catch (SQLException e) {
      System.out.println("Error loading base price");
      while (e != null) {
        System.out.println("Message     : " + e.getMessage());
        e = e.getNextException();
      }


      conn.close();
      return bp;
    }

    conn.close();
    return bp;
  }

  /**
   *
   * @return the list of all discounts in the database
   * @throws SQLException
   * @throws IOException
   * @ensures all discounts are included in the returned list
   */
  public static ArrayList<Discount> getDiscountList() throws SQLException, IOException
  {
    ArrayList<Discount> discs = new ArrayList<Discount>();
    connect_to_db();

    String query = "SELECT DiscountType FROM discount;";

    Statement stmt = conn.createStatement();
    try {
      ResultSet rSet = stmt.executeQuery(query);

      while(rSet.next()) {
        String ID = rSet.getString(1);
        discs.add(getDiscount(ID));
      }
    }

    catch (SQLException e) {
      System.out.println("Error loading discounts");
      while (e != null) {
        System.out.println("Message     : " + e.getMessage());
        e = e.getNextException();
      }

      conn.close();
      return discs;
    }

    conn.close();
    return discs;
  }

  /**
   *
   * @return the list of all delivery and carry out customers
   * @throws SQLException
   * @throws IOException
   * @ensures the list contains all carryout and delivery customers in the database
   */
  public static ArrayList<ICustomer> getCustomerList() throws SQLException, IOException
  {
    ArrayList<ICustomer> custs = new ArrayList<ICustomer>();
    connect_to_db();

    String query = "SELECT CustomerID FROM customer;";
    Statement stmt = conn.createStatement();
    try {
      ResultSet rSet = stmt.executeQuery(query);
      while(rSet.next())
      {
        int ID = rSet.getInt(1);
        custs.add(getCustomer(ID));
      }
    }
    catch (SQLException e) {
      System.out.println("Error getting customer list");
      while (e != null) {
        System.out.println("Message     : " + e.getMessage());
        e = e.getNextException();
      }

      conn.close();
      return custs;
    }
    conn.close();
    return custs;
  }



  /*
  Note: The following incomplete functions are not strictly required, but could
  make your DBNinja class much simpler. For instance, instead of writing one
  query to get all of the information about an order, you can find the primary
  key of the order, and use that to find the primary keys of the pizzas on that
  order, then use the pizza primary keys individually to build your pizzas.
  We are no longer trying to get everything in one query, so feel free to break
  them up as much as possible
  You could also add functions that take in a Pizza object and add that to the
  database, or take in a pizza id and a topping id and add that topping to the
  pizza in the database, etc. I would recommend this to keep your addOrder
  function much simpler
  These simpler functions should still not be called from our menu class.
  That is why they are private
  We don't need to open and close the connection in these, since they are only
  called by a function that has opened the connection and will close it after
  */

  public static Topping updateTopping(String ID) throws SQLException, IOException
  {
    Topping t = new Topping("", 0.25, 100.0, 4);
    String query2 = "UPDATE TOPPING SET InventoryLevel = InventoryLevel - 3 WHERE Topping_Name = "+ID+";";

    Statement stmt2 = conn.createStatement();
    try {
      ResultSet rSet2 = stmt2.executeQuery(query2);

      while(rSet2.next())
      {

        double price = rSet2.getDouble(1);
        int inv = rSet2.getInt(2);


        t = new Topping(ID, price, inv, 4);
      }

    }
    catch (SQLException e) {
      System.out.println("Error loading Topping");
      while (e != null) {
        System.out.println("Message     : " + e.getMessage());
        e = e.getNextException();
      }


      return t;
    }

    return t;
  }

  private static Topping getTopping(String ID) throws SQLException, IOException
  {

    Topping t = new Topping("", 0.25, 100.0, 4);
    String query = "SELECT Topping_ID, Topping_Price, InventoryLevel FROM TOPPING WHERE Topping_Name ='" + ID + "';";

    Statement stmt = conn.createStatement();
    try {
      ResultSet rSet = stmt.executeQuery(query);

      while(rSet.next())
      {
        int toppingID = rSet.getInt(1);
        double price = rSet.getDouble(2);
        int inv = rSet.getInt(3);


        t = new Topping(ID, price, inv, toppingID);
      }

    }
    catch (SQLException e) {
      System.out.println("Error loading Topping");
      while (e != null) {
        System.out.println("Message     : " + e.getMessage());
        e = e.getNextException();
      }
      conn.close();
      return t;
    }


    return t;

  }

  private static Discount getDiscount(String ID)  throws SQLException, IOException
  {

    Discount D = new Discount("", 0.25, 100.0, 4);
    String query = "SELECT DiscountType, PercentOff, DollarOff FROM discount WHERE DiscountType = '" + ID + "';";

    Statement stmt = conn.createStatement();
    try {
      ResultSet rSet = stmt.executeQuery(query);

      while(rSet.next())
      {
        String tname = rSet.getString(1);
        double percent = rSet.getDouble(2);
        double dollar = rSet.getDouble(3);
        D = new Discount(tname, percent, dollar, 4);
      }

    }
    catch (SQLException e) {
      System.out.println("Error loading Discount");
      while (e != null) {
        System.out.println("Message     : " + e.getMessage());
        e = e.getNextException();
      }

      conn.close();
      return D;
    }

    return D;
  }

  private static Pizza getPizza(String ID)  throws SQLException, IOException
  {

    Pizza P = new Pizza(-1, "", "", -1);

    String query = "SELECT MAX(PizzaID), Size, Crust, Price FROM pizza;";
    Statement stmt = conn.createStatement();


    try {
      ResultSet rSets = stmt.executeQuery(query);
      while(rSets.next()) {
        int pizzaID = rSets.getInt(1);
        String crust = rSets.getString(2);
        String size = rSets.getString(3);
        double price = rSets.getDouble(4);

        P = new Pizza(pizzaID, crust, size, price);
      }
    } catch (SQLException e) {
      System.out.println("Error loading Pizza");
      while (e != null) {
        System.out.println("Message     : " + e.getMessage());
        e = e.getNextException();
      }
    }

    conn.close();
    return P;

  }

  private static ICustomer getCustomer(int ID)  throws SQLException, IOException
  {

    ICustomer C = new DeliveryCustomer(-1, "", "", "");
    String query = "SELECT CustomerPhone, CustName, CustomerAddress FROM customer where CustomerID = " + ID + ";";
    Statement stmt = conn.createStatement();
    try {
      ResultSet rset = stmt.executeQuery(query);
      while(rset.next())
      {
        String phone = rset.getString(1);
        String name = rset.getString(2);
        String add = rset.getString(3);
        if(phone == "" && add == "")
        {

        }
        else if(add != "")
        {
          C = new DeliveryCustomer(ID, name, phone, add);
        }
        else
        {
          C = new DineOutCustomer(ID, name, phone);
        }
      }
    }
    catch (SQLException e) {
      System.out.println("Error getting customer");
      while (e != null) {
        System.out.println("Message     : " + e.getMessage());
        e = e.getNextException();
      }

      conn.close();
      return C;
    }
    return C;
  }

  private static Order getOrder(int ID)  throws SQLException, IOException
  {


    boolean tmp = false;
    String type = "";
    ICustomer c = new DeliveryCustomer(-1, "", "", "");
    Order O = new Order(-1, c, "");
    if (!tmp) {
      String query = "SELECT COUNT(*) FROM dinein WHERE OrderID = " + ID + ";";
      Statement stmt = conn.createStatement();
      try {
        ResultSet rset = stmt.executeQuery(query);
        while(rset.next())
        {
          int count = rset.getInt(1);
          if (count > 0) {
            tmp = true;
            type = dine_in;
          }
        }
      }
      catch (SQLException e) {
        System.out.println("Error with customer for order");
        while (e != null) {
          System.out.println("Message     : " + e.getMessage());
          e = e.getNextException();
        }


        conn.close();
        return O;
      }
    }
    if (!tmp) {
      String query2 = "SELECT COUNT(*), CustomerID FROM pickup WHERE OrderID = " + ID + ";";
      Statement stmt2 = conn.createStatement();
      try {
        ResultSet rset2 = stmt2.executeQuery(query2);
        while(rset2.next())
        {
          int count = rset2.getInt(1);
          int CID = rset2.getInt(2);
          if (count > 0) {
            String query3 = "SELECT CustomerPhone, CustName FROM customer WHERE CustomerID = " + CID + ";";
            Statement stmt3 = conn.createStatement();
            try {
              ResultSet rset3 = stmt3.executeQuery(query3);
              while(rset3.next())
              {
                String phone = rset3.getString(1);
                String name = rset3.getString(2);
                c = new DineOutCustomer(ID, name, phone);
                tmp = true;
                type = pickup;
              }
            }
            catch (SQLException e) {
              System.out.println("Error with customer for order");
              while (e != null) {
                System.out.println("Message     : " + e.getMessage());
                e = e.getNextException();
              }

              conn.close();
              return O;
            }
          }
        }
      }
      catch (SQLException e) {
        System.out.println("Error with customer for order");
        while (e != null) {
          System.out.println("Message     : " + e.getMessage());
          e = e.getNextException();
        }

        conn.close();
        return O;
      }
    }
    if (!tmp) {
      String query4 = "SELECT COUNT(*), CustomerID FROM delivery WHERE OrderID = " + ID + ";";
      Statement stmt4 = conn.createStatement();
      try {
        ResultSet rset4 = stmt4.executeQuery(query4);
        while(rset4.next())
        {
          int cnt = rset4.getInt(1);
          int CID = rset4.getInt(2);
          if (cnt > 0) {
            String query5 = "SELECT CustomerPhone, CustName, CustomerAddress FROM customer WHERE CustomerID = " + CID + ";";
            Statement stmt5 = conn.createStatement();
            try {
              ResultSet rset5 = stmt5.executeQuery(query5);
              while(rset5.next())
              {
                String phone = rset5.getString(1);
                String name = rset5.getString(2);
                String add = rset5.getString(3);
                c = new DeliveryCustomer(ID, name, phone, add);
                type = delivery;
              }
            }
            catch (SQLException e) {
              System.out.println("Error with customer for order");
              while (e != null) {
                System.out.println("Message     : " + e.getMessage());
                e = e.getNextException();
              }

              conn.close();
              return O;
            }
          }
        }
      }
      catch (SQLException e) {
        System.out.println("Error with customer for order");
        while (e != null) {
          System.out.println("Message     : " + e.getMessage());
          e = e.getNextException();
        }

        conn.close();
        return O;
      }
    }
    O = new Order(ID, c, type);
    Statement stmt6 = conn.createStatement();
    try {
      String query6 = "SELECT DiscountType FROM customer_order WHERE OrderID = "+ID+";";
      ResultSet rset6 = stmt6.executeQuery(query6);
      while(rset6.next())
      {
        String DID = rset6.getString(1);
        O.addDiscount(getDiscount(DID));
      }
    }
    catch (SQLException e) {
      System.out.println("Error applying discount to order");
      while (e != null) {
        System.out.println("Message     : " + e.getMessage());
        e = e.getNextException();
      }

      conn.close();
      return O;
    }
    Statement stmt7 = conn.createStatement();
    try {
      String query7 = "SELECT PizzaID FROM pizza WHERE OrderID = "+ID+";";
      ResultSet rset7 = stmt7.executeQuery(query7);
      while(rset7.next())
      {
        String PID = rset7.getString(1);
        O.addPizza(getPizza(PID));
      }
    }
    catch (SQLException e) {

      while (e != null) {

        e = e.getNextException();
      }

      conn.close();
      return O;
    }

    return O;
  }

}