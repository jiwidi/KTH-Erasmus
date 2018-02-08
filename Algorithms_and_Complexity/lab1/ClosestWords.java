/* Labb 2 i DD1352 Algoritmer, datastrukturer och komplexitet    */
/* Se labbanvisning under kurssidan http://www.csc.kth.se/DD1352 */
/* Ursprunglig författare: Viggo Kann KTH viggo@nada.kth.se      */
import java.util.LinkedList;
import java.util.List;

public class ClosestWords {
  LinkedList<String> closestWords = null;

  int closestDistance = -1;

   public static int partDist(String w1, String w2, int w1len, int w2len,int[][] M) {
       
    if (w1len == 0)
      return w2len;
    if (w2len == 0)
      return w1len;
    if (M[w1len][w2len]!=-1){
        return M[w1len][w2len];
    }
    int res = partDist(w1, w2, w1len - 1, w2len - 1,M) + 
	(w1.charAt(w1len - 1) == w2.charAt(w2len - 1) ? 0 : 1);
    int addLetter = partDist(w1, w2, w1len - 1, w2len,M) + 1;
    if (addLetter < res)
      res = addLetter;
    int deleteLetter = partDist(w1, w2, w1len, w2len - 1,M) + 1;
    if (deleteLetter < res)
      res = deleteLetter;
    return res;
   }
  
   public static int fill(String w1, String w2, int w1len, int w2len,int[][] M){
      int ci,cj;
      ci=0;
      cj=0;
      for (int i = 0; i < M.length; i++) {
          for (int j = 0; j < M[i].length; j++) {
              M[i][j]=partDist(w1,w2,ci,cj,M);
              cj++;
          }
          ci++;
          cj=0;
          System.out.println();
      } 
      for (int i = 0; i < M.length; i++) {
          for (int j = 0; j < M[i].length; j++) {
              System.out.print(M[i][j] + " ");
          }
          System.out.println();
      }
      return M[w1len][w2len];
   }

  public static int Distance(String w1, String w2) {   
      int[][] M = new int[w1.length()+1][w2.length()+1];
      for (int i = 0; i < M.length; i++) {
          for (int j = 0; j < M[i].length; j++) {
              M[i][j]=-1;
          }
          System.out.println();
      }
      return fill(w1, w2, w1.length(), w2.length(),M);
      

  }

  public ClosestWords(String w, List<String> wordList) {
    for (String s : wordList) {
      int dist = Distance(w, s);
      // System.out.println("d(" + w + "," + s + ")=" + dist);
      if (dist < closestDistance || closestDistance == -1) {
        closestDistance = dist;
        closestWords = new LinkedList<String>();
        closestWords.add(s);
      }
      else if (dist == closestDistance)
        closestWords.add(s);
    }
  }

  int getMinDistance() {
    return closestDistance;
  }

  List<String> getClosestWords() {
    return closestWords;
  }
}
