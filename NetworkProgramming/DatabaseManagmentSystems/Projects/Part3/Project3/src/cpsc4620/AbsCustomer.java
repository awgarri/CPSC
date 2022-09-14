package cpsc4620;

/**
 * An abstract class that includes the ID and customer type that all customers can extend
 *
 * Correspondences: ID = ID Type = type
 */
public abstract class AbsCustomer implements ICustomer {
  protected int ID;
  protected String type;

  @Override
  public int getID() {
    return ID;
  }

  @Override
  public String getType() {
    return type;
  }

}
