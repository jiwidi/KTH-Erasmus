package punkt0
package ast

import Trees._
import lexer._
import scala.collection.mutable.ListBuffer

object Parser extends Phase[Iterator[Token], Program] {
  def run(tokens: Iterator[Token])(ctx: Context): Program = {
    import Reporter._
    /** Store the current token, as read from the lexer. */
    var currentToken: Token = new Token(BAD)

    def readToken: Unit = {
      if (tokens.hasNext) {
        // uses nextToken from the Lexer trait
        currentToken = tokens.next

        // skips bad tokens
        while (currentToken.kind == BAD) {
          currentToken = tokens.next
        }
      }
    }

    def expect(kind : TokenKind): Token = {
      if (currentToken.kind == kind) {
        var token = currentToken
        readToken
        token
      } else {
        error("Expected " + kind)
        currentToken
      }
    }

    /** ''Eats'' the expected token, or terminates with an error. */
    def eat(kind: TokenKind): Unit = {
      if (currentToken.kind == kind) {
        readToken
      } else {
        expected(kind)
      }
    }

    /** Complains that what was found was not expected. The method accepts arbitrarily many arguments of type TokenKind */
    def expected(kind: TokenKind, more: TokenKind*): Nothing = {
      fatal("expected: " + (kind::more.toList).mkString(" or ") + ", found: " + currentToken, currentToken)
    }

    def parseGoal: Program = {
      var classes = new ListBuffer[ClassDecl]
      while (currentToken.kind == CLASS) {
        classes += parseClass
      }
      var main = parseMain
      Program(main, classes.toList)
    }

    def parseMain: MainDecl = {
      eat(OBJECT)
      val identifier = expect(IDKIND).toString
      eat(EXTENDS)
      val parent = expect(IDKIND).toString
      eat(LBRACE)
      var vars = new ListBuffer[VarDecl]
      var exprs = new ListBuffer[ExprTree]
      // Temp skip, removes everything until matching brace
      var depth = 0
      while (currentToken.kind != RBRACE || depth > 0) {
        if (currentToken.kind == LBRACE) {
          depth += 1
        } else if (currentToken.kind == RBRACE) {
          depth -= 1
        }
        readToken
      }
      // Parse main method
      eat(RBRACE)
      MainDecl(Identifier(identifier), Identifier(parent), vars.toList, exprs.toList)
    }

    def parseClass: ClassDecl = {
      eat(CLASS)
      val identifier = expect(IDKIND).toString
      var parent: Option[Identifier] = None
      if (currentToken.kind == EXTENDS) {
        eat(EXTENDS)
        parent = Some(Identifier(expect(IDKIND).toString))
      }
      eat(LBRACE)
      var vars = new ListBuffer[VarDecl]
      //while (currentToken.kind == VAR) {
      //  eat(VAR)
      //  var id = expect(IDKIND).toString
      //  eat(COLON)
      //  if 
      //  vars.append(Vardecl(type, id ,expr)
      //}
      var methods = new ListBuffer[MethodDecl]
      // Temp skip, removes everything until matching brace
      var depth = 0
      while (currentToken.kind != RBRACE || depth > 0) {
        if (currentToken.kind == LBRACE) {
          depth += 1
        } else if (currentToken.kind == RBRACE) {
          depth -= 1
        }
        readToken
      }
      // Parse class
      eat(RBRACE)
      ClassDecl(Identifier(identifier), parent, vars.toList, methods.toList)
    }

    readToken
    val tree = parseGoal
    if (ctx.doAST) {
      println(tree)
    }
    terminateIfErrors
    tree
  }
}
