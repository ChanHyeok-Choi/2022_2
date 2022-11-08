sealed trait IntList
case object Nil extends IntList
case class Cons(v: Int, t: IntList) extends IntList

sealed trait BTree
case object Leaf extends BTree
case class IntNode(v: Int, left: BTree, right: BTree)
extends BTree

sealed trait Formula
case object True extends Formula
case object False extends Formula
case class Not(f: Formula) extends Formula
case class Andalso(left: Formula, right: Formula) extends Formula
case class Orelse(left: Formula, right: Formula)  extends Formula
case class Implies(left: Formula, right: Formula) extends Formula

object Hw1 extends App {
// Writter : ChanHyeok Choi of 20181257
  println("Hw1!")

  def fibo(n: Int): Int = {
    if (n < 2) 1 else fibo(n - 1) + fibo(n - 2)
  }

  def sum(f: Int=>Int, n: Int): Int = {
    if (n == 1) f(1) else f(n) + sum(f, n - 1)
  }

  def foldRight(init: Int, ftn: (Int, Int)=>Int, list: IntList): Int = list match {
    case Nil => init
    case Cons(h, t) => ftn(foldRight(init, ftn, t), h)
  }

  def filter(f: Int => Boolean, list: IntList): IntList = list match {
    case Nil => Nil
    case Cons(h, t) => if (f(h)) Cons(h, filter(f, t)) else filter(f, t)
  }

  def iter[A](f: A => A, n: Int): A => A = (x: A) => {
    if(n == 1) f(x) else iter[A](f, n - 1)(f(x))
  }
  
  def insert(t: BTree, a: Int): BTree = t match {
    case Leaf => IntNode(a, Leaf, Leaf)
    case IntNode(v, left, right) => if (v > a) IntNode(v, insert(left, a), right) else IntNode(v, left, insert(right, a))
  }

  def eval(f: Formula): Boolean = f match {
    case True => true
    case False => false
    case Not(f) => if (eval(f)) !eval(f) else eval(f)
    case Andalso(left, right) => eval(left) && eval(right)
    case Orelse(left, right) => eval(left) || eval(right)
    case Implies(left, right) => !eval(left) || eval(right)
  }

}