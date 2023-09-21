#include <stdio.h>   //at g=28000 it gives a segmentation fault
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define MAX_ROWS 20
#define MAX_PC 4

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
int provide_ex_t(int pc_id, int task_id)
{
    int exec;
    switch(pc_id){
        case 1:
        exec=ex_t2[task_id];
        break;
        case 2:
        exec = ex_t3[task_id];
        break;
        case 3:
        exec = ex_t4[task_id];
        break;
        case 0:
        exec = ex_t1[task_id];
        break;
    }
    return exec;
}
void group_by_task_id(struct data *head)
{
    if (head == NULL)
    {
        return; // If the linked list is empty, there's nothing to group
    }

    struct data *current = head;

    while (current != NULL)
    {
        // Find the last node with the same task_id as the current node
        struct data *last = current;
        while (last->next != NULL && last->next->task_id == current->task_id)
        {
            last = last->next;
        }

        // If there's only one node with this task_id, move on to the next node
        if (last == current)
        {
            current = current->next;
        }
        // Otherwise, group the nodes with the same task_id together
        else
        {
            // Disconnect the group of nodes from the linked list
            struct data *group_head = current;
            struct data *group_tail = last->next;
            last->next = NULL;

            // Reverse the order of the group of nodes
            struct data *prev = NULL;
            while (group_head != NULL)
            {
                struct data *next = group_head->next;
                group_head->next = prev;
                prev = group_head;
                group_head = next;
            }

            // Insert the group of nodes back into the linked list
            if (current == head)
            {
                head = prev;
            }
            else
            {
                struct data *prev_node = head;
                while (prev_node->next != current)
                {
                    prev_node = prev_node->next;
                }
                prev_node->next = prev;
            }
            group_tail->next = current->next;
            current->next = prev;

            // Move on to the next node
            current = group_tail;
        }
    }
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

    // scheduling Non migrating task
    int g = min(deadline, MAX_ROWS);
    // printf("%d",g);
    int Capacity[MAX_PC], task_allocation_flag[MAX_ROWS] = {0};
    struct Node *Allocation_seq[MAX_PC][MAX_ROWS] = {0};
    int no_task_Core[MAX_PC] = {0};
    for (int i = 0; i < MAX_PC; i++)
    {
        Capacity[i] = 25000; // g // use
    }

    struct data *ptr = head;

    while (ptr != NULL)
    {
        while (ptr != NULL && (task_allocation_flag[ptr->task_id] == 1 || ptr->share_id > Capacity[ptr->pc_id]))
        {
            ptr = ptr->next;
        }
        if (ptr != NULL)
        {
            Allocation_seq[ptr->pc_id][no_task_Core[ptr->pc_id]++] = create_allonode(ptr->task_id, ptr->share_id, ptr->tss);
            Capacity[ptr->pc_id] -= ptr->share_id;
            task_allocation_flag[ptr->task_id] = 1;
            // remaining deadline
            ptr = ptr->next;
        }
    }

    // for (int i = 0; i < MAX_PC; i++)        //printing allocation sequence after non migratiing scheduling
    // {
    //     for (int j = 0; j < no_task_Core[i]; j++)
    //     {
    //         struct Node *current_node = Allocation_seq[i][j];
    //         printf("%d ", current_node->task_id);
    //     }
    //     printf("\n");
    // }

    struct data *head_copy = NULL; // Copy linked list
    struct data *tail_copy = NULL;
    struct data *temp = head;
    while (temp != NULL)
    {
        struct data *new_node = (struct data *)malloc(sizeof(struct data));
        new_node->task_id = temp->task_id;
        new_node->share_id = temp->share_id;
        new_node->tss = temp->tss;
        new_node->pc_id = temp->pc_id;
        new_node->next = NULL;
        if (head_copy == NULL)
        {
            head_copy = new_node;
            tail_copy = new_node;
        }
        else
        {
            tail_copy->next = new_node;
            tail_copy = new_node;
        }
        temp = temp->next;
    }
    // ptr = head_copy;
    // while (ptr != NULL)                   // printing structure
    // {
    //     printf("task_id: %d, share_id: %d, pc_id: %d, tss: %d\n", ptr->task_id, ptr->share_id, ptr->pc_id, ptr->tss);
    //     ptr = ptr->next;
    // }

    struct data *prev = NULL; // Delete allocated nodes
    temp = head_copy;
    while (temp != NULL)
    {
        if (task_allocation_flag[temp->task_id] == 1)
        {
            if (prev == NULL)
            {
                head_copy = temp->next;
            }
            else
            {
                prev->next = temp->next;
            }
            free(temp);
            temp = prev == NULL ? head_copy : prev->next;
        }
        else
        {
            prev = temp;
            temp = temp->next;
        }
    }
    ptr = head_copy;
    while (ptr != NULL)                   // printing structure
    {
        printf("task_id: %d, share_id: %d, pc_id: %d, tss: %d\n", ptr->task_id, ptr->share_id, ptr->pc_id, ptr->tss);
        ptr = ptr->next;
    }
    printf("check\n");
    struct data *h = head_copy; // grouping according to task id
    struct data *pt = h;
    if(h!=NULL){
        ptr = h->next;
    }
    while (h != NULL && h->next != NULL)
    {
        if (ptr != NULL && h->next == ptr && h->task_id == ptr->task_id)
        {
            h = h->next;
        }
        else if (ptr != NULL && h->task_id == ptr->task_id)
        {
            pt->next = ptr->next;
            ptr->next = h->next;
            h->next = ptr;
            h = ptr;
            ptr = pt->next;
        }
        else if (ptr == NULL || ptr == h)
        {
            pt = h;
            h = h->next;
            ptr = h;
        }
        ptr = ptr->next;
        pt = pt->next;
    }
    printf("check");
    ptr = head_copy;
    while (ptr != NULL)                   // printing structure
    {
        printf("task_id: %d, share_id: %d, pc_id: %d, tss: %d\n", ptr->task_id, ptr->share_id, ptr->pc_id, ptr->tss);
        ptr = ptr->next; 
        printf("check");
    }
    printf("check");
    //  scheduling Migrating Task

    ptr = head_copy;
    while (ptr != NULL)
    {
        if (task_allocation_flag[ptr->task_id] == 0)
        {
            task_allocation_flag[ptr->task_id] = 1;
            int current_share_id = ptr->share_id;
            int current_pc = ptr->pc_id;

            while (current_share_id > 0)
            {
                if (current_share_id > Capacity[current_pc] && Capacity[current_pc] != 0)
                {
                    Allocation_seq[current_pc][no_task_Core[current_pc]++] = create_allonode(ptr->task_id, Capacity[current_pc], ptr->tss);
                    current_share_id -= Capacity[current_pc];
                    Capacity[current_pc] = 0;
                    int this_pc=ptr->pc_id,this_task_id=ptr->task_id;
                    while(Capacity[ptr->pc_id]==0 && ptr->next->task_id==ptr->task_id){
                        ptr = ptr->next;
                        current_pc = ptr->pc_id;
                    }
                    current_share_id = (provide_ex_t(ptr->task_id, current_pc) * current_share_id) / provide_ex_t(this_task_id, this_pc);
                }
                else if (current_share_id <= Capacity[current_pc])
                {
                    Allocation_seq[current_pc][no_task_Core[current_pc]++] = create_allonode(ptr->task_id, current_share_id, ptr->tss);
                    Capacity[current_pc] -= current_share_id;
                    break;
                }
                else{
                    break;
                }
            }
        }
        ptr = ptr->next;
    }

    for (int i = 0; i < MAX_PC; i++) // printing allocation sequence after migrating scheduling
    {
        for (int j = 0; j < no_task_Core[i]; j++)
        {
            struct Node *current_node = Allocation_seq[i][j];
            printf("%d ", current_node->task_id);
        }
        printf("\n");
    }
    for (int i = 0; i < MAX_PC; i++)
    {
        printf("%d ", Capacity[i] ); 
    }
    return 0;
}

// BIN SECTION

// void migrateTask()

//     temp = head_copy;
//     while (temp != NULL)
//     {
//         if (temp->share_id != -1)
//         {
//             int pc = 0;
//             int current_share_id = temp->share_id;
//             while (1)
//             {
//                 if (current_share_id > capacity[pc])
//                 {
//                     current_share_id = current_share_id - capacity[pc];
//                     allotTask(temp->task_id, pc, capacity[pc]);
//                     capacity[pc] = 0;
//                     current_share_id = ((next_pc[pc] / current_pc[pc]) * current_share_id);
//                     pc = next_pc[pc];
//                 }
//                 else if (current_share_id <= capacity[pc])
//                 {
//                     allotTask(temp->task_id, pc, current_share_id);
//                     capacity[pc] = capacity[pc] - current_share_id;
//                     break;
//                 }
//             }
//         }
//         temp = temp->next;
//     }
// }
// struct data *pt = temp;
// int false_task_allocation_flag[20] = {0};
// while (pt->task_id == temp->task_id && false_task_allocation_flag[temp->task_id] == 0)
// {
//     prev = pt;
//     pt = pt->next;
// }
// false_task_allocation_flag[temp->task_id] = 1;
// temp = temp->next;