#include <stdio.h>
#define MAXLINELENGTH 255
#include "..\struct.h"

int check_headerfile(char *filename);
int hix_headerfile(char *filename);

/********************************************************************/
/********************************************************************/
int main( int argc, char **argv )
{
  if(argc!=2)
  {
    printf("ERROR: usage \"chkgrp <newsgroupheaderfile>\"\n");
    printf("             \"chkgrp <newmessagefile>\"\n");
    return -1;
  }
  if((!strcmp(strrchr(argv[1],'.'),".INH")) || (!strcmp(strrchr(argv[1],'.'),".inh")))
    return check_headerfile(argv[1]);
  else if((!strcmp(strrchr(argv[1],'.'),".INM")) || (!strcmp(strrchr(argv[1],'.'),".inm")))
    return fix_headerfile(argv[1]);
  
}

/********************************************************************/
/********************************************************************/
int check_headerfile(char *filename)
{
  FILE *headerfile;
  NewsHeader nh;
  char tecken;
  char buffer[9];
  int counter;
  headerfile=fopen(filename,"rb");
  if(!headerfile)
  {
    printf("ERROR: Could not open file %s\n",filename);
    return -2;
  }
  if(fread(buffer,8,1,headerfile)!=1)
  {
    printf("ERROR: Incorrect fileformat\n");
    return -3;
  }
  buffer[8]='\0';
  if(strcmp(buffer,"NEWS0.50"))
  {
    printf("ERROR: Incorrect fileformat\n");
    return -4;
  }
  counter=0;
  while(fread(&nh,sizeof(NewsHeader),1,headerfile)==1)
  {
    printf("msg_num :%d\n",counter);
    printf("Subject : %s\n",nh.subject);
//    printf("From    : %s\n",nh.from);
    printf("offset  : %ld\n",nh.offset);
    printf("length  : %ld\n",nh.length);
//    printf("date=%4d/%02d/%02d %02d:%02d\n",nh.datetime.year,nh.datetime.month,nh.datetime.day,nh.datetime.hour,nh.datetime.min);
    printf("links  : Parent:%d  Child:%d  Next:%d  Previous:%d\n",nh.i.parent,nh.i.child,nh.i.next,nh.i.prev);
    tecken=getch();
    if (tecken==27)
      break;
    else if (tecken=='+')
      counter+=10;
    else if (tecken=='-')
      counter-=10;
    else if (tecken=='*')
      counter+=100;
    else if (tecken=='/')
      counter-=100;
    else
      counter++;
    if(counter<0) counter=0;
    fseek(headerfile,counter*sizeof(NewsHeader)+8,SEEK_SET);
  }
  fclose(headerfile);
  return 0;
}

