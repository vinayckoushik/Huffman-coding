#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//Structure to build tree
typedef struct ex
{
		int freq;
		unsigned char ch;
		struct ex* next;
		struct ex* prev;
}ex;

struct ex* head;

//Compare function to sort
int cmpfnc(const void *a,const void *b)
{
		const ex **x=(const ex **) a;
		const ex **y=(const ex **) b;
		if((*x)->freq ==(*y)->freq)
				return ((*x)->ch < (*y)->ch) ? 1:-1;
		else
				return((*x)->freq < (*y)->freq) ? 1:-1;

}

//Building the tree function
void ctree(int count, ex *ptr3[])
{
		count = count - 1;

		while(count > 0)
		{
				ex *build=(ex *)malloc(sizeof(ex *));
				build->freq=ptr3[count]->freq+ptr3[count-1]->freq;
				ptr3[count]->prev=build;
				ptr3[count]->next=NULL;
				ptr3[count-1]->prev=NULL;
				ptr3[count-1]->next=build;
				ptr3[count-1] = build;
				qsort(ptr3, count, sizeof(ex *), cmpfnc);
				count--;

		}
}

//Adding elements to linked list
int addlist(int v1[])
{
		int i,count=0;
		struct ex* temp;

		for(i=0;i<256;i++)
		{
				if(v1[i]!=0)
				{
						if(count==0)
						{
								struct ex* newv=(struct ex*)malloc(sizeof(struct ex*));
								newv->prev=NULL;
								temp=newv;
								newv->freq=v1[i];
								newv->ch=(char)i;
								head=newv;

						}
						else
						{
								struct ex* newv=(struct ex*)malloc(sizeof(struct ex*));
								newv->freq=v1[i];
								newv->ch=(char)i;
								temp->next=newv;
								newv->prev=temp;
								temp=newv;
						}
						count++;
				}
		}

		temp->next=NULL;
		return(count);
}



int main(int argc,char *argv[])
{
		int n,r,i,count,k,j,total=0,ct2=0, z; 
		int buffer;
		unsigned int bf2=0;
		struct ex *ptr[256];
		struct ex *ptr2[256];
		struct ex *temp;
		unsigned int v[256];
		FILE *fpt = fopen(argv[1],"rb"),*fpo;
		unsigned char C;
		unsigned int **table=(unsigned int **)malloc(256 * sizeof(unsigned int *));
		struct ex* newc;
		int powval=0;

		head=NULL;

		for(j=0;j<256;j++)
		{
				table[j]=(unsigned int *)calloc(3, sizeof (int));
		}

		for(n=0;n<256;n++)
		{
				v[n]=0;
		}

		/* Frequency Distribution*/
		n=0;
		while(fread(&C, 1, 1, fpt) > 0)
		{
				n++;
				v[C]++;
		}
		total=n;

		/* Create Doubly Linked List */
		count=addlist(v);

		newc=head;

		i=0;

		while(newc != NULL)
		{
				ptr[i] = newc;
				ptr2[i] = newc;
				newc = newc -> next;
				i++;
		}

		/* Build tree */
		qsort(ptr2,count,sizeof(ex *),cmpfnc);
		ctree(count,ptr2);

		/* Get codes */
		for(i = 0;i < count; i++)
		{
				temp=ptr[i];
				j=0;
				buffer=0;
				while(temp!=ptr2[0])
				{
						if(temp->prev==NULL)
						{
								k=0;
								temp=temp->next;
						}
						else if(temp->next==NULL)
						{
								k=1;
								temp=temp->prev;
						}
						buffer=buffer+(k<<j);
						j++;
				}
				table[i][0]=(int)ptr[i]->ch;
				table[i][1]=buffer;
				table[i][2]=j;
		}

		fpo=fopen("huff_encode","wb");

		/* Write file size and number of characters */
		fwrite(&total,1,sizeof(int),fpo);
		fwrite(&count, 1, sizeof (int), fpo);

		/* Write Frequency to a file */
		for(i=0; i<count; i++)
		{
			fwrite(&ptr[i]->ch, 1, 1, fpo);
			fwrite(&ptr[i]->freq, 1, sizeof (int), fpo);
		}

		j=0;
		k=0;
		buffer=0;
		r=0;

		fseek (fpt, 0, SEEK_SET);

		/* Huff_code */

		while(fread(&C, 1, 1, fpt) > 0)
		{

				for(i = 0; i < count; i++)
				{
						if((unsigned char)table[i][0] == C)
						{
								r = table[i][2];
								buffer = (buffer<<r) + table[i][1];
								k=k+r;

								while(k>8)
								{
										bf2=0;
										bf2=(buffer << (32-k));
										C=bf2>>24;
										fwrite(&C,1,1,fpo);
										k=k-8;
										powval=pow(2,k)-1;	
										buffer=buffer & powval;
								}

								break;
						}
				}
		}

		if(k!=0)
		{
				bf2=0;
				bf2=(buffer << (32-k));
				C=bf2>>24;
				fwrite(&C,1,1,fpo);
		}

		/* Close files */
		fcloseall();

		return 0;
}
