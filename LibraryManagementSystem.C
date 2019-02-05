#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<stdlib.h>

FILE* sfp;
FILE* bfp;
//FILE* ifp;

int i,j,k,m,n,qb=0,sl,bl,ap,ab,f;
int curi,curj,ib;
char booknm[20],sid[10];
int found=0;

/*struct issue
{
int isno;
char istuname[20];
char ibook[20];
char issdate[8];
char retdate[8];
};*/

struct book
{
char bpn[10];
char issue_date[9];
char return_date[9];
};

struct details
{
int sno;
char srollno[15];
char sname[15];
char dept[10];
int btaken;
struct book bissued[3];
}student[100],stu,stemp;

struct record
{
int snum;
char b_name[100],b_code[10];
int quantity;
}brecord[100],temp,bnm;

//struct issue ibk[100];

//struct issue ibkt,itemp;


void setDetails_stu(int i)
{
sfp=fopen("student.bin","ab");
student[i].sno=i;


printf("Enter STUDENTS ROLLNO:\n");
fflush(stdin);
gets(student[i].srollno);

printf("Enter STUDENTS NAME:\n");
gets(student[i].sname);

printf("Enter STUDENTS DEPARTMENT:\n");
gets(student[i].dept);

printf("Enter number of books with student:>\n");
scanf("%d",&student[i].btaken);
fflush(stdin);



for(k=1;k<=(student[i].btaken);k++)
{
printf("Enter book %d name:",k);
gets(student[i].bissued[k].bpn);
printf("BOOK %d Issue date:\n",k);
fflush(stdin);
gets(student[i].bissued[k].issue_date);
printf("Return date:\n");
fflush(stdin);
gets(student[i].bissued[k].return_date);
}
fwrite(&student[i],sizeof(struct details),1,sfp);

fclose(sfp);
}



void setDetails_book(int j)
{
bfp=fopen("book.bin","ab");

brecord[j].snum=j;
printf("Enter BookCode:\n");
fflush(stdin);
gets(brecord[j].b_code);
printf("Enter BookName:\n");
fflush(stdin);
gets(brecord[j].b_name);
printf("Enter number of such books present:\n");
scanf("%d",&brecord[j].quantity);

fwrite(&brecord[j],sizeof(struct record),1,bfp);
fclose(bfp);
}



void printdetails()
{
if((sfp=fopen("student.bin","rb"))==NULL)
{
    printf("\nDetails not found");
}
else
{
    printf("\nS.No\tRNo\tNAME\tDEPT\tBTaken\t");
 while((fread(&stemp,sizeof(struct details),1,sfp))==1)
 {
  printf("\n%d\t%s\t%s\t%s\t%d\n",stemp.sno,stemp.srollno,stemp.sname,stemp.dept,stemp.btaken);
 }

fclose(sfp);
}
}

void bkdetails(char i[])
{
int found=0;
if((sfp=fopen("student.bin","rb"))==NULL)
{
    printf("\nDetails not found");
}
else
{
while((fread(&stemp,sizeof(struct details),1,sfp))==1)
{
if(!(strcmp(stemp.srollno,i)))
{
    found=1;
    break;
}
}
if(found==0)
{
    printf("\nSTUDENT NOT FOUND");
}
else
{
    printf("\nStudentID: %s\n",stemp.srollno);
    for(int bt=1;bt<=stemp.btaken;bt++)
    {
        printf("\nBOOK%dIssueDate\tBOOK%dReturnDate\n",bt,bt);
        printf("\n%s\t%s\n",stemp.bissued[bt-1].issue_date,stemp.bissued[bt-1].return_date);
    }
}
}
}

void printrecords()
{
if((bfp=fopen("book.bin","rb"))==NULL)
{
    printf("\nDetails not found");
}
else
{
    printf("\nB.Code\tBkNm\tQuantity\n");
 while((fread(&temp,sizeof(struct record),1,bfp))==1)
 {
  printf("%s\t%s\t%d\n",temp.b_code,temp.b_name,temp.quantity);
 }
}
fclose(bfp);
}

int bookcheck()
{
int t=1;
bfp=fopen("book.bin","rb");
printf("Enter the Book Name to check its availability:\n");
scanf("%s",&booknm);

while((fread(&bnm,sizeof(struct record),1,bfp))==1)
{
if(!strcmp(bnm.b_name,booknm))
{
printf("BOOK AVAILABLE");
t=0;
break;
}
if(t==0)
break;
}
fclose(bfp);
return 1;
}


int getstu()
{
int s=0;
sfp=fopen("student.bin","rb");
while((fread(&stemp,sizeof(struct details),1,sfp))==1)
{
//s=stemp.sno;
s++;
}
fclose(sfp);
return s;
}

int getbkpre()
{
int b=0;
bfp=fopen("book.bin","rb");
while((fread(&temp,sizeof(struct record),1,bfp))==1)
{
b++;
}
fclose(bfp);
return b;
}

/*int issuebk()
{
int ib=0;
struct issue itemp;
ifp=fopen("issuerec.bin","rb");

while(fread(&itemp,sizeof(struct issue),1,ifp))
{
ib++;
}
printf("ib:%d",ib);
fclose(ifp);
return ib;
} */
		   /////////////////////////////
