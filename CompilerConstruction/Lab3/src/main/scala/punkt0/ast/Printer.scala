package punkt0
package ast

import Trees._

object Printer {
  def apply(t: Tree): String = {
    var progstr = new StringBuffer
    t match {
      case p: Program => {
        //_.classes.foreach {
        //  progstr.append(_.apply)
        //}
        //progstr.append(_.main.apply)
        for (c <- p.classes) {
          progstr.append(c.toString + "\n")
        }
        progstr.append(apply(p.main))
      }
      case m: MainDecl => {
        progstr.append("object " + m.obj + " extends " + m.parent + " {\n")
        progstr.append(indent("vars: " + m.vars.toString) + "\n")
        progstr.append(indent("methods: " + m.exprs.toString) + "\n")
        // vars: List(VarDecl)
        // exprs: List(ExprTree)
        progstr.append("}\n")
      }
      case t => println("No match for: " + t.toString)
    }
    progstr.toString
  }
  def indent(s: String) = {
    "  " + s.replace("\n", "  \n")
  }
}
