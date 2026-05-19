#ifndef ONTHEFLY_H
#define ONTHEFLY_H

#define AP_SIZE 2

typedef struct graph_state{
	int n,len_out,len_in;
	char ap[AP_SIZE];
	struct graph_state **out;
	//struct graph_state **in;
	char leaf;
} State;

typedef struct dag_graph{
	int num_states;
	State *q_0;
	State **s;
	char ap[AP_SIZE];
} DAG;

typedef struct ctl_props{
	char op; //EF:4,EG=5,EX=6,&:7,|:8,T=9,F=10,AP=11,U=12								AF:1,AG:2,AX:3,EF:4,EG:5,EX:6,&:7,|:8,T=9,F=10,AP:11
	struct ctl_props *p,*q; // AF(p & q)
	char op_p,op_q; //operateurs des sous-formules p et q
	char ap;
	char negation;
} CTL;

int a_in_ap(char *l, char a);
CTL *encode_props(char * props, int len);
char check(State *q,CTL *ctl);
char model_checking(DAG *g, char *props, int len_props);
void free_State(DAG *dag);
void free_DAG(DAG *dag);

#define MAX_STATES 1024
extern State* all_states[MAX_STATES];
 
State* charger_etat(int target_id, const char* filename);
 
int check_EF(int current_id, char target_ap, const char* filename);
int check_EX(int current_id, char target_ap, const char* filename);
int check_EG(int current_id, char target_ap, const char* filename);
int check_nested_EF_EF(int current_id, char p, char q, const char* filename);
int check_EF_p_ET_feuille(int current_id, char target_ap, const char* filename);
int check_AF_p_ET_feuille(int current_id, char target_ap, const char* filename);
int check_EG_AF(int current_id, char target_ap, const char* filename);
int check_EF_EG(int current_id, char target_ap, const char* filename);
#endif
