#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct ex
{
		int freq;
		unsigned char ch;
		struct ex* next;
		struct ex* prev;
}ex;

struct ex* head;

/*Function to sort */
int cmpfnc(const void *a,const void *b)
{
		const ex **x=(const ex **) a;
		const ex **y=(const ex **) b;
		if((*x)->freq ==(*y)->freq)
				return ((*x)->ch < (*y)->ch) ? 1:-1;
		else
				return((*x)->freq < (*y)->freq) ? 1:-1;

}

/*Building tree from frequency count */
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

/*Adding to Linked List */
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
		int n,r,i,count,k,j,total=0, powval=0; 
		int buffer;
		unsigned int bf2=0;
		struct ex *ptr[256];
		struct ex *ptr2[256];
		struct ex *temp;
		unsigned int v[256],v1[256];
		FILE *fpt,*fpo=fopen("huff_decode","wb");
		unsigned char C;
		head=NULL;
		unsigned int **table=(unsigned int **)malloc(256 * sizeof(unsigned int *));
		struct ex* newc;

		for(j=0;j<256;j++)
		{
				table[j]=(unsigned int *)malloc(100);
		}

		for(n=0;n<256;n++)
		{
				v[n]=0;
		}

		fpt=fopen("huff_encode","rb");

		/*Read Header*/

		fread(&total,1,4,fpt);
		fread(&count,1,4,fpt);

		for(i=0;i<count;i++)
		{
				fread(&C,1,1,fpt);
				fread(&v[C],1,4,fpt);
		}

		/*Creat Doubly Linked list*/
		count=addlist(v);

		newc=head;
		i=0;

		while(newc!=NULL)
		{
				ptr[i]=newc;
				ptr2[i]=newc;
				newc=newc->next;
				i++;
		}
		i=0;
		qsort(ptr2, count, sizeof(ex *), cmpfnc);

		/*Build Tree*/
		ctree(count,ptr2);

		/*Build Table*/
		for(i=0;i<count;i++)
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

		buffer=0;
		j=0;
		k=0;

		/*Decode bits*/
		for(i=0; i<total; )
		{
				fread(&C,1,1,fpt);

				k=0;

				while(k < 8)
				{
						powval=pow(2, (7 - k));

						bf2 = (bf2 << 1) + ((C & powval)>>(7-k));

						j++;

						for(n=0;n<count;n++)
						{
								if((bf2 == table[n][1]) && (j == table[n][2]))
								{
										r=1;
										break;
								}
						}

						if(r == 1)
						{
								fwrite((char *)&table[n][0],1,1,fpo);

								i++;

								r=0;
								j=0;
								bf2=0;

								if(i == total)
								{
										break;
								}
						}

						k++;
				}
		}

		fclose(fpt);
		fclose(fpo);
}
