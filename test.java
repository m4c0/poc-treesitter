package org.mycompany.myproject.MyClass;

import java.io.IOException;

public class Test {
  public static void main(String args[]) throws IOException {
    try {
      try (var x = openSomething()) {
        String s = "ok";
        (x -> x)(s);
      } catch (NullPointerException e) {
      }
    } catch (Throwable e) {
      throw new IOException(e);
    }
  }
}
