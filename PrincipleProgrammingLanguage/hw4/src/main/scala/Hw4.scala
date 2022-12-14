package hw4

import scala.collection.immutable.HashMap 
import hw4._
import scala.collection.mutable.ReusableBuilder
import com.ibm.icu.text.AlphabeticIndex.Record


package object hw4 {
  type Env = HashMap[Var,LocVal]
}

case class Mem(m: HashMap[LocVal,Val], top: Int) {
  def extended(v: Val): (Mem, LocVal) = {
    val new_mem = Mem(m.updated(LocVal(top),v), top+1)
    (new_mem,LocVal(top))
  }
  def updated(l: LocVal, new_val: Val): Option[Mem] = {
    m.get(l) match {
      case Some(v) => Some(Mem(m.updated(l, new_val), top))
      case None => None
    }
  }
  def get(l: LocVal): Option[Val] = m.get(l)
  def getLocs(): List[LocVal] = m.keySet.toList
}

sealed trait Val
case object SkipVal extends Val
case class IntVal(n: Int) extends Val
case class BoolVal(b: Boolean) extends Val
case class ProcVal(args: List[Var], expr: Expr, env: Env) extends Val
case class LocVal(l: Int) extends Val
sealed trait RecordValLike extends Val
case object EmptyRecordVal extends RecordValLike
case class RecordVal(field: Var, loc: LocVal, next: RecordValLike) extends RecordValLike


sealed trait Program
sealed trait Expr extends Program
case object Skip extends Expr
case object False extends Expr
case object True extends Expr
case class NotExpr(expr: Expr) extends Expr
case class Const(n: Int) extends Expr
case class Var(s: String) extends Expr {
  override def toString = s"Var(${"\""}${s}${"\""})"
}
case class Add(l: Expr, r: Expr) extends Expr
case class Sub(l: Expr, r: Expr) extends Expr
case class Mul(l: Expr, r: Expr) extends Expr
case class Div(l: Expr, r: Expr) extends Expr
case class LTEExpr(l: Expr, r: Expr) extends Expr
case class EQExpr(l: Expr, r: Expr) extends Expr
case class Iszero(c: Expr) extends Expr
case class Ite(c: Expr, t: Expr, f: Expr) extends Expr
case class Let(i: Var, v: Expr, body: Expr) extends Expr
case class Proc(args: List[Var], expr: Expr) extends Expr
case class Asn(v: Var, e: Expr) extends Expr
case class BeginEnd(expr: Expr) extends Expr
case class FieldAccess(record: Expr, field: Var) extends Expr
case class FieldAssign(record: Expr, field: Var, new_val: Expr) extends Expr
case class Block(f: Expr, s: Expr) extends Expr
case class PCallV(ftn: Expr, arg: List[Expr]) extends Expr
case class PCallR(ftn: Expr, arg: List[Var]) extends Expr
case class WhileExpr(cond: Expr, body: Expr) extends Expr
sealed trait RecordLike extends Expr
case object EmptyRecordExpr extends RecordLike
case class RecordExpr(field: Var, initVal: Expr, next: RecordLike) extends RecordLike







// Writer: 20181257 Chanhyeok Choi
object MiniCInterpreter {

  case class Result(v: Val, m: Mem)
  case class UndefinedSemantics(msg: String = "", cause: Throwable = None.orNull) extends Exception("Undefined Semantics: " ++ msg, cause)
    
  
  def UpdatePCALLV(exprs: List[Expr], xs: List[Var], env: Env, _env: Env, _mem: Mem): (Env, Mem) = exprs match {
    case Nil => (_env, _mem)
    case head_E :: next_E => {
      val E_n = eval(env, _mem, head_E)
      xs match {
        case Nil => throw new UndefinedSemantics(s"message $E_n}")
        case head_x :: next_x => {
          val (new_mem, new_loc) = E_n.m.extended(E_n.v)
          val new_env = _env + (head_x -> new_loc)
          UpdatePCALLV(next_E, next_x, env, new_env, new_mem)
        }
      }
    }
  }

