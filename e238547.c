#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#define FILESIZE 50



struct graphHead{

    int count;
    struct graphVertex *first;

};

struct graphVertex{

    struct graphVertex *next;
    int data;
    int inDegree;
    int outDegree;
    int processed;
    struct graphArc *firstArc;
};

struct graphArc{

    struct graphVertex *destination;
    struct graphArc *nextArc;

};

struct areasOfInterest//this is the areas of interest array of struct
{
    char elementName[2];
    int topLeftX;
    int width;
    int topLeftY;
    int height;
};

struct eyeTracking//here, I am holfing people's scanpaths and unique IDs, then I used this uniqueID in creating vertices
{
    char scanpath[100];
    int uniqueID;

};


struct graphHead *createVertices(char[],char[]);
struct graphHead *createEdges(struct graphHead *head);
void insertVertex(struct graphHead *head,int);
int insertArc(struct graphHead *head,int,int);
void displayGraph(struct graphHead *head);







int main()
{



char AOIsFile[FILESIZE];
char scanpathsFile[FILESIZE];

printf("Enter AOIs file name:");//I am taking the name of the files
gets(AOIsFile);
printf("Enter scanpaths file name:");
gets(scanpathsFile);


struct graphHead *myGraph;
myGraph=createVertices(AOIsFile,scanpathsFile);




    return 0;
}





struct graphHead *createVertices(char AOIsFile[FILESIZE],char scanpathsFile[FILESIZE])
{
    struct graphHead *head;//here, I created head of the graph, then I assigned memory to it.
    head=(struct graphHead*)malloc(sizeof(struct graphHead));
    if(head==NULL)
        printf("Allocation failed!");
    head->count=0;
    head->first=NULL;

    FILE *aoiFile;
    aoiFile=fopen(AOIsFile,"r");
    if(aoiFile==NULL)
        printf("problem");

    char elementName[2];
    int topLeftX,width,topLeftY,height;
    int i,j;
    int lineNumofAOIs=0;
    char ch;

    while((ch = fgetc(aoiFile))!= EOF)//counting the lines of aois.txt file
    {
        if(ch == '\n')
            lineNumofAOIs++;
    }

    struct areasOfInterest *AOI;//creating array of struct and assign memory to it
    AOI=(struct areasOfInterest*)malloc(lineNumofAOIs*sizeof(struct areasOfInterest));

    fseek(aoiFile,0L,SEEK_SET);

    for(i=0;((fscanf(aoiFile,"%s %d %d %d %d\n",elementName,&topLeftX,&width,&topLeftY,&height)!=EOF));i++)//taking informations of areas of interests in the aois.txt file
    {
        //then I assigned those values to my array of struct, which is namely AOI
        strcpy(AOI[i].elementName,elementName);
        AOI[i].topLeftX=topLeftX;
        AOI[i].width=width;
        AOI[i].topLeftY=topLeftY;
        AOI[i].height=height;
    }

    fclose(aoiFile);

    FILE *fixationFile;//shortcut of scanPath
    fixationFile=fopen(scanpathsFile,"r");
    if(fixationFile==NULL)
        printf("allocation failed!");


    struct eyeTracking *person;
    person=(struct eyeTracking*)malloc(100*sizeof(struct eyeTracking));


    int fixationID,xCoordinate,Ycoordinate,duration;

    char sPath[100]="";
    int secondXcoordinate;//this coordinate equals to topleftX+width
    int secondYcoordinate;//this coordinate equals to topLeftY+height
    int start=0;

    int personCount=0;//this is number of person that joins the eye tracking test, in our case, this number is 8


    for(i=0;((fscanf(fixationFile,"%d %d %d %d\n",&fixationID,&xCoordinate,&Ycoordinate,&duration)!=EOF));i++)
    {

        if(fixationID==0)//when fixation ID is 0, I understood new person came
        {
            if(start!=0)
            {
                strcpy(person[personCount].scanpath,sPath);//I set the people's scanpaths
                person[personCount].uniqueID=personCount+1;//I set the unique Id for people
                personCount++;//I incremented the person count
                strcpy(sPath,"");
            }
            start=1;//here,for not to assign an empty path to first person,I ignore the first ID=0 condition

        }//unfortunately, I could not manage to achieve 8th person's data.



        for(j=0;j<lineNumofAOIs;j++)//in this loop, I found the scanpath of people
        {

            secondXcoordinate=AOI[j].topLeftX+AOI[j].width;
            secondYcoordinate=AOI[j].topLeftY+AOI[j].height;

            if(xCoordinate>AOI[j].topLeftX&&xCoordinate<secondXcoordinate&&Ycoordinate>AOI[j].topLeftY&&Ycoordinate<secondYcoordinate)
            {
                strcat(sPath,AOI[j].elementName);
                break;
            }
        }
    }

    for(i=0;i<personCount;i++)
    {
        insertVertex(head,person[i].uniqueID);//here,I inserted vertices based on the unique ID
    }

    return head;
}

struct graphHead *createEdges(struct graphHead *head)
{







    return head;//it returns the updated graph
}

void insertVertex(struct graphHead *head, int dataIn)
{
    struct graphVertex *vertex;
    vertex=(struct graphVertex*)malloc(sizeof(struct graphVertex));
    if(vertex==NULL)
        printf("Allocation failed!");

    vertex->next=NULL;
    vertex->data=dataIn;
    vertex->inDegree=0;
    vertex->outDegree=0;
    vertex->processed=0;
    vertex->firstArc=NULL;
    head->count++;

    if(head->first==NULL)
        head->first=vertex;
    else
    {
        struct graphVertex *tempVertex;
        tempVertex=head->first;

        while(tempVertex->next != NULL)
            tempVertex=tempVertex->next;
        tempVertex->next=vertex;
    }
}

int insertArc(struct graphHead *head, int fromKey, int toKey)
{
    struct graphArc *arc = (struct graphArc*)malloc(sizeof(struct graphArc));
    struct graphVertex *fromVertex =NULL;
    struct graphVertex *toVertex =NULL;

    fromVertex=head->first;
    while(fromVertex != NULL && fromVertex->data != fromKey)
        fromVertex=fromVertex->next;
    if(fromVertex==NULL)
        return -2;//from vertex is not found!

    toVertex=head->first;
    while(toVertex != NULL && toVertex->data != fromKey)
        toVertex=toVertex->next;
    if(toVertex==NULL)
        return -3;//toVertex is not found!

    fromVertex->outDegree++;
    toVertex->inDegree++;

    arc->destination = toVertex;
    arc->nextArc=NULL;

    if(fromVertex->firstArc == NULL)
        fromVertex->firstArc=arc;
    else
    {
        struct graphArc *tempArc = fromVertex->firstArc;
        while(tempArc->nextArc != NULL)
            tempArc=tempArc->nextArc;
        tempArc->nextArc=arc;
    }
    return 1;//successful operation

}












void displayGraph(struct graphHead *head)
{
    struct graphVertex *tempVertex = NULL;
    struct graphArc *tempArc = NULL;
    tempVertex=head->first;

    while(tempVertex != NULL)
    {
        printf(" Data : %d\n",tempVertex->data);
        tempArc=tempVertex->firstArc;
        while(tempArc != NULL)
            printf("Destination Data : %d\n",tempArc->destination->data);
            tempArc=tempArc->nextArc;
    }
    tempVertex=tempVertex->next;
}

























