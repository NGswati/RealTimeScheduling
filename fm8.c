#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <math.h>

#define MAX_ROWS 20
#define MAX_PC 4
#define MAX_FREQUENCY 7

int ex_t1[MAX_ROWS], ex_t2[MAX_ROWS], ex_t3[MAX_ROWS], ex_t4[MAX_ROWS];
int temp1[MAX_ROWS], temp2[MAX_ROWS], temp3[MAX_ROWS], temp4[MAX_ROWS];
int share1[MAX_ROWS], share2[MAX_ROWS], share3[MAX_ROWS], share4[MAX_ROWS];
int deadline[MAX_ROWS], rem_deadline[MAX_ROWS];
int temp_array[MAX_PC][MAX_ROWS]={0}, avg_temp[MAX_PC], peak_temp[MAX_PC], end_temp[MAX_PC];
double frequency_array[MAX_FREQUENCY]={.1,.2,.4,.6,.8,.9,1.0};

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
    struct data *present;
    if (*head_ref == NULL || (*head_ref)->share_id >= new_node->share_id)
    {
        new_node->next = *head_ref;
        *head_ref = new_node;
    }
    else
    {
        present = *head_ref;
        while (present->next != NULL && present->next->share_id < new_node->share_id)
        {
            present = present->next;
        }
        new_node->next = present->next;
        present->next = new_node;
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

int max(int arr[], int n)
{
    int max = -INT_MAX;
    for (int i = 0; i < n; i++)
    {
        if (arr[i] > max)
        {
            max = arr[i];
        }
    }
    return max;
}


int share_func(int et[MAX_ROWS], int sm[MAX_ROWS], int dl[MAX_ROWS], int dlr[MAX_ROWS])
{
    int g = min(dlr, MAX_ROWS);
    for (int i = 0; i < MAX_ROWS; i++)
    {
        sm[i] = ((float)et[i] /(float)dl[i]) * g;
        int x = 0;
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
    while (row < MAX_ROWS && fscanf(fp, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d", &test, &ex_t1[row], &ex_t2[row], &ex_t3[row], &ex_t4[row], &deadline[row], &temp1[row], &temp2[row], &temp3[row], &temp4[row]) == 10)
    {
        row++;
    }
    fclose(fp);
}

int provide_ex_t(int pc_id, int task_id)
{
    int exec;
    switch (pc_id)
    {
    case 1:
        exec = ex_t2[task_id];
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

struct data *copy_linkedlist(struct data *head, struct data *head_copy){ 
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
    return head_copy;
}

struct data *remaining_grouped_task(struct data *head, int task_allocation[MAX_ROWS])
{
    int task_allocated[MAX_ROWS] = {0};
    for (int i = 0; i < MAX_ROWS; i++)
    {
        task_allocated[i] = task_allocation[i];
    }
    struct data *prev = NULL;
    struct data *temp = head;
    while (temp != NULL)
    {
        if (task_allocated[temp->task_id] == 1)
        {
            if (prev == NULL)
            {
                head = temp->next;
            }
            else
            {
                prev->next = temp->next;
            }
            free(temp);
            temp = prev == NULL ? head : prev->next;
        }
        else
        {
            prev = temp;
            temp = temp->next;
        }
    }

    temp = NULL;
    struct data *pt = head;
    temp = create_node(0, 0, 0, 0);
    struct data *head_copy = temp;
    while (pt != NULL)
    {
        prev = pt;
        while (prev != NULL && task_allocated[pt->task_id] != 1)
        {
            if (prev->task_id == pt->task_id)
            {
                struct data *new_node = create_node(prev->task_id, prev->share_id, prev->pc_id, prev->tss);

                temp->next = new_node;
                temp = temp->next;
            }
            prev = prev->next;
        }
        task_allocated[pt->task_id] = 1;
        pt = pt->next;
    }
    return head_copy->next;
}



double temp_counter(int core_temp, int tempTSS, int tempshr)
{
    double T_e, B;
    // int tempshr;

    // tempTSS = 70;
    // tempshr = 200;
    // core_temp = 25;
    B = 0.003;
    T_e = tempTSS + ((core_temp - tempTSS)) / exp(B * tempshr);

    return T_e;
}

void avg_peak_end_temperature(struct Node *Allocated_seq[MAX_PC][MAX_ROWS], int nor_task_Core[MAX_PC])
{
    for (int i = 0; i < MAX_PC; i++)
    {
        
        int core_temp = 25, sum = 0;
        for (int j = 0; j < nor_task_Core[i]; j++)
        {
            if (!Allocated_seq[i][j])
            {
                temp_array[i][j] = temp_counter(core_temp, 25, Allocated_seq[i][j]->share_id);
                core_temp = temp_counter(core_temp, 25, Allocated_seq[i][j]->share_id);
            }
            else
            {
                temp_array[i][j] = temp_counter(core_temp, Allocated_seq[i][j]->tss, Allocated_seq[i][j]->share_id);
                core_temp = temp_counter(core_temp, Allocated_seq[i][j]->tss, Allocated_seq[i][j]->share_id);
            }
        }
        for (int j = 0; j < nor_task_Core[i]; j++)
        {
            sum += temp_array[i][j]; // printf("%d ", temp_array[i][j]);
        }
        avg_temp[i] = sum / nor_task_Core[i];
        peak_temp[i] = max(temp_array[i], nor_task_Core[i]);
        end_temp[i] = temp_array[i][nor_task_Core[i] - 1];
    }
}

int find_min_index(struct Node *Allocate_seq[MAX_PC][MAX_ROWS],int nor_task_core[MAX_ROWS], int pc_id)
{
    int min_index = 0;
    int min_value = Allocate_seq[pc_id][0]->tss;
    int i = 0;

    while(1)
    {
        if (Allocate_seq[pc_id][i]->tss > 0)
        {
            min_index = i;
            min_value = Allocate_seq[pc_id][i]->tss;
            break;
        }
        i++;
    }

    for ( i = 0; i < nor_task_core[pc_id]; i++)
    {
        if (Allocate_seq[pc_id][i]->tss > 0 && Allocate_seq[pc_id][i]->tss < min_value)
        {
            min_value = Allocate_seq[pc_id][i]->tss;
            min_index = i;
        }
    }
    Allocate_seq[pc_id][min_index]->tss = 0;
    return min_index;
}

int find_max_index(struct Node *Allocate_seq[MAX_PC][MAX_ROWS], int nor_task_core[MAX_ROWS], int pc_id)
{
    int max_index = 0;
    int max_value = Allocate_seq[pc_id][0]->tss;
    for (int i = 1; i < nor_task_core[pc_id]; i++)
    {
        if (Allocate_seq[pc_id][i]->tss > max_value)
        {
            max_value = Allocate_seq[pc_id][i]->tss;
            max_index = i;
        }
    }
    Allocate_seq[pc_id][max_index]->tss = 0;
    return max_index;
}

void max_min_insert(struct Node *Allocate_seq[MAX_PC][MAX_ROWS], struct Node *shuffled_Allocated_seq[MAX_PC][MAX_ROWS], int nor_task_Core[MAX_PC])
{
    int index,count;
    struct Node *copy_allocate[MAX_PC][MAX_ROWS];
    for(int i=0;i<MAX_PC;i++){
        for(int j=0;j<nor_task_Core[i];j++){
            copy_allocate[i][j] = create_allonode(Allocate_seq[i][j]->task_id, Allocate_seq[i][j]->share_id, Allocate_seq[i][j]->tss );
        }
    }
    for (int i=0;i<MAX_PC;i++){
        count = 0;
        for (int j = 0; j < nor_task_Core[i]; j++)
        {
            if(j%2==0){

                index = find_max_index(copy_allocate, nor_task_Core, i);
                shuffled_Allocated_seq[i][count++] = create_allonode(Allocate_seq[i][index]->task_id, Allocate_seq[i][index]->share_id, Allocate_seq[i][index]->tss);
            }
            else{
                index = find_min_index(copy_allocate, nor_task_Core, i);
                shuffled_Allocated_seq[i][count++] = create_allonode(Allocate_seq[i][index]->task_id, Allocate_seq[i][index]->share_id, Allocate_seq[i][index]->tss);
            }
        }
    }
}

double find_nearest_freq(double frequency){
    int index=0;
    for (int i=0;i<MAX_FREQUENCY;i++){
        if (frequency <= frequency_array[i] && frequency >= frequency_array[i-1])
        {
            index=i;
            break;
        }
    }
    return frequency_array[index];
}

int file_write(char *name)
{
    const char *filename = "tempresult.txt";
    FILE *fp = fopen(filename, "a");
    if (!fp)
    {
        printf("Error: could not open file %s\n", filename);
        return 1;
    }

    fprintf(fp, "\nprinting temperatures (%s):\nPc_id  End_temp  peak_temp  Avg_temp\n", name);
    for (int i = 0; i < MAX_PC; i++)
    {
        fprintf(fp, " %d       %d        %d        %d\n", i, end_temp[i], peak_temp[i], avg_temp[i]);
    }

    fclose(fp);
    printf("\nOutput written to %s\n", filename);
    return 0;
}
int main() // int argc, char *argv
{
    int row = 0;
    char *filename = "Set_1_m_4_U_80_n_20_tss_70.txt"; // argv[1]
    Readfile(filename);

    while (row < MAX_ROWS)
    {
        rem_deadline[row] = deadline[row];
        row++;
    }

    // printf("Printing file value:\n");
    // for (row = 0; row < MAX_ROWS; row++){            
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
    }

    struct data *ptr = head;
    // printf("\nprinting structure of node formed:\n");
    // while (ptr != NULL)                  
    // {
    //     printf("task_id: %d, share_id: %d, pc_id: %d, tss: %d\n", ptr->task_id, ptr->share_id, ptr->pc_id, ptr->tss);
    //     ptr = ptr->next;
    // }

    int g = min(deadline, MAX_ROWS);
    int Capacity[MAX_PC], task_allocation_flag[MAX_ROWS] = {0};
    struct Node *Allocation_seq[MAX_PC][MAX_ROWS] = {0};
    int nor_task_Core[MAX_PC] = {0};
    for (int i = 0; i < MAX_PC; i++)
    {
        Capacity[i] = g; // 27000; // 40000 //g
    }

    // scheduling Non migrating task
    ptr = head;

    while (ptr != NULL)
    {
        while (ptr != NULL && (task_allocation_flag[ptr->task_id] == 1 || ptr->share_id > Capacity[ptr->pc_id]))
        {
            ptr = ptr->next;
        }
        if (ptr != NULL)
        {
            Allocation_seq[ptr->pc_id][nor_task_Core[ptr->pc_id]++] = create_allonode(ptr->task_id, ptr->share_id, ptr->tss);
            Capacity[ptr->pc_id] -= ptr->share_id;
            task_allocation_flag[ptr->task_id] = 1;
            // remaining deadline
            ptr = ptr->next;
        }
    }
    // printf("\nprinting allocation sequence after non migratiing scheduling:\n");
    // for (int i = 0; i < MAX_PC; i++) 
    // {
    //     for (int j = 0; j < nor_task_Core[i]; j++)
    //     {
    //         struct Node *present_node = Allocation_seq[i][j];
    //         printf("%d ", present_node->task_id);
    //     }
    //     printf("\n");
    // }

    // printf("Remaining capacity after non-migrating :\n");
    // for (int i = 0; i < MAX_PC; i++)
    // {
    //     printf("%d ", Capacity[i]);
    // }
    // printf("\n");

    struct data *head_copy = copy_linkedlist(head, head_copy);

    // printf("\nprinting structure for copied linked list:\n");
    // ptr = head_copy;
    // while (ptr != NULL)                
    // {
    //     printf("task_id: %d, share_id: %d, pc_id: %d, tss: %d\n", ptr->task_id, ptr->share_id, ptr->pc_id, ptr->tss);
    //     ptr = ptr->next;
    // }
    head_copy = remaining_grouped_task(head_copy, task_allocation_flag); // deleted and grouped task id

    // printf("\nprinting grouped structure for remaining task :\n");
    // ptr = head_copy;
    // while (ptr != NULL) // printing structure
    // {
    //     printf("task_id: %d, share_id: %d, pc_id: %d, tss: %d\n", ptr->task_id, ptr->share_id, ptr->pc_id, ptr->tss);
    //     ptr = ptr->next;
    // }


    //  scheduling Migrating Task

    ptr = head_copy;
    while (ptr != NULL)
    {
        if (task_allocation_flag[ptr->task_id] == 0)
        {
            task_allocation_flag[ptr->task_id] = 1;
            int present_share_id = ptr->share_id;
            int present_pc = ptr->pc_id;

            while (present_share_id > 0)
            {
                if (present_share_id > Capacity[present_pc] && Capacity[present_pc] != 0)
                {
                    Allocation_seq[present_pc][nor_task_Core[present_pc]++] = create_allonode(ptr->task_id, Capacity[present_pc], ptr->tss);
                    present_share_id -= Capacity[present_pc];
                    Capacity[present_pc] = 0;
                    int this_pc = ptr->pc_id, this_task_id = ptr->task_id;
                    while (Capacity[ptr->pc_id] == 0 && ptr->next->task_id == ptr->task_id)
                    {
                        ptr = ptr->next;
                        present_pc = ptr->pc_id;
                    }
                    present_share_id = (provide_ex_t(ptr->task_id, present_pc) * present_share_id) / provide_ex_t(this_task_id, this_pc);
                }
                else if (present_share_id <= Capacity[present_pc])
                {
                    Allocation_seq[present_pc][nor_task_Core[present_pc]++] = create_allonode(ptr->task_id, present_share_id, ptr->tss);
                    Capacity[present_pc] -= present_share_id;
                    present_share_id = 0;
                }
                else if (Capacity[present_pc] == 0)
                {
                    while (Capacity[ptr->pc_id] == 0 && ptr->next->task_id == ptr->task_id)
                    {
                        ptr = ptr->next;
                        present_pc = ptr->pc_id;
                    }
                }
                else
                {
                    break;
                }
            }
        }
        ptr = ptr->next;
    }

    // printf("\nprinting allocation sequence after migrating scheduling\n");
    // for (int i = 0; i < MAX_PC; i++) 
    // {
    //     for (int j = 0; j < nor_task_Core[i]; j++)
    //     {
    //         struct Node *present_node = Allocation_seq[i][j];
    //         printf("%d  ", present_node->task_id);
    //     }
    //     printf("\n");
    // }

    printf("Remaining capacity after migrating scheduling:\n");
    for (int i = 0; i < MAX_PC; i++)
    {
        printf("%d ", Capacity[i]);
    }
    printf("\n");

    for (int i = 0; i < MAX_PC; i++)
    {
        if (Capacity[i] > 0)
        {
            Allocation_seq[i][nor_task_Core[i]++] = create_allonode(20, Capacity[i], 25);
        }
    }

    // printf("\nshare id for particular task id after allocation:\n ");
    // for (int i = 0; i < 4; i++)
    // {
    //     for (int j = 0; j < nor_task_Core[i]; j++)
    //     {
    //         printf("%d ->  %d\n", Allocation_seq[i][j]->task_id,Allocation_seq[i][j]->share_id );
    //     }
    // }

    avg_peak_end_temperature(Allocation_seq, nor_task_Core);
    printf("\nprinting the temprature of each task id: \n");
    for (int i=0;i<MAX_PC;i++){
        for (int j = 0; j <nor_task_Core[i]; j++)
        {
            printf("%d-t:%d   ", Allocation_seq[i][j]->task_id, temp_array[i][j]);
        }
        printf("\n");
    }
 
    printf("\nprinting temperatures (NORMAL):\nPc_id  End_temp  peak_temp  Avg_temp\n");
    for (int i = 0; i < MAX_PC; i++)
    {
        printf(" %d       %d        %d        %d\n", i, end_temp[i], peak_temp[i], avg_temp[i]);
    }
    file_write("NORMAL");

    struct Node *shuffled_Allocation_seq[MAX_PC][MAX_ROWS] = {0};
    max_min_insert(Allocation_seq, shuffled_Allocation_seq, nor_task_Core);

    for (int i = 0; i < MAX_PC; i++)
    {
        for (int j = 0; j < nor_task_Core[i]; j++)
        {
            printf("%d-%d ", Allocation_seq[i][j]->task_id, temp_array[i][j]);
        }
        printf("\n");
    }
    avg_peak_end_temperature(shuffled_Allocation_seq, nor_task_Core);
    printf("\nprinting the temprature of each task id: \n");
    for (int i = 0; i < MAX_PC; i++)
    {
        for (int j = 0; j < nor_task_Core[i]; j++)
        {
            printf("%d-%d ", shuffled_Allocation_seq[i][j]->task_id, temp_array[i][j]);
        }
        printf("\n");
    }

    printf("\nprinting temperatures (SHUFFLED):\nPc_id  End_temp  peak_temp  Avg_temp\n");
    for (int i = 0; i < MAX_PC; i++)
    {
        printf(" %d       %d        %d        %d\n", i, end_temp[i], peak_temp[i], avg_temp[i]);
    }
    file_write("SHUFFLED");

    for (int i = 0; i < MAX_PC; i++)
    {
        for (int j = 0; j < nor_task_Core[i]; j++)
        {
            if (Allocation_seq[i][j]->task_id == 20)
            {
                free(Allocation_seq[i][j]);
                nor_task_Core[i]--;
            }
        }
    }
    
    //dvfs
    double req_frequency, frequency;
    for(int i=0;i<MAX_PC;i++){
        req_frequency =(double) (g-Capacity[i]) / (double) g;
        frequency=find_nearest_freq(req_frequency);
        printf("\nEnregy Consumed by PC %d: %lf",i, g * frequency*frequency*frequency);
        for(int j=0;j<nor_task_Core[i];j++){
            Allocation_seq[i][j]->share_id = Allocation_seq[i][j]->share_id / frequency;
            Allocation_seq[i][j]->tss = Allocation_seq[i][j]->tss * frequency;
            Capacity[i]=0;
        }
    }
    printf("\nOriginal Allocation After dvfs:\n"); 
    for (int i = 0; i < MAX_PC; i++)
    {
        for (int j = 0; j < nor_task_Core[i]; j++)
        {
            printf("%d ", Allocation_seq[i][j]->task_id);
        }
        printf("\n");
    }

    avg_peak_end_temperature(Allocation_seq, nor_task_Core);

    printf("\n\nprinting the temprature of each task id: \n");
    for (int i = 0; i < MAX_PC; i++)
    {
        for (int j = 0; j < nor_task_Core[i]; j++)
        {
            printf("%d-t:%d %d ", Allocation_seq[i][j]->task_id, Allocation_seq[i][j]->tss, temp_array[i][j]);
        }
        printf("\n");
    }
    printf("\nprinting temperatures (DVFS NORMAL):\nPc_id  End_temp  peak_temp  Avg_temp\n");
    for (int i = 0; i < MAX_PC; i++)
    {
        printf(" %d       %d        %d        %d\n", i, end_temp[i], peak_temp[i], avg_temp[i]);
    }
    file_write("DVFS NORMAL");
    // struct Node *shuffled2_Allocation_seq[MAX_PC][MAX_ROWS] = {0};
    max_min_insert(Allocation_seq, shuffled_Allocation_seq, nor_task_Core);

    for (int i = 0; i < MAX_PC; i++)
    {
        for (int j = 0; j < nor_task_Core[i]; j++)
        {
            printf("%d-%d ", Allocation_seq[i][j]->task_id, temp_array[i][j]);
        }
        printf("\n");
    }
    avg_peak_end_temperature(shuffled_Allocation_seq, nor_task_Core);
    printf("\nprinting the temprature of each task id: \n");
    for (int i = 0; i < MAX_PC; i++)
    {
        for (int j = 0; j < nor_task_Core[i]; j++)
        {
            printf("%d-%d ", shuffled_Allocation_seq[i][j]->task_id, temp_array[i][j]);
        }
        printf("\n");
    }

    printf("\nprinting temperatures (DVFS SHUFFLED):\nPc_id  End_temp  peak_temp  Avg_temp\n");
    for (int i = 0; i < MAX_PC; i++)
    {
        printf(" %d       %d        %d        %d\n", i, end_temp[i], peak_temp[i], avg_temp[i]);
    }
    file_write("DVFS SHUFFLED");
    //     return 0;
}

// BIN SECTION

// FILE *fp = fopen("tempresult.txt", "w");
// fprintf(fp, "\tTEMPERATURE DETAILS\n");
// fclose(fp);