  def UpdateEnvPCALLR(ys: List[Var], xs: List[Var], env: Env, _env: Env): Env = ys match {
    case Nil => _env
    case head_y :: next_y => {
      env.get(head_y) match {
        case None => throw new UndefinedSemantics(s"message ${head_y}")
        case Some(value) => {
          xs match {
            case Nil => throw new UndefinedSemantics(s"message ${xs}")
            case head_x :: next_x => {
              val new_env = _env + (head_x -> value)
              UpdateEnvPCALLR(next_y, next_x, env, new_env)
            }
          }
        }
      }
    }
  }

  def findVar(r: RecordValLike, x: Var): LocVal = r match {
    case EmptyRecordVal => throw new UndefinedSemantics(s"message ${r}")
    case RecordVal(field, loc, next) => if (field == x) loc else findVar(next, x)
  }

  def eval(env: Env, mem: Mem, expr: Expr): Result = expr match {
    case Skip => Result(SkipVal, mem)
    case False => Result(BoolVal(false), mem)
    case True => Result(BoolVal(true), mem)
    case NotExpr(expr_prime) => {
      val E = eval(env, mem, expr_prime) match {
        case Result(v, m) => v match {
          case BoolVal(b) => if (b == true) Result(BoolVal(false), m) else Result(BoolVal(true), m)
          case _ => throw new UndefinedSemantics(s"message ${expr}")
        }
      }
      E
    }
    case Const(n) => Result(IntVal(n), mem)
    case Var(s) => {
      if (env.isEmpty)
        throw new UndefinedSemantics(s"message ${expr}")
      val p = env(Var(s)) match {
        case (x: LocVal) => mem.m(x)
        case _ => throw new UndefinedSemantics(s"message ${expr}")
      }
      Result(p, mem)
		}
    case Add(l, r) => {
      val E1 = eval(env, mem, l)
      val E2 = eval(env, E1.m, r)
      val E = (E1, E2) match {
        case (Result(x: IntVal, xm: Mem), Result(y: IntVal, ym: Mem)) => IntVal(x.n + y.n)
        case _ => throw new UndefinedSemantics(s"message ${expr}")
      }
      Result(E, E2.m)
    }
    case Sub(l, r) => {
      val E1 = eval(env, mem, l)
      val E2 = eval(env, E1.m, r)
      val E = (E1, E2) match {
        case (Result(x: IntVal, xm: Mem), Result(y: IntVal, ym: Mem)) => IntVal(x.n - y.n)
        case _ => throw new UndefinedSemantics(s"message ${expr}")
      }
      Result(E, E2.m)
    }
    case Mul(l, r) => {
      val E1 = eval(env, mem, l)
      val E2 = eval(env, E1.m, r)
      val E = (E1, E2) match {
        case (Result(x: IntVal, xm: Mem), Result(y: IntVal, ym: Mem)) => IntVal(x.n * y.n)
        case _ => throw new UndefinedSemantics(s"message ${expr}")
      }
      Result(E, E2.m)
    }
    case Div(l, r) => {
      val E1 = eval(env, mem, l)
      val E2 = eval(env, E1.m, r)
      val E = (E1, E2) match {
        case (Result(x: IntVal, xm: Mem), Result(y: IntVal, ym: Mem)) => {
          if (y.n == 0)
            throw new UndefinedSemantics(s"message ${expr}")
          IntVal(x.n / y.n)
        }
        case _ => throw new UndefinedSemantics(s"message ${expr}")
      }
      Result(E, E2.m)
    }
    case LTEExpr(l, r) => {
      val E1 = eval(env, mem, l)
      val E2 = eval(env, E1.m, r)
      val E = (E1, E2) match {
        case (Result(n1: IntVal, n1m: Mem), Result(n2: IntVal, n2m: Mem)) => BoolVal(n1.n <= n2.n)
        case _ => throw new UndefinedSemantics(s"message ${expr}")
      }
      Result(E, E2.m)
    }
    case EQExpr(l, r) => {
      val E1 = eval(env, mem, l)
      val E2 = eval(env, E1.m, r)
      val E = (E1, E2) match {
        case (Result(n1: IntVal, n1m: Mem), Result(n2: IntVal, n2m: Mem)) => BoolVal(n1.n == n2.n)
        case (Result(b1: BoolVal, b1m: Mem), Result(b2: BoolVal, b2m: Mem)) => BoolVal(b1.b == b2.b)
        // case (Result(skip1: SkipVal, skip1m: Mem), Result(skip2: SkipVal, skip2m: Mem)) => BoolVal(skip1 == skip2)
        case _ => throw new UndefinedSemantics(s"message ${expr}")
      }
      Result(E, E2.m)
    }
    case Iszero(c) => {
      val v = eval(env, mem, c) match {
        case Result(x: IntVal, xm: Mem) => Result(BoolVal(x.n == 0), xm)
        case _ => throw new UndefinedSemantics(s"message ${expr}")
      }
      v
    }
    case Ite(c, t, f) => {
      val E1 = eval(env, mem, c)
      val E = E1 match {
        case Result(x: BoolVal, xm: Mem) => if (x.b) eval(env, E1.m, t) else eval(env, E1.m, f)
        case _ => throw new UndefinedSemantics(s"message ${expr}")
      }
      E
    }
    case Let(i, v, body) => {
      val E1 = eval(env, mem, v)
      val (new_mem, loc) = E1.m.extended(E1.v)
      val new_env = env + (i -> loc)
      val E2 = eval(new_env, new_mem, body)
      E2
    }
    case Proc(args, expr) => Result(ProcVal(args, expr, env), mem)
    case Asn(v, e) => {
      val E = eval(env, mem, e)
      env.get(v) match {
        case None => throw new UndefinedSemantics(s"message ${expr}")
        case Some(value) => E.m.updated(value, E.v) match {
          case None => throw new UndefinedSemantics(s"message ${expr}")
          case Some(value) => Result(E.v, value)
        }
      }
    }
    case BeginEnd(expr) => eval(env, mem, expr)
    case FieldAccess(record, field) => {
      val E = eval(env, mem, record)
      val E_prime = E.v match {
        case RecordVal(x, loc, next) => {
          if (x == field) E.m.get(loc) match {
            case None => throw new UndefinedSemantics(s"message ${expr}")
            case Some(value) => Result(value, E.m)
          }
          else E.m.get(findVar(next, field)) match {
            case None => throw new UndefinedSemantics(s"message ${expr}")
            case Some(value) => Result(value, E.m)
          }
        }
        case _ => throw new UndefinedSemantics(s"message ${expr}")
      }
      E_prime
    }
    case FieldAssign(record, field, new_val) => {
      val E1 = eval(env, mem, record)
      val E2 = eval(env, E1.m, new_val)
      val loc = E1.v match {
        case RecordVal(x, loc, next) => {
          if (x == field) loc
          else findVar(next, field)
        }
        case _ => throw new UndefinedSemantics(s"message ${expr}")
      }
      val new_mem = E2.m.updated(loc, E2.v) match {
        case None => throw new UndefinedSemantics(s"message ${expr}")
        case Some(value) => value
      }
      Result(E2.v, new_mem)
    }
    case Block(f, s) => {
      val E1 = eval(env, mem, f)
      eval(env, E1.m, s)
    }
    case PCallV(ftn, arg) => {
      val E0 = eval(env, mem, ftn)
      val E = E0.v match {
        case ProcVal(args, expr_0, env_prime) => {
          val (cal_env, cal_mem) = UpdatePCALLV(arg, args, env, env_prime, mem)
          eval(cal_env, cal_mem, expr_0)
        }
        case _ => throw new UndefinedSemantics(s"message ${expr}")
      }
      E
    }
    case PCallR(ftn, arg) => {
      // how to process a list of arg??
      val E0 = eval(env, mem, ftn)
      val E = E0.v match {
        case ProcVal(args, expr_0, env_prime) => {
          val cal_env = UpdateEnvPCALLR(arg, args, env, env_prime)
          eval(cal_env, E0.m, expr_0)
        }
        case _ => throw new UndefinedSemantics(s"message ${expr}")
      }
      E
    }
    case WhileExpr(cond, body) => {
      val E1 = eval(env, mem, cond)
      val E = E1.v match {
        case BoolVal(b) => if (b == true) {
          val E2 = eval(env, E1.m, body)
          Result(SkipVal, eval(env, E2.m, WhileExpr(cond, body)).m)
        } else Result(SkipVal, E1.m) // b == false
        case _ => throw new UndefinedSemantics(s"message ${expr}")
      }
      E
    }
    case EmptyRecordExpr => Result(EmptyRecordVal, mem)
    case RecordExpr(field, initVal, next) => {
      eval(env, mem, initVal) match {
        case Result(v, m) => {
          val (new_mem, new_loc) = m.extended(v)
          next match {
            case EmptyRecordExpr => Result(RecordVal(field, new_loc, EmptyRecordVal), new_mem)
            case recordExpr: RecordExpr => eval(env, new_mem, recordExpr) match {
              case Result(next_v: RecordVal, next_m) => Result(RecordVal(field, new_loc, next_v), next_m)
              case _ => throw new UndefinedSemantics(s"message ${expr}")
            }
          }
        }
      }
    }
    case _ => throw new UndefinedSemantics(s"message ${expr}")
  }

