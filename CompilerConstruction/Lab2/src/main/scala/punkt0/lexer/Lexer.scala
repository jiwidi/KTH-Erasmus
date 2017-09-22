package punkt0
package lexer

import java.io.File


object Lexer extends Phase[File, Iterator[Token]] {
  import Reporter._

  def run(f: File)(ctx: Context): Iterator[Token] = {
    val source = scala.io.Source.fromFile(f)

    // TODO: implement this method

    new Iterator[Token] {
      var current = source.next

      var keywords = List("object" ,"class" ,"def" ,"override" ,"var" ,"Unit" ,"String" ,"extends" ,"Int" ,"Boolean" ,"while" ,"if" ,"else" ,"true" ,"false" ,"this" ,"null" ,"new" ,"println")

      def hasNext = {
        source.hasNext
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
        if (current == '\u0000') {
          new Token(EOF)
        } else if (division) {
          new Token(DIV)
        } else if (isLetter) {
          var currstr = new StringBuffer
          while (isLetter || isDigit) {
            currstr.append(current)
            nextChar
          }
          if (keywords.contains(currstr.toString)) {
            currstr.toString match {
              case "object" =>
                nextChar
                new Token(OBJECT)
              case "class" =>
                nextChar
                new Token(CLASS)
              case "def" =>
                nextChar
                new Token(DEF)
              case "override" =>
                nextChar
                new Token(OVERRIDE)
              case "var" =>
                nextChar
                new Token(VAR)
              case "Unit" =>
                nextChar
                new Token(UNIT)
              case "String" =>
                nextChar
                new Token(STRING)
              case "extends" =>
                nextChar
                new Token(EXTENDS)
              case "Int" =>
                nextChar
                new Token(INT)
              case "Boolean" =>
                nextChar
                new Token(BOOLEAN)
              case "while" =>
                nextChar
                new Token(WHILE)
              case "if" =>
                nextChar
                new Token(IF)
              case "else" =>
                nextChar
                new Token(ELSE)
              case "true" =>
                nextChar
                new Token(TRUE)
              case "false" =>
                nextChar
                new Token(FALSE)
              case "this" =>
                nextChar
                new Token(THIS)
              case "null" =>
                nextChar
                new Token(NULL)
              case "new" =>
                nextChar
                new Token(NEW)
              case "println" =>
                nextChar
                new Token(PRINTLN)
            }
          } else {
            nextChar
            new ID(currstr.toString)
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
            new Token(BAD)
          } else {
            new INTLIT(value)
          }
        } else {
          nextChar
          new Token(BAD)
        }
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
