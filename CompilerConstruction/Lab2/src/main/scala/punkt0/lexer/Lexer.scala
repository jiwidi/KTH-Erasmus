package punkt0
package lexer

import java.io.File


object Lexer extends Phase[File, Iterator[Token]] {
  import Reporter._

  def run(f: File)(ctx: Context): Iterator[Token] = {
    val source = scala.io.Source.fromFile(f)

    new Iterator[Token] {
      var pos = "(" + Positioned.lineOf(source.pos) +":" + Positioned.columnOf(source.pos) + ")"

      var token = new Token(BAD)

      var current = source.next
      
      var keywords = List("object" ,"class" ,"def" ,"override" ,"var" ,"Unit" ,"String" ,"extends" ,"Int" ,"Boolean" ,"while" ,"if" ,"else" ,"true" ,"false" ,"this" ,"null" ,"new" ,"println")

      var eofleft = true

      def hasNext = {
        eofleft
      }

      def next = {
        // Everything else
        var division = false
        var done = false
        while (!done) {
          done = true
          while (current==' '||current=='\t'||current=='\n') {
            done = false
            nextChar
          }
          if (current=='/') {
            pos = "(" + Positioned.lineOf(source.pos) +":" + Positioned.columnOf(source.pos) + ")"
            nextChar
            if (current=='/') {
              done = false
              var comment = true
              while (comment) {
                if (current == '\n' || current == '\u0000') {
                  comment = false
                  nextChar
                } else {
                  nextChar
                }
              }
            } else if (current=='*') {
              done = false
              var comment = true
              while (comment && current != '\u0000') {
                if (current == '*') {
                  nextChar
                  if (current == '/') {
                    comment = false
                    nextChar
                  }
                } else {
                  nextChar
                }
              }
            } else {
              division = true
            }
          }
        }
        if (!division) {
          pos = "(" + Positioned.lineOf(source.pos) +":" + Positioned.columnOf(source.pos) + ")"
        }
        if (current == '\u0000') {
          eofleft = false
          token = new Token(EOF)
        } else if (division) {
          token = new Token(DIV)
        } else if (current==':') {
          nextChar
          token = new Token(COLON)
        } else if (current==';') {
          nextChar
          token = new Token(SEMICOLON)
        } else if (current=='.') {
          nextChar
          token = new Token(DOT)
        } else if (current==',') {
          nextChar
          token = new Token(COMMA)
        } else if (current=='=') {
          nextChar
          if (current=='=') {
            nextChar
            token = new Token(EQUALS)
          } else {
            token = new Token(EQSIGN)
          }
        } else if (current=='!') {
          nextChar
          token = new Token(BANG)
        } else if (current=='(') {
          nextChar
          token = new Token(LPAREN)
        } else if (current==')') {
          nextChar
          token = new Token(RPAREN)
        } else if (current=='{') {
          nextChar
          token = new Token(LBRACE)
        } else if (current=='}') {
          nextChar
          token = new Token(RBRACE)
        } else if (current=='&') {
          nextChar
          if (current=='&') {
            token = new Token(AND)
          } else {
            token = new Token(BAD)
          }
        } else if (current=='|') {
          nextChar
          if (current=='|') {
            token = new Token(OR)
          } else {
            token = new Token(BAD)
          }
        } else if (current=='<') {
          nextChar
          token = new Token(LESSTHAN)
        } else if (current=='+') {
          nextChar
          token = new Token(PLUS)
        } else if (current=='-') {
          nextChar
          token = new Token(MINUS)
        } else if (current=='*') {
          nextChar
          token = new Token(TIMES)
        } else if (current=='"') {
          var currstr = new StringBuffer
          nextChar
          while (current!='"' && current!='\u0000') {
            currstr.append(current)
            nextChar
          }
          if (current=='"') {
            nextChar
            token = new STRLIT(currstr.toString)
          } else {
            token = new Token(BAD)
          }
        } else if (isLetter) {
          var currstr = new StringBuffer
          while (isLetter || isDigit || current=='_') {
            currstr.append(current)
            nextChar
          }
          if (keywords.contains(currstr.toString)) {
            currstr.toString match {
              case "object" =>
                nextChar
                token = new Token(OBJECT)
              case "class" =>
                nextChar
                token = new Token(CLASS)
              case "def" =>
                nextChar
                token = new Token(DEF)
              case "override" =>
                nextChar
                token = new Token(OVERRIDE)
              case "var" =>
                nextChar
                token = new Token(VAR)
              case "Unit" =>
                nextChar
                token = new Token(UNIT)
              case "String" =>
                nextChar
                token = new Token(STRING)
              case "extends" =>
                nextChar
                token = new Token(EXTENDS)
              case "Int" =>
                nextChar
                token = new Token(INT)
              case "Boolean" =>
                nextChar
                token = new Token(BOOLEAN)
              case "while" =>
                nextChar
                token = new Token(WHILE)
              case "if" =>
                nextChar
                token = new Token(IF)
              case "else" =>
                nextChar
                token = new Token(ELSE)
              case "true" =>
                nextChar
                token = new Token(TRUE)
              case "false" =>
                nextChar
                token = new Token(FALSE)
              case "this" =>
                nextChar
                token = new Token(THIS)
              case "null" =>
                nextChar
                token = new Token(NULL)
              case "new" =>
                nextChar
                token = new Token(NEW)
              case "println" =>
                nextChar
                token = new Token(PRINTLN)
            }
          } else {
            token = new ID(currstr.toString)
          }
        } else if (isDigit) {
          var value = 0
          while (isDigit) {
            value = value * 10 + (current - 48)
            nextChar
          }
          if (isLetter) {
            while (isDigit || isLetter) {
              nextChar
            }
            token = new Token(BAD)
          } else {
            token = new INTLIT(value)
          }
        } else {
          nextChar
          token = new Token(BAD)
        }
        if (ctx.doTokens) {
          println(token + pos)
        }
        token
      }

      def isDigit = {
        (current >= '0' && current <= '9')
      }

      def isLetter = {
        (current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z')
      }

      def nextChar: Unit = {
        if (source.hasNext) {
          current = source.next
        } else {
          current = '\u0000'
        }
      }
    }
  }
}
