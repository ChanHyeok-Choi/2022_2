package hw3

import scala.collection.immutable.HashMap 
import hw3._
import javax.annotation.processing.RoundEnvironment


package object hw3 {
	type Env = HashMap[Var,Val]
	type Loc = Int
  
}

case class Mem(m: HashMap[Loc,Val], top: Loc) {
  def apply(variable: Loc): Val = m(variable)
  def exists(l: Loc): Boolean = m.exists((a: (Loc, Val)) => a._1 == l)
  def add(l: Loc, value: Val) = Mem(m + (l -> value), l)
}

sealed trait Val
case class IntVal(n: Int) extends Val
case class BoolVal(b: Boolean) extends Val
case class ProcVal(v: Var, expr: Expr, env: Env) extends Val
case class RecProcVal(fv: Var, av: Var, body: Expr, env: Env) extends Val
case class LocVal(l: Loc) extends Val


sealed trait Program
sealed trait Expr extends Program
case class Const(n: Int) extends Expr
case class Var(s: String) extends Expr
case class Add(l: Expr, r: Expr) extends Expr
case class Sub(l: Expr, r: Expr) extends Expr
case class Mul(l: Expr, r: Expr) extends Expr
case class Div(l: Expr, r: Expr) extends Expr
case class GTExpr(l: Expr, r: Expr) extends Expr
case class GEQExpr(l: Expr, r: Expr) extends Expr
case class Iszero(c: Expr) extends Expr
case class Ite(c: Expr, t: Expr, f: Expr) extends Expr
case class ValExpr(name: Var, value: Expr, body: Expr) extends Expr
case class VarExpr(name: Var, value: Expr, body: Expr) extends Expr
case class Proc(v: Var, expr: Expr) extends Expr
case class DefExpr(fname: Var, aname: Var, fbody: Expr, ibody: Expr) extends Expr
case class Asn(v: Var, e: Expr) extends Expr
case class Paren(expr: Expr) extends Expr
case class Block(f: Expr, s: Expr) extends Expr
case class PCall(ftn: Expr, arg: Expr) extends Expr






// Writer: 20181257 ChanHyeok Choi
object MiniScalaInterpreter {

	case class UndefinedSemantics(msg: String = "", cause: Throwable = None.orNull) extends Exception("Undefined Semantics: " ++ msg, cause)


	def doInterpret(env: Env, mem: Mem, expr: Expr): Val = EvalwithMem(env, mem, expr)._1


  def EvalwithMem(env: Env, mem: Mem, expr: Expr): (Val, Mem) = expr match {
    case Const(n) => (IntVal(n), mem)
		case Var(s) => {
      val p = env(Var(s)) match {
        // case (x: LocVal) => if (mem.exists(x.l)) mem.apply(x.l) else env(Var(s))
        case (x: LocVal) => mem.apply(x.l)
        case _ => env(Var(s))
      }
      (p, mem)
		}
    case Add(l, r) => {
      val E1 = EvalwithMem(env, mem, l)
      val E2 = EvalwithMem(env, E1._2, r)
      val E = (E1, E2) match {
        case ((x: IntVal, xm: Mem), (y: IntVal, ym: Mem)) => IntVal(x.n + y.n)
        case _ => throw new UndefinedSemantics(s"message ${expr}")
      }
      (E, E2._2)
    }
    case Sub(l, r) => {
      val E1 = EvalwithMem(env, mem, l)
      val E2 = EvalwithMem(env, E1._2, r)
      val E = (E1, E2) match {
        case ((x: IntVal, xm: Mem), (y: IntVal, ym: Mem)) => IntVal(x.n - y.n)
        case _ => throw new UndefinedSemantics(s"message ${expr}")
      }
      (E, E2._2)
    }
    case Mul(l, r) => {
      val E1 = EvalwithMem(env, mem, l)
      val E2 = EvalwithMem(env, E1._2, r)
      val E = (E1, E2) match {
        case ((x: IntVal, xm: Mem), (y: IntVal, ym: Mem)) => IntVal(x.n * y.n)
        case _ => throw new UndefinedSemantics(s"message ${expr}")
      }
      (E, E2._2)
    }
    case Div(l, r) => {
      val E1 = EvalwithMem(env, mem, l)
      val E2 = EvalwithMem(env, E1._2, r)
      val E = (E1, E2) match {
        case ((x: IntVal, xm: Mem), (y: IntVal, ym: Mem)) => IntVal(x.n / y.n)
        case _ => throw new UndefinedSemantics(s"message ${expr}")
      }
      (E, E2._2)
    }
    case GTExpr(l, r) => {
      val E1 = EvalwithMem(env, mem, l)
      val E2 = EvalwithMem(env, E1._2, r)
      val E = (E1, E2) match {
        case ((x: IntVal, xm: Mem), (y: IntVal, ym: Mem)) => BoolVal(x.n > y.n)
        case _ => throw new UndefinedSemantics(s"message ${expr}")
      }
      (E, E2._2)
    }
    case GEQExpr(l, r) => {
      val E1 = EvalwithMem(env, mem, l)
      val E2 = EvalwithMem(env, E1._2, r)
      val E = (E1, E2) match {
        case ((x: IntVal, xm: Mem), (y: IntVal, ym: Mem)) => BoolVal(x.n >= y.n)
        case _ => throw new UndefinedSemantics(s"message ${expr}")
      }
      (E, E2._2)
    }
    case Iszero(c) => {
      val v = EvalwithMem(env, mem, c) match {
        case (x: IntVal, xm: Mem) => (BoolVal(x.n == 0), xm)
        case _ => throw new UndefinedSemantics(s"message ${expr}")
      }
      v
    }
    case Ite(c, t, f) => {
      val E1 = EvalwithMem(env, mem, c)
      val E = E1 match {
        case (x: BoolVal, xm: Mem) => if (x.b) EvalwithMem(env, E1._2, t) else EvalwithMem(env, E1._2, f)
        case _ => throw new UndefinedSemantics(s"message ${expr}")
      }
      E
    }
    case ValExpr(name, value, body) => {
      val E1 = EvalwithMem(env, mem, value)
      val new_env = env + (name -> E1._1)
      EvalwithMem(new_env, E1._2, body)
    }
    case VarExpr(name, value, body) => {
      val E1 = EvalwithMem(env, mem, value)
      val new_env = env + (name -> LocVal(E1._2.top))
      val new_mem = E1._2.add(E1._2.top, E1._1)
      EvalwithMem(new_env, new_mem, body)
    }
    case Proc(v, expr) => (ProcVal(v, expr, env), mem)
    case DefExpr(fname, aname, fbody, ibody) => {
      val new_env = env + (fname -> RecProcVal(fname, aname, fbody, env))
      EvalwithMem(new_env, mem, ibody)
    }
    case Asn(v, e) => {
      val E1 = EvalwithMem(env, mem, e)
      val p = env(v) match {
        case LocVal(l) => l
        case _ => throw new UndefinedSemantics(s"message ${expr}")
      }
      val new_mem = E1._2.add(p, E1._1)
      (E1._1, new_mem)
    }
    case Paren(expr) => EvalwithMem(env, mem, expr)
    case Block(f, s) => {
      val E1 = EvalwithMem(env, mem, f)
      EvalwithMem(env, E1._2, s)
    }
    case PCall(ftn, arg) => {
      val proc = EvalwithMem(env, mem, ftn) match {
        case (ProcVal(v, expr, env_prime), o1: Mem) => {
          val E2 = EvalwithMem(env, o1, arg)
          val new_env = env_prime + (v -> E2._1)
          EvalwithMem(new_env, E2._2, expr)
        }
        case (RecProcVal(fv, av, body, env_prime), o1: Mem) => {
          val E2 = EvalwithMem(env, o1, arg)
          val new_env = env_prime + (av -> E2._1) + (fv -> RecProcVal(fv, av, body, env_prime))
          EvalwithMem(new_env, E2._2, body)
        }
        case _ => throw new UndefinedSemantics(s"message ${expr}")
      }
      proc
    }
    case _ => throw new UndefinedSemantics(s"message ${expr}")
  }

	def apply(program: String): Val = {
		val parsed = MiniScalaParserDriver(program)
		doInterpret(new Env(), Mem(new HashMap[Loc,Val],0), parsed)
	}

}


object Hw3App extends App {
  
	println("Hello from Hw3!")

}