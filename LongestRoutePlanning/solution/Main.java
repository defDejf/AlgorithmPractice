import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.Scanner;
public class Main {
    public static void main(String[] args) {
//        try {
//            System.setIn(new FileInputStream(new File("pub09.in")));
//        } catch (FileNotFoundException e) {
//            throw new RuntimeException(e);
//        }
        Scanner scanner = new Scanner(System.in);
        int num_V = scanner.nextInt();
        int num_E = scanner.nextInt();
        Graph g = new Graph(num_V);
        for (int i = 0; i < num_V; i++){
            g.vertexes.add(new Vertex(i));
        }
        for (int i = 0; i < num_E; i++) {
            int from, to, weight;
            from = scanner.nextInt();
            to = scanner.nextInt();
            weight = scanner.nextInt();
            g.add_edge(from - 1, to - 1, weight);
            //System.out.println(i);
        }

//        for (alg.Vertex v : g.vertexes){
//            for(alg.GraphEdge e : v.adjacent){
//                if (e.to - e.from > 1) {
//                    for (int i = e.from+1; i < e.to; i++){
//                        g.vertexes.get(i).isCottage = false;
//                    }
//                }
//            }
//        }
        g.markCottages();

        int num_cottages = 0;
        for (Vertex v : g.vertexes){
            if (v.isCottage){
                num_cottages++;
            }
        }

        g.scores.get(0).even.score = 0;
        g.scores.get(0).even.steps = 0;
        g.scores.get(0).odd.score = 0;
        g.scores.get(0).odd.steps = -1;

        System.out.println(g.getLongest() + " " + num_cottages );

//        int l = 1;
//        for (alg.PairPair pp : g.scores){
//            System.out.println("ID " + l + " EVEN SCORE " + pp.even.score + " EVEN STEP " + pp.even.steps + " ODD SCORE " + pp.odd.score + " ODD STEP " + pp.odd.steps);
//            l++;
//        }

//        for (alg.Vertex v : g.vertexes){
//            System.out.println("vert " + (v.id+1)+ " " +v.isCottage);
//        }
    }
}