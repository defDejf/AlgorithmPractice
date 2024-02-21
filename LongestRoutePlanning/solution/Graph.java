import java.util.ArrayList;
import java.util.Collections;
import java.util.Stack;

public class Graph {
    public int numVertexes;
    public ArrayList<Vertex> vertexes;
    public Stack<Vertex> topSort;

    public ArrayList<Boolean> visited;

    public ArrayList<PairPair> scores;

    public Graph(int numVertexes) {
        this.numVertexes = numVertexes;
        this.vertexes = new ArrayList<>();
        this.topSort = new Stack<>();
        this.visited = new ArrayList<>();
        this.scores = new ArrayList<>();
        for (int i = 0; i < numVertexes; i++){
            visited.add(Boolean.FALSE);
            scores.add(new PairPair());
        }
    }

    public void add_edge(int from, int to, int weight){
            //System.out.println("From " + (from+1)+ " To " +(to+1)+" Weight "+weight);
            vertexes.get(from).add_adj(to, weight);
    }

    public void topSortUtil(Vertex current){
        visited.set(current.id, Boolean.TRUE);
        for(GraphEdge g : current.adjacent){
            if (visited.get(g.to) == Boolean.FALSE){
                topSortUtil(vertexes.get(g.to));
            }
        }
        topSort.push(current);
    }

    public void markCottages(){
        topSortUtil(vertexes.get(0));
        ArrayList<Vertex> topSortList = new ArrayList<>(topSort);
        Collections.reverse(topSortList);
        for (Vertex v : topSortList){
            int cur_idx = topSortList.indexOf(v);
            for (GraphEdge e : v.adjacent){
                int dest_idx = topSortList.indexOf(vertexes.get(e.to));
                //System.out.println(cur_idx + " " + dest_idx);
                if (dest_idx - cur_idx > 1){
                    for (int i = cur_idx+1; i < dest_idx; i++) {
                        vertexes.get(topSortList.get(i).id).isCottage = false;
                    }
                }
            }
        }
    }

    int getLongest(){
        for (int i = 0; i < numVertexes; i++){
            visited.set(i, Boolean.FALSE);
        }
        while (!topSort.empty()){
            Vertex v = topSort.pop();
            for (GraphEdge e : v.adjacent){
                if (vertexes.get(e.to).isCottage){ // dest is cottage
                    if (scores.get(v.id).odd.score + e.weight > scores.get(e.to).even.score){
                        scores.get(e.to).even.score = scores.get(v.id).odd.score + e.weight;
                        scores.get(e.to).even.steps = scores.get(v.id).odd.steps +1;
                    }
                } else {
                    if (!v.isCottage) { // ADDED COND WHEN LEAVING COTTAGE SET ONLY ODD IN NEXT
                        if (scores.get(v.id).odd.score + e.weight > scores.get(e.to).even.score) {
                            scores.get(e.to).even.score = scores.get(v.id).odd.score + e.weight;
                            scores.get(e.to).even.steps = scores.get(v.id).odd.steps + 1;
                        }
                    }
                    if (scores.get(v.id).even.score + e.weight > scores.get(e.to).odd.score) {
                        scores.get(e.to).odd.score = scores.get(v.id).even.score + e.weight;
                        scores.get(e.to).odd.steps = scores.get(v.id).even.steps + 1;
                    }
                }
            }
        }
        return scores.get(numVertexes-1).even.score;
    }

}
