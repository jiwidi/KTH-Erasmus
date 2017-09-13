package recfun

object Main {

  def main(args: Array[String]) {

    println("Pascal's Triangle")
    for (row <- 0 to 10) {
      var spaces=""
      for (a <- 0 to (10-row)){
        spaces+=" ";
      }
      print(spaces);
      for (col <- 0 to row)
        print(pascal(col, row) + " ")
      println()
    }
  }

  /**
   * Exercise 1
   */
  def pascal(c: Int, r: Int): Int =
      if(r==0 && c==0){
        1;
      }
      else if(c<0 || c>r){
        0;
      }
      else{
        pascal(c-1,r-1)+pascal(c,r-1);
      }

  /**
   * Exercise 2
   */
  def balance(chars: List[Char]): Boolean =
    for ( i <- 0 to (chars.length - 1)){
      if(chars(i)==')'){
        false
      }
      else if(chars(i)=='(') {
        for (x2 <- (chars.length - 1) to 0 by -1) {
          if (chars(x2) == ')') {
            chars(x2)=' '
            true
          }
          else if (chars(x2) == '(') {
            false
          }
          else if (x2 == 0) {
            false
          }
        }
      }
    }
    false


  /**
   * Exercise 3
   */
  def countChange(money: Int, coins: List[Int]): Int = ???

}
