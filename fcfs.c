#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct node
{
    int data;
    struct node *next;
} *head=NULL ;

int min(int arr[], int n)
{
    int min = INT_MAX;
    for (int i = 0; i < n; i++)
    {
        if (arr[i] < min)
        {
            min = arr[i];
        }
    }
    return min;
}
struct node *sort_linked_list(struct node *head)
{
    struct node *i, *j;
    int temp;
    for (i = head; i->next != NULL; i = i->next)
    {
        for (j = i->next; j != NULL; j = j->next)
        {
            if (i->data > j->data)
            {
                temp = i->data;
                i->data = j->data;
                j->data = temp;
            }
        }
    }
    return head;
}

int main()
{
    int time = 0;
    int n = 10;             
    int rp[n];              
    int g;                  
    int share_matrix[n][n]; 
    int capacity[n];        
    
    for (int i = 0; i < n; i++)
    {
        rp[i] = i;
    }

    while (time <= 100000)
    {
        g = min(rp, n);

        for (int i=0;i<n;i++)
        {
            for (int j = 0; j < n; j++)
            {
                share_matrix[i][j] = (rp[i] - g) * (rp[j] - g);
            }
        }


        for (int i = 0; i < n; i++)
        {
            capacity[i] = abs(g);
        }

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                struct node *new_node = (struct node *)malloc(sizeof(struct node));
                new_node->data = share_matrix[i][j];
                new_node->next = head;
                head = new_node;
                head = sort_linked_list(head);
            }
        }
        struct node *current = head;
        while (current != NULL)
        {
            printf("%d ", current->data);
            current = current->next;
        }

        time++;
    }
    return 0;
}