#include<iostream>
#include<string.h>
#include<fstream>
#include<dirent.h>

using namespace std;

struct node
{
	int docId;
	bool title,link,category,infobox;
	int freq;
};

int main()
{
    DIR *pDIR;
    struct dirent *entry;
    std::ifstream fpIn[500];
    int count=0;
    map<string, pair<ifstream,node> >mymap;
    string buffer;
    if (pDIR = opendir("./"))
    {
	while (entry = readdir(pDIR))
	{
	    if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && entry->d_name[0]=='i' && entry->d_name[1]=='d'&& entry->d_name[2]=='x')
	    {
		cout << entry->d_name << "\n";
		fpIn[count++].open(entry->d_name,ifstream::in | std::ios::binary);
	    }
	}
	closedir(pDIR);
    }

    for(int i=0;i<count;i++)
    {
	while(!fpIn[i].eof())
	{
	    getline(fpIn[i], buffer);
	}
    }
    return 0;
}