void returnbk()
{
int lm,k,g,sd;
char sdi[10];
int bn;
char rbn[15];
printf("Enter Student's ID:\n");
fflush(stdin);
gets(sdi);

sfp=fopen("student.bin","rb+");
bfp=fopen("book.bin","rb+");

//ifp=fopen("issuerec","rb");
while((fread(&stemp,sizeof(struct details),1,sfp))==1)
{
if(!strcmp(stemp.srollno,sdi))
{
k=stemp.sno;
do
{
printf("Enter BookToken Number to be returned:\n");
scanf("%d",&bn);
stemp.btaken--;
strcpy(stemp.bissued[bn].bpn,NULL);
strcpy(stemp.bissued[bn].return_date,NULL);
strcpy(stemp.bissued[bn].issue_date,NULL);

fseek(sfp,((k-1)*sizeof(struct details)),0);
fwrite(&stemp,sizeof(struct details),1,sfp);

printf("Enter ReturnedBook Name:\n");
fflush(stdin);
gets(rbn);

while((fread(&temp,sizeof(struct record),1,bfp))==1)
{
if(!strcmp(temp.b_name,rbn))
{
temp.quantity++;
k=temp.snum;
}
}
fseek(bfp,((k-1)*sizeof(struct record)),0);
fwrite(&temp,sizeof(struct record),1,bfp);



/*while((fread(&ibkt,sizeof(struct issue),1,ifp))==1)
{
if((!strcmp(ibkt.ibook,rbn)) &&(!strcmp(ibkt.istuname,stemp.sname)))
{

strcpy(ibk[ibkt.isno].ibook,"");
strcpy(ibk[ibkt.isno].istuname,"");
strcpy(ibk[ibkt.isno].issdate,"");
strcpy(ibk[ibkt.isno].retdate,"");
//lm=issuebk();

for(k=ibkt.isno;k<lm;k++)
{
strcpy(ibk[k].ibook,ibk[k+1].ibook);
strcpy(ibk[k].istuname,ibk[k+1].istuname);
strcpy(ibk[k].issdate,ibk[k+1].issdate);
strcpy(ibk[k].retdate,ibk[k+1].retdate);
}

break;*/


printf("Enter 1.Continue\n 0.Discontinue");
scanf("%d",&g);

}while(g!=0);
}
}
bkdetails(stemp.srollno);
}
      ////////////////////////////////////////
void renew()
{
int sd,bn,g=0;
char rbn[15];
sfp=fopen("student.bin","rb+");
bfp=fopen("book.bin","rb");
//ifp=fopen("issuerec.bin","ab+");

printf("Enter Student's RollNo:\n");
fflush(stdin);
gets(sid);

while((fread(&stemp,sizeof(struct details),1,sfp))==1)
{
if(!strcmp(stemp.srollno,sid))
{
sd=stemp.sno;
do
{
printf("Enter BookToken Number to be renewed:\n");
fflush(stdin);
scanf("%d",&bn);
printf("Enter new Renewal Date:\n");
fflush(stdin);
gets(stemp.bissued[bn].return_date);

printf("Renewed Date:\n");
puts(stemp.bissued[bn].return_date);

fseek(sfp,((sd-1)*sizeof(struct details)),0);
fwrite(&stemp,sizeof(struct details),1,sfp);

/*printf("Enter IssuedBook Name:\n");
fflush(stdin);
gets(rbn);

while((fread(&ibkt,sizeof(struct issue),1,ifp))==1)
{
if((!strcmp(ibkt.ibook,rbn)) &&(!strcmp(ibkt.istuname,stemp.sname)))
{
printf("New Renewal Date for book:\n");
fflush(stdin);
gets(ibkt.retdate);
break;*/

printf("Enter 1.Continue\n 0.Discontinue");
scanf("%d",&g);
}while(g!=0); //do

break;
} //oif
}               //owhile
/*while((fread(&stemp,sizeof(struct details),1,sfp))==1)
{
if(!strcmp(stemp.srollno,sid))
{
puts(stemp.bissued[bn].issue_date);
puts(stemp.bissued[bn].return_date);
}
} */
bkdetails(stemp.srollno);
fclose(sfp);
fclose(bfp);
//fclose(ifp);

}                //fn




	///////////MAIN/////////////////////

