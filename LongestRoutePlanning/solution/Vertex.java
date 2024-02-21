import java.util.ArrayList;

public class Vertex {
    public int id;
    public ArrayList<GraphEdge> adjacent;

    public boolean isCottage;

    public Vertex(int id) {
        this.id = id;
        this.adjacent = new ArrayList<>();
        this.isCottage = true;
    }

    public void add_adj(int to, int weight){
        adjacent.add(new GraphEdge(this.id, to, weight));
    }
}
