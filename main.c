#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "onthefly.h"


int main(int argc, char **argv){
	
	State *s0=malloc(sizeof(State)),*s1=malloc(sizeof(State)),*s2=malloc(sizeof(State)),*s3=malloc(sizeof(State)),*s4=malloc(sizeof(State));
	s0->n=0;
	s0->len_out=2;
	s0->len_in=1;
	s0->ap[0]='p';
	s0->ap[1]='q';
	s0->out=malloc(sizeof(State*)*2);
	s0->out[0]=s1;
	s0->out[1]=s2;
	s0->leaf=0;
	//s0->in=NULL;
	s1->n=1;
	s1->len_out=1;
	s1->len_in=1;
	s1->ap[0]='p';
	s1->ap[1]='\0';
	s1->out=malloc(sizeof(State*));
	s1->out[0]=s3;
	s1->leaf=0;
	//s1->in=malloc(sizeof(State*));
	//s1->in[0]=s0;
	s2->n=2;
	s2->len_out=1;
	s2->len_in=1;
	s2->ap[0]='q';
	s2->ap[1]='\0';
	s2->out=malloc(sizeof(State*));
	s2->out[0]=s3;
	s2->leaf=0;
	//s2->in=malloc(sizeof(State*));
	//s2->in[0]=s0;
	s3->n=3;
	s3->len_out=1;
	s3->len_in=2;
	s3->ap[0]='\0';
	s3->ap[1]='\0';
	s3->out=malloc(sizeof(State*));
	s3->out[0]=s4;
	s3->leaf=0;
	//s3->in=malloc(sizeof(State*)*2);
	//s3->in[0]=s1;
	//s3->in[1]=s2;
	s4->n=4;
	s4->len_out=1;
	s4->len_in=1;
	s4->ap[0]='p';
	s4->ap[1]='\0';
	s4->out=malloc(sizeof(State*));
	s4->out[0]=s4;
	s4->leaf=1;
	//s4->in=malloc(sizeof(State*)*2);
	//s4->in[0]=s3;
	//s4->in[1]=s4;
	
	DAG *d= malloc(sizeof(DAG));
	d->num_states=5;
	d->q_0=s0;
	d->s=malloc(sizeof(State)*5);
	d->s[0]=s0;
	d->s[1]=s1;
	d->s[2]=s2;
	d->s[3]=s3;
	d->s[4]=s4;
	d->ap[0]='p';
	d->ap[1]='q';
	//model_checking();
	encode_props("AG(p)",6);
	free_DAG(d);
	return 0;
}

