#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#define MAX_ROWS 20

int ex_t1[MAX_ROWS], ex_t2[MAX_ROWS], ex_t3[MAX_ROWS], ex_t4[MAX_ROWS];
int temp1[MAX_ROWS], temp2[MAX_ROWS], temp3[MAX_ROWS], temp4[MAX_ROWS];
int share1[MAX_ROWS], share2[MAX_ROWS], share3[MAX_ROWS], share4[MAX_ROWS];
int deadline[MAX_ROWS], rem_deadline[MAX_ROWS];

struct data
{
    int task_id;
    int share_id;
    int pc_id;
    int tss;
    struct data *next;
} *head = NULL, *Shead = NULL;

#define MAX_PC 4

struct Node
{
    int task_id;
    int share_id;
    int tss;
};
struct data *create_node(int task_id, int share_id, int pc_id, int tss)
{
    struct data *new_node = (struct data *)malloc(sizeof(struct data));
    new_node->task_id = task_id;
    new_node->share_id = share_id;
    new_node->pc_id = pc_id;
    new_node->tss = tss;
    new_node->next = NULL;
    return new_node;
}
struct Node *create_allonode(int task_id, int share_id, int tss)
{
    struct Node *new_anode = (struct Node *)malloc(sizeof(struct Node));
    new_anode->task_id = task_id;
    new_anode->share_id = share_id;
    new_anode->tss = tss;
    return new_anode;
}

