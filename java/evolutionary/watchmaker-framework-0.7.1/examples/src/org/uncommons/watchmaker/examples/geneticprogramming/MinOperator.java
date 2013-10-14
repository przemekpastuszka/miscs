package org.uncommons.watchmaker.examples.geneticprogramming;

public class MinOperator extends BinaryNode {

  public MinOperator(Node left, Node right) {
    super(left, right, 'm');
  }

  @Override
  public double evaluate(double[] programParameters) {
    return Math.min(left.evaluate(programParameters), right.evaluate(programParameters));
  }

  @Override
  public Node simplify() {
    return this;
  }

}
