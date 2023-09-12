import java.util.Arrays;

class Node {
    int data;
    Node next;

    Node(int data) {
        this.data = data;
        this.next = null;
    }

}


class fcfs{
    public static void main(String[] args) {
        int time = 0;
        int n = 10; 
        int[] rp = new int[n]; 
        int g; 
        int[][] shareMatrix = new int[n][n]; 
        int[] capacity = new int[n]; 
        Node head = null;

        for (int i = 0; i < n; i++) {
            rp[i] = i;
        }

        while (time <= 100000) {
            g = Arrays.stream(rp).min().getAsInt();

            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    shareMatrix[i][j] = (rp[i] - g) * (rp[j] - g);
                }
            }

            for (int i = 0; i < n; i++) {
                capacity[i] = Math.abs(g);
            }

            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    Node newNode = new Node(shareMatrix[i][j]);
                    newNode.next = head;
                    head = newNode;
                }
            }
            head = sortLinkedList(head);
            
            time++;
        }
        Node current = head;
        while (current != null) {
            System.out.print(current.data + " ");
            current = current.next;
        }
    }   
        
    public static Node sortLinkedList(Node head) {
        Node dummy = new Node(0);
        dummy.next = head;
        Node current = head;
        int n = 0;
        while (current != null) {
            current = current.next;
            n++;
        }

        for (int i = 1; i < n; i++) {
            current = dummy;
            for (int j = 1; j < n; j++) {
                if (current.next.data > current.next.next.data) {
                    Node next1 = current.next;
                    Node next2 = current.next.next;
                    next1.next = next2.next;
                    current.next = next2;
                    next2.next = next1;
                }
                current = current.next;
            }
        }
        return dummy.next;
    }
}