void sorted_insert(struct data **head_ref, struct data *new_node)
{
    struct data *current;
    if (*head_ref == NULL || (*head_ref)->share_id >= new_node->share_id)
    {
        new_node->next = *head_ref;
        *head_ref = new_node;
    }
    else
    {
        current = *head_ref;
        while (current->next != NULL && current->next->share_id < new_node->share_id)
        {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}
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

int share_func(int et[MAX_ROWS], int sm[MAX_ROWS], int dl[MAX_ROWS], int dlr[MAX_ROWS])
{
    int g = min(dlr, MAX_ROWS);
    for (int i = 0; i < MAX_ROWS; i++)
    {
        sm[i] = (et[i] * g) / dl[i];
    }
    return sm[MAX_ROWS];
}

void Readfile(char *filename)
{
    FILE *fp;
    int row = 0;
    int test;
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Error opening file\n");
    }
    while (row < MAX_ROWS && fscanf(fp, "%d\t%d\t%d\t%d\t%d\t%d", &test, &ex_t1[row], &ex_t2[row], &deadline[row], &temp1[row], &temp2[row]) == 6)
    {
        row++;
    }
    fclose(fp);
}

int main()
{
    int row = 0;
    char *filename = "Set_1_Input_m_2_U_40_n_20.txt"; // argv[1]
    Readfile(filename);

    while (row < MAX_ROWS)
    {
        ex_t3[row] = ex_t1[row] + 10;
        ex_t4[row] = ex_t2[row] + 10;
        temp3[row] = temp1[row] + 10;
        temp4[row] = temp2[row] + 10;
        rem_deadline[row] = deadline[row];
        row++;
    }

    // for (row = 0; row < MAX_ROWS; row++){            //printing file values
    //     printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t",row, ex_t1[row], ex_t2[row], ex_t3[row], ex_t4[row], deadline[row], temp1[row], temp2[row], temp3[row], temp4[row]);
    //     printf("\n");
    // }

    share_func(ex_t1, share1, deadline, rem_deadline);
    share_func(ex_t2, share2, deadline, rem_deadline);
    share_func(ex_t3, share3, deadline, rem_deadline);
    share_func(ex_t4, share4, deadline, rem_deadline);
    for (row = 0; row < MAX_ROWS; row++)
    {
        struct data *new_node;
        new_node = create_node(row, share1[row], 0, temp1[row]);
        sorted_insert(&head, new_node);
        new_node = create_node(row, share2[row], 1, temp2[row]);
        sorted_insert(&head, new_node);
        new_node = create_node(row, share3[row], 2, temp3[row]);
        sorted_insert(&head, new_node);
        new_node = create_node(row, share4[row], 3, temp4[row]);
        sorted_insert(&head, new_node);
        // new_node = create_node(row, ex_t1[row], 1, temp1[row]);            //input normal ex_table
        // sorted_insert(&head, new_node);
        // new_node = create_node(row + 20, ex_t2[row], 2, temp2[row]);
        // sorted_insert(&head, new_node);
        // new_node = create_node(row + 40, ex_t3[row], 3, temp3[row]);
        // sorted_insert(&head, new_node);
        // new_node = create_node(row + 60, ex_t4[row], 4, temp4[row]);
        // sorted_insert(&head, new_node);
    }

    // struct data *ptr = head;
    // while (ptr != NULL)                   // printing structure
    // {
    //     printf("task_id: %d, share_id: %d, pc_id: %d, tss: %d\n", ptr->task_id, ptr->share_id, ptr->pc_id, ptr->tss);
    //     ptr = ptr->next;
    // }

    // scheduling
    int g = min(deadline ,MAX_ROWS);
    //c printf("%d",g);
    int Capacity[MAX_PC], task_allocation_flag[MAX_ROWS] = {0};
    struct Node *Allocation_seq[MAX_PC][MAX_ROWS] = {0};
    int no_task_Core[MAX_PC] = {0};
    for (int i = 0; i < MAX_PC; i++)
    {
        Capacity[i] = g;  //use
    }

    struct data *pt = head;

    while (pt != NULL)
    {
        while (pt != NULL && (task_allocation_flag[pt->task_id] == 1 || pt->share_id > Capacity[pt->pc_id]))
        {
            pt = pt->next;
        }
        if (pt != NULL)
        {
            Allocation_seq[pt->pc_id][no_task_Core[pt->pc_id]++] = create_allonode(pt->task_id, pt->share_id, pt->tss);
            Capacity[pt->pc_id] -= pt->share_id;
            // printf("%d  ", Capacity[pt->pc_id]);
            task_allocation_flag[pt->task_id] = 1;
            // remaining deadline
            // deletion of allocated node
            pt = pt->next;
        }
    }

    // for (int i = 0; i < MAX_PC; i++)        //printing allocation sequence
    // {
    //     for (int j = 0; j < no_task_Core[i]; j++)
    //     {
    //         struct Node *current_node = Allocation_seq[i][j];
    //         printf("%d ", current_node->task_id);
    //     }
    //     printf("\n");
    // }
    
    
    //Migrating Task
    return 0;
}

// BIN SECTION
// 1
//  for (int i = 0; i < 4; i++)
//  {
//      for (int j = 0; j < 20; j++)
//      {
//          printf("%d ",Allocation_seq[i][j]);
//      }
//      printf("\n");
//  }
// 2
//  while (pt->share_id > Capacity[pt->pc_id] && pt != NULL && task_allocation_flag[pt->task_id] == 0)
//  {
//      pt = pt->next;
//  }
//  if (pt != NULL)
//  {
//      Allocation_seq[pt->pc_id][x++] = pt->task_id;
//      Capacity[pt->pc_id] -= pt->share_id;
//  }

// for (int i = 0; i < MAX_PC; i++)
// {
// printf("Core %d tasks: ", i);
// for (int j = 0; j < MAX_ROWS; j++)
// {
//     struct Node *current_node = Allocation_seq[i][j];

//     printf("%d ", current_node->task_id);
// }
// printf("\n");
// }


    /*void migrateTask()
    {
        // Copy linked list
        struct node *temp = head;
        struct node *newList = NULL;
        struct node *newTail = NULL;
        while (temp != NULL)
        {
            struct node *newNode = (struct node *)malloc(sizeof(struct node));
            newNode->task_id = temp->task_id;
            newNode->share_id = temp->share_id;
            newNode->tss_id = temp->tss_id;
            newNode->next = NULL;
            if (newList == NULL)
            {
                newList = newNode;
                newTail = newNode;
            }
            else
            {
                newTail->next = newNode;
                newTail = newNode;
            }
            temp = temp->next;
        }

        // Delete allocated nodes
        temp = head;
        struct node *prev = NULL;
        while (temp != NULL)
        {
            if (temp->share_id != -1)
            {
                if (prev == NULL)
                {
                    head = temp->next;
                    free(temp);
                    temp = head;
                }
                else
                {
                    prev->next = temp->next;
                    free(temp);
                    temp = prev->next;
                }
            }
            else
            {
                prev = temp;
                temp = temp->next;
            }
        }

        // Store share_id, tss_id and task_id in allocation_seq
        temp = newList;
        while (temp != NULL)
        {
            if (temp->share_id != -1)
            {
                int pc = 0;
                int current_share_id = temp->share_id;
                while (1)
                {
                    if (current_share_id > capacity[pc])
                    {
                        current_share_id = current_share_id - capacity[pc];
                        allotTask(temp->task_id, pc, capacity[pc]);
                        capacity[pc] = 0;
                        current_share_id = ((next_pc[pc] / current_pc[pc]) * current_share_id);
                        pc = next_pc[pc];
                    }
                    else if (current_share_id <= capacity[pc])
                    {
                        allotTask(temp->task_id, pc, current_share_id);
                        capacity[pc] = capacity[pc] - current_share_id;
                        break;
                    }
                }
            }
            temp = temp->next;
        }
    }*/