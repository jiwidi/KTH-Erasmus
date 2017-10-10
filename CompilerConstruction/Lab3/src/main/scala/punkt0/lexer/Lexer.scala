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
      
      var keywords = Map(("object", new Token(OBJECT)), ("class", new Token(CLASS)), ("def", new Token(DEF)), ("override", new Token(OVERRIDE)), ("var", new Token(VAR)), ("Unit", new Token(UNIT)), ("String", new Token(STRING)), ("extends", new Token(EXTENDS)), ("Int", new Token(INT)), ("Boolean", new Token(BOOLEAN)), ("while", new Token(WHILE)), ("if", new Token(IF)), ("else", new Token(ELSE)), ("true", new Token(TRUE)), ("false", new Token(FALSE)), ("this", new Token(THIS)), ("null", new Token(NULL)), ("new", new Token(NEW)), ("println", new Token(PRINTLN)))

      var simpletokens = Map((':', new Token(COLON)), (';', new Token(SEMICOLON)), ('.', new Token(DOT)), (',', new Token(COMMA)), ('!', new Token(BANG)), ('(', new Token(LPAREN)), (')', new Token(RPAREN)), ('{', new Token(LBRACE)), ('}', new Token(RBRACE)), ('<', new Token(LESSTHAN)), ('+', new Token(PLUS)), ('-', new Token(MINUS)), ('*', new Token(TIMES)))

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
        } else if (simpletokens contains current) {
          token = simpletokens(current)
          nextChar
        } else if (current=='=') {
          nextChar
          if (current=='=') {
            token = new Token(EQUALS)
            nextChar
          } else {
            token = new Token(EQSIGN)
          }
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
          if (keywords contains currstr.toString) {
            token = keywords(currstr.toString)
            nextChar
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