  def Reach(env: Env, mem: Mem, reach: List[LocVal]): List[LocVal] = reach match {
    case head :: next => {
      mem.get(head) match {
        case None => throw new UndefinedSemantics(s"message ${head}")
        case Some(value) => value match {
          case LocVal(l) => {
            if (reach.exists(_ == LocVal(l))) List(head) ::: Reach(env, mem, next)
            else {
              val new_reach = List(LocVal(l)) ::: next
              List(head) ::: Reach(env, mem, new_reach)
            }
          }
          case rv: RecordVal => {
            val new_reach = ReachRec(rv, env, mem) ::: next
            List(head) ::: Reach(env, mem, new_reach)
          }
          case ProcVal(args, expr, env_prime) => List(head) ::: Reach(env_prime, mem, reach) ::: Reach(env, mem, next)
          case _ => List(head) ::: Reach(env, mem, next)
        }
      }
    }
    case Nil => Nil
  }

  def ReachRec(rv: RecordValLike, env: Env, mem: Mem): List[LocVal] = rv match {
    case EmptyRecordVal => Nil
    case RecordVal(field, loc, next) => {
      List(loc) ::: ReachRec(next, env, mem)
    }
  }

  def ReachMem(l: List[LocVal], mem: Mem, mem_locs: List[LocVal]): Mem = mem_locs match {
    case head :: next => {
      if (l.exists(_ == head)) ReachMem(l, mem, next)
      else ReachMem(l, Mem(mem.m.-(head), mem.top), next)
    }
    case Nil => mem
  }

  def gc(env: Env, mem: Mem): Mem = {
    val init_reach = env.values.toList
    val reach = Reach(env, mem, init_reach)
    ReachMem(reach, mem, mem.getLocs())
  }
  
  def apply(program: String): (Val, Mem) = {
    val parsed = MiniCParserDriver(program)
    val res = eval(new Env(), Mem(new HashMap[LocVal,Val],0), parsed)
    (res.v, res.m)
  }

}


object Hw4App extends App {
  
  println("Hello from Hw4!")

}