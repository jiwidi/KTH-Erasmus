/* Labb 2 i DD1352 Algoritmer, datastrukturer och komplexitet    */
/* Se labbanvisning under kurssidan http://www.csc.kth.se/DD1352 */
/* Ursprunglig författare: Viggo Kann KTH viggo@nada.kth.se      */
import java.util.LinkedList;
import java.util.List;
import java.util.HashMap;
import java.util.Map;

public class ClosestWords {
  LinkedList<String> closestWords = null;

  int MAX=100000;
    int closestDistance =MAX;



    int partDist(String w1, String w2, int w1len, int w2len,int[][] M) {

      if (w1len == 0)
        return w2len;
      else if (w2len == 0)
        return w1len;
      if (M[w1len][w2len]!=0){
          return M[w1len][w2len];
      }
      int res = M[w1len-1][w2len-1] +
  	(w1.charAt(w1len - 1) == w2.charAt(w2len - 1) ? 0 : 1);
      int addLetter = M[w1len-1][w2len] + 1;
      if (addLetter < res)
        res = addLetter;
      int deleteLetter = M[w1len][w2len-1] + 1;
      if (deleteLetter < res)
        res = deleteLetter;
      return res;
   }

   int fill(String w1, String w2, int w1len, int w2len,int[][] M){
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
          //System.out.println();
      }
      //for (int i = 0; i < M.length; i++) {
      //    for (int j = 0; j < M[i].length; j++) {
      //        System.out.print(M[i][j] + " ");
      //    }
      //    System.out.println();
      //}
      return M[w1len][w2len];
   }

    int Distance(String w1, String w2) {
      int w1len=w1.length();
      int w2len=w2.length();
      int[][] M = new int[w1len+1][w2len+1];
      int ci=0;
      int cj=0;
      for (int i = 0; i < M.length; i++) {
          for (int j = 0; j < M[i].length; j++) {
              M[i][j]=partDist(w1,w2,ci,cj,M);
              cj++;
      }
          ci++;
          cj=0;
      //System.out.println();
      }
      return M[w1len][w2len];


  }

  public ClosestWords(String w, List<String> wordList) {
    for (String s : wordList) {
      int dist;
      //System.out.println("d(" + w + "," + s + ")=" + dist);
      if ( closestDistance!=MAX && s.length() - w.length() > closestDistance){
        dist=MAX;
        break;
      }
      else{
        dist = Distance(w, s);
        if (dist < closestDistance || closestDistance == -1) {
          closestDistance = dist;
          closestWords = new LinkedList<String>();
          closestWords.add(s);
        }
        else if (dist == closestDistance)
          closestWords.add(s);
      }
    }
  }

  int getMinDistance() {
    return closestDistance;
  }

  List<String> getClosestWords() {
    return closestWords;
  }
}
