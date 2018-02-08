/* Labb 2 i DD1352 Algoritmer, datastrukturer och komplexitet    */
/* Se labbanvisning under kurssidan http://www.csc.kth.se/DD1352 */
/* Ursprunglig författare: Viggo Kann KTH viggo@nada.kth.se      */
import java.util.LinkedList;
import java.util.List;
import java.util.HashMap;
import java.util.Map;

public class ClosestWords {
  LinkedList<String> closestWords = null;
  int closestDistance = -1;
    static int maxDist = 9999;
    static int matrixSize = 50;
    static int M[][] = new int[matrixSize][matrixSize];

    // initiate M with base cases
    static {
	for (int i = 0; i < matrixSize; i++){
	    M[i][0] = i; 
	    M[0][i] = i;
	}
    }
    int partDist(String w1, String w2, int w1len, int w2len,int[][] M) {
      if (M[w1len][w2len]!=0){
          return M[w1len][w2len];
      }
      if (w1len == 0)
        return w2len;
      if (w2len == 0)
        return w1len;

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

    // int Distance(String w1, String w2) {

    // 	int w1len=w1.length();
    // 	int w2len=w2.length();
    // 	int[][] M = new int[w1len+1][w2len+1];
    // 	int ci=0;
    // 	int cj=0;
    // 	for (int i = 0; i < M.length; i++) {
    // 	    for (int j = 0; j < M[i].length; j++) {
    // 		M[i][j]=partDist(w1,w2,ci,cj,M);
    // 		cj++;
    // 	    }
    // 	    ci++;
    // 	    cj=0;
    // 	    //System.out.println();
    // 	}
    // 	// for (int i = 0; i < M.length; i++) {
    // 	//     for (int j = 0; j < M[i].length; j++) {
    // 	//         System.out.print(M[i][j] + " ");
    // 	//     }
    // 	// 	  System.out.println();
    // 	// }
    // 	return M[w1len][w2len];


    // }

    int Distance(String word1, String dictword, String prevdictword){
	int w1len = word1.length();
	int dwlen = dictword.length();
	int pwlen = prevdictword.length();

	int col_index = 1;	
	int minlength = Math.min(dwlen,pwlen);
	// start at the column col_index where the characters in the word are unequal
	for (int i = 1; i < minlength &&
		 dictword.charAt(i) == prevdictword.charAt(i); i++){
	    // if (dictword.charAt(i) == prevdictword.charAt(i))
		col_index++;
	}
	// Skip words who cannot have smaller distance than the previous min distance
	if (dwlen - w1len > getMinDistance() && getMinDistance() != -1){
	    return maxDist;
	}
	// Fill the matrix
	for (int i = 1; i <= w1len; i++){
	    for(int j = col_index; j <= dwlen; j++){
		if (word1.charAt(i - 1) == dictword.charAt(j - 1)){
		    M[i][j] = M[i - 1][j - 1];
		}
		else {
		    M[i][j] = 1 + Math.min(Math.min(M[i - 1][j - 1],
						    M[i][j - 1]), M[i - 1][j]);
		}
	    }

	}
	return M[w1len][dwlen];
    }
	
  public ClosestWords(String w, List<String> wordList) {
      String prevword = "";
    for (String s : wordList) {
        int dist = Distance(w, s, prevword);
	 
      // System.out.println("d(" + w + "," + s + ")=" + dist);
      if (dist < closestDistance || closestDistance == -1) {
        closestDistance = dist;
        closestWords = new LinkedList<String>();
        closestWords.add(s);
      }
      else if (dist == closestDistance)
        closestWords.add(s);
      // skip words that we know are not good candidates
      if (dist != maxDist) 
	  prevword = s;

    }
  }

  int getMinDistance() {
    return closestDistance;
  }

  List<String> getClosestWords() {
    return closestWords;
  }
}
