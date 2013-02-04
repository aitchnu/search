#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<vector>
#include<map>
#include<cstring>
#include<fstream>

using namespace std;

struct node
{
	int docId;
	vector<int> pos;
};

map<string, vector< node > > myMap;

bool read_file(FILE **fp, char *filename)
{
	*fp = fopen(filename, "r");
	if (*fp == NULL)
	{
		perror("Error while opening the file.\n");
		return 0;
	}
	return 1;
}

bool write_file(FILE **fp, char *filename)
{
	*fp = fopen(filename, "w");
	if (*fp == NULL)
	{
		perror("Error while opening the file.\n");
		return 0;
	}
	return 1;
}

bool make_index(FILE *fpIn, FILE *fpOut)
{
	char myword[100];
	int docId=0,pos=0;
	while(fscanf(fpIn,"%s",myword)!=EOF)
	{
//		if(strcmp(myword,"(doc)")==0)
//		{
//			docId++;
//			pos=0;
//			continue;
//		}
		int size=myMap[myword].size()-1;
		if(myMap[myword].size()==0 || myMap[myword][size-1].docId < docId )
		{
			myMap[myword].push_back(node());
			size++;
		}
		myMap[myword][size].pos.push_back(pos);
		myMap[myword][size].docId=docId;
		pos++;
	}
	return 1;
}

bool print_map(char *outFile)
{
	map<string, vector<node> >::iterator it;
	fstream fpOut;
	fpOut.open(outFile,fstream::out);
	for(it=myMap.begin();it!=myMap.end();it++)
	{
		fpOut<<it->first<<" ";
		for(int i=0;i<it->second.size();i++)
		{
			fpOut<<it->second[i].docId<<" ( ";
			for(int j=0;j<it->second[i].pos.size();j++)
			{
				fpOut<<it->second[i].pos[j]<<" ";
			}
			fpOut<<") ";
		}
		fpOut<<endl;
	}
	return 1;
}

int main() {
	char inFile[50], outFile[50];
	FILE *fpIn=NULL, *fpOut=NULL;
	cout << "Enter file name : ";
	cin >> inFile;
	strcpy(outFile,inFile);
	strcat(outFile,"_idx");
	read_file(&fpIn, inFile);
	write_file(&fpOut,outFile);
	make_index(fpIn, fpOut);
//	print_map(outFile);
	cout<<"Program terminated successfully"<<endl;
	return 0;
}