int main()
{
int c,ch;
char stuid[10];

int found=0;
sfp=fopen("student.bin","rb");
bfp=fopen("book.bin","rb");
//ifp=fopen("issuerec.bin","rb");


do
{
printf("\n\n-----------------------------------------------------\n");
printf("1.STUDENTS DETAILS\n\n2.BOOKS DETAILS\n\n3.TOTAL NUM OF BOOKS AVAILABLE\n\n");
printf("4.ENTER STUDENT DETAILS\n\n5.ENTER BOOK DETAILS\n\n6.STUDENT SERVICES\n\n7.CLOSE APPLICATION\n\n");

printf("Enter your choice of action:");
scanf("%d",&c);

switch(c)
{
case 1:
{
printdetails();

break;
}
case 2:
{
printrecords();

break;
}
case 3:
{
bfp=fopen("book.bin","rb");
qb=0;
while((fread(&temp,sizeof(struct record),1,bfp))==1)
{
qb=qb+temp.quantity;
}
printf("Total no. of Books in Library are %d\n\n",qb);
fclose(bfp);
break;
}
case 4:
{
printf("Enter number of Student's Details you need to Enter:\n");
scanf("%d",&n);

curi=getstu();

for(i=curi+1;i<=curi+n;i++)
{
printf("\nEnter STUDENT%d's INFO:\n",i);
setDetails_stu(i);
}
printf("SUCESSFULLY SAVED");

break;
}
case 5:
{
int p=0;
printf("Enter number of BookDetails you want to Enter:");
scanf("%d",&m);
curj=getbkpre();
p=curj+1;
while(p<=curj+m)
{
printf("P:%d\n",p);
printf("Enter BOOK%d's INFO:",p);
setDetails_book(p);
p++;
}

printf("SUCESSFULLY SAVED");

break;
}
case 6:
{
do
{
printf("\n---------------STUDENT SERVICES---------------\n");

printf("\n1.BOOK AVAILABILITY CHECK\n\n2.ISSUE\n\n3.RENEWAL\n\n4.RETURN\n\n5.BOOK DETAILS\n\n6.EXIT\n");
scanf("%d",&ch);
 switch(ch)
 {
 case 1:
 {
bookcheck();
 break;
}
 case 2:
 {
 struct record btemp;
 int g=1,s=0,u=1,w=0;
 sfp=fopen("student.bin","ab+");
 printf("Enter Students ID:\n");
 fflush(stdin);
 gets(stuid);
s=0;

while((fread(&stu,sizeof(struct details),1,sfp))==1)
{
s++;
if(!strcmp(stu.srollno,stuid))
{
printf("Student Details Found\n");
u=0;
}
 if(u==0)
 break;
 }
do
{
if(stu.btaken==3)
{
printf("Cant be Issued!Limit Reached\n");

}
else
{
bfp=fopen("book.bin","rb");
printf("Enter the Book Name to check its availability:\n");
scanf("%s",&booknm);
w=0;

while((fread(&bnm,sizeof(struct record),1,bfp))==1)
{
w++;
if(!strcmp(bnm.b_name,booknm))
{
printf("BOOK AVAILABLE\n");
found=1;
}
if(found==1)
break;
}

fclose(bfp);

if(found==1)
{
(bnm.quantity)=(bnm.quantity)-1;  //////////////
(stu.btaken)=((stu.btaken)+1);


printf("\nEnter Today's Date:\n");
fflush(stdin);
gets(stu.bissued[stu.btaken-1].issue_date);
printf("Enter the day on which book is to be returned:\n");
fflush(stdin);
gets(stu.bissued[stu.btaken-1].return_date);

bfp=fopen("book.bin","rb+");

fseek(bfp,((w-1)*sizeof(struct record)),0);
fwrite(&bnm,sizeof(struct record),1,bfp);


sfp=fopen("student.bin","rb+");

fclose(bfp);

fseek(sfp,((s-1)*sizeof(struct details)),0);
fwrite(&stu,sizeof(struct details),1,sfp);
fclose(sfp);
//fflush(ifp);
}	   //////////issuefile/////////////
/*
ifp=fopen("issuerec.dat","ab+");

if(ifp)
printf("issue file opened\n");

f=issuebk();
printf("F: %d",f);

ibk[f].isno=f+1;
strcpy(ibk[f].istuname,stu.sname);
strcpy(ibk[f].ibook,bnm.b_name);
strcpy(ibk[f].issdate,stu.bissued[stu.btaken].issue_date);
strcpy(ibk[f].retdate,stu.bissued[stu.btaken].return_date);
fseek(ifp,0,SEEK_END);

fwrite(&ibk[f],sizeof(struct issue),1,ifp);

if(fwrite(&ibk[f],sizeof(struct issue),1,ifp))
{
printf("WRITTEN\n");
}
printf("ISSUE DONE\n");
}  */

else
printf("No Book Found!Can't be Issued\n");
}
printf("Enter 1.Continue\n 0.Discontinue\n");
scanf("%d",&g);

}while(g!=0);
w=0;
s=0;
bkdetails(stu.srollno);
fclose(sfp);
//fclose(ifp);

break;
 }
 case 3:
 {
renew();
  printf("BOOK RENEWED!\n");
 break;
 }
 case 4:
 {
returnbk();
 printf("BOOK RETURNED!DETAILS UPDATED\n");
  break;
  }
case 5:
{
char id[20];
printf("Enter Students Id:\n");
fflush(stdin);
gets(id);
bkdetails(id);

break;
}
}
}while(ch!=6);//do inner
}

}//switch out

}while(c!=7);//do outer
fclose(sfp);
fclose(bfp);
//fclose(ifp);
//printf("books issued: %d",issuebk());
printf("\nPress ENTER to exit");
getch();

}//main