/********************************************************************/
/********************************************************************/
int fix_headerfile(char *filename)
{
  FILE *messagefile,*headerfile;
  NewsHeader nh;
  char buffer[1024],tempbuffer[1024];
  long temp1,temp2,num_of_messages;

  printf("Recreating Headerfile (from scratch)\n(Each dot represents 1 message)\n");
  messagefile=fopen(filename,"r");
  if(!messagefile)
  {
    printf("ERROR: Could not open file %s\n",filename);
    return -2;
  }
  filename[strlen(filename)-1]='h';
  headerfile=fopen(filename,"wb");
  if(!headerfile)
  {
    printf("ERROR: Could not open file %s\n",filename);
    fclose(messagefile);
    return -2;
  }
  fwrite( NEWSID , sizeof( long ), 1, headerfile ) ;
  fwrite( NEWSVER , sizeof( long ), 1, headerfile ) ;
  fseek(messagefile,0,SEEK_SET);
  rewind(messagefile);
  nh.offset=0;
  memset(&nh,0,sizeof(NewsHeader));
  
  num_of_messages=0;
  while(fgets(buffer,1023,messagefile))
  {
    if(num_of_messages==1040)
    {
      printf("%s",buffer);
      printf("%ld\n",ftell(messagefile));
    }
    if(strrchr(buffer,'\n'))
      ((char *)strrchr(buffer,'\n'))[0]='\0';
    if(strrchr(buffer,'\r'))
      ((char *)strrchr(buffer,'\r'))[0]='\0';
    if(!strcmp(buffer,"."))
    {
      nh.length=ftell(messagefile)-nh.offset;
      nh.i.child=FAIL;
      nh.i.parent=FAIL;
      nh.i.next=FAIL;
      nh.i.prev=FAIL;
      if(num_of_messages==1040)
      {
        printf("Subject : %s\n",nh.subject);
        printf("From    : %s\n",nh.from);
        printf("offset  : %ld\n",nh.offset);
        printf("length  : %ld\n",nh.length);
        printf("date=%4d/%02d/%02d %02d:%02d\n",nh.datetime.year,nh.datetime.month,nh.datetime.day,nh.datetime.hour,nh.datetime.min);
        printf("links  : Parent:%d  Child:%d  Next:%d  Previous:%d\n",nh.i.parent,nh.i.child,nh.i.next,nh.i.prev);
        printf("%ld\n",ftell(messagefile));
        getch();
      }
      fwrite(&nh,sizeof(NewsHeader),1,headerfile);
      printf("\33l%ld %ld",num_of_messages+1,nh.offset);
      memset(&nh,0,sizeof(NewsHeader));
      nh.offset=ftell(messagefile);
      num_of_messages++;
      

    }
    else if( !strncmp( "From: ", buffer, 6 ) )
    {
      if(strchr(buffer,'('))
      {
        temp1=strcspn(buffer,"(");
        temp1++;
        while(isspace(buffer[temp1])) temp1++;
        strncpy(tempbuffer,buffer+temp1,1023);
        tempbuffer[1023]='\0';
        temp2=strcspn(tempbuffer,")");
        tempbuffer[temp2]='\0';
      }
      else if(strchr(buffer,'<'))
      {
        temp1=strcspn(buffer," ");
        strncpy(tempbuffer,buffer+temp1+1,1023);
        tempbuffer[1023]='\0';
        temp2=strcspn(tempbuffer,"<");
        tempbuffer[temp2]='\0';
      }
      if(!tempbuffer[0])
      {
        temp1=strcspn(buffer," ");
        strncpy(tempbuffer,buffer+temp1+1,1023);
        tempbuffer[1023]='\0';
        temp2=strcspn(tempbuffer," ");
        tempbuffer[temp2]='\0';
      }
      strncpy( nh.from , tempbuffer , NEWS_FROMSIZE - 1 ) ;
      nh.from[ NEWS_FROMSIZE - 1 ] = '\0' ;
    }
    else if( !strncmp( "Subject: " , buffer, 9 ) )
    {
      strncpy(nh.subject,buffer+9,NEWS_SUBJECTSIZE-1);
      nh.subject[NEWS_SUBJECTSIZE-1]='\0';
    }
    else if( !strncmp( "Date: " , buffer, 6 ) )
    {
      int day=0,year=0,hour=0,min=0,sec=0;
      char month[4];
      month[3]=0;
      temp1=0;
      while(!isdigit(buffer[temp1])) temp1++;
      sscanf(buffer+temp1,"%d %c%c%c %d %d:%d:%d",&day,&month[0],&month[1],&month[2],&year,&hour,&min,&sec);
      month[0]=toupper(month[0]);
      month[1]=toupper(month[1]);
      month[2]=toupper(month[2]);
      if(year<50)
        nh.datetime.year=year+20;
      else if(year<100)
        nh.datetime.year=year-80;
      else
        nh.datetime.year=year-1980;
      nh.datetime.day=day;
      nh.datetime.hour=hour;
      nh.datetime.min=min;
      nh.datetime.sec=sec;
      if(!strcmp(month,"JAN"))
        nh.datetime.month=0;
      else if(!strcmp(month,"FEB"))
        nh.datetime.month=1;
      else if(!strcmp(month,"MAR"))
        nh.datetime.month=2;
      else if(!strcmp(month,"APR"))
        nh.datetime.month=3;
      else if(!strcmp(month,"MAY"))
        nh.datetime.month=4;
      else if(!strcmp(month,"JUN"))
        nh.datetime.month=5;
      else if(!strcmp(month,"JUL"))
        nh.datetime.month=6;
      else if(!strcmp(month,"AUG"))
        nh.datetime.month=7;
      else if(!strcmp(month,"OCT"))
        nh.datetime.month=8;
      else if(!strcmp(month,"SEP"))
        nh.datetime.month=9;
      else if(!strcmp(month,"NOV"))
        nh.datetime.month=10;
      else if(!strcmp(month,"DEC"))
        nh.datetime.month=11;
    }
  }
  printf("\n");
  printf("Recreated %d Headers\n",num_of_messages);
  fclose(headerfile);
  fclose(messagefile);
  return 0;
}
