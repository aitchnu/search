#include<iostream>
#include<fstream>
#include<string>
#include<malloc.h>
#include<map>
#include<cstdlib>
#include<vector>
#include<cstdio>
#include<algorithm>
#include <unordered_map>
#include"stemmer.h"
#include"myhash.h"

#define SW_SIZE 670

using namespace std;

struct node
{
	char docId[4];
	//int docId;
	bool title, link, category, infobox;
	unsigned short int freq;
};

unordered_map<string, vector<node> > myMap;

int charMap[]={1,2,3,4,5,6,7,8,9,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,
128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252};

string docArray;

vector<string> words;

void initialize()
{
}

int main(int argc, char *argv[])
{

    s = (char *) malloc(i_max + 1);
    initialize();
    unordered_map<string, vector<node> >::iterator it;
    vector<string>::iterator it2;
    Perfect_Hash *swCheck = new Perfect_Hash();
    node *mynode;
    char inFile[50];
    char outFile[50];
    char chBuffer[50];
    char ch;
    string buffer;
    string myword;
    strcpy(inFile, argv[1]);
    strcpy(outFile, argv[2]);
    std::ifstream fpIn(inFile, std::ios::in | std::ios::binary);
    if (fpIn == NULL)
    {
	cout << "File not found" << endl;
	return 0;
    }
    std::ofstream fpOut(outFile, std::ios::out | std::ios::binary);
    bool text = 0, title = 0, infobox = 0, link = 0, ignore = 0, id = 0,
	    category = 0;
    int pos = 0;
    int begin = 0;
    int end = 0;
    int token = 0;
    int docId;
    char chId[]={0,0,0,0};
    int t_pos;
    int t_end;
    int size = 0;
    int i=0;
    string t_buffer;
    int n=0;
    while (!fpIn.eof())
    {
//	if(n>100)
//	    break;
//	n++;
	begin = 0;
	getline(fpIn, buffer);
	while (buffer[begin] == ' ')
	    begin++;
	pos = begin;
	end = buffer.size();
	if (text == 1 && buffer.substr(begin, 2) == "</")
	{
	    text = 0;
	    infobox = 0;
	    title = 0;
	}
	else
	{
	    string tag = buffer.substr(begin, 4);
	    if (tag == "<tit")
	    {
		title = 1;
		pos = begin + 7;
		end = end - 8;
		t_pos = pos;
		t_end = end;
		t_buffer = buffer;
		pos = begin;
	    }
	    else if (title && tag == "<id>")
	    {
		pos = begin + 4;
		end = end - 5;
		id = 1;
		strcpy(chBuffer,buffer.substr(pos, end - pos).c_str());
		docArray+=chBuffer;
		docArray+="\n";
		docId = atoi(chBuffer);
		pos = t_pos;
		end = t_end;
		buffer = t_buffer;
		for(i=0;i<4;i++)
		{
		    if(chId[i]<=125)
		    {
			chId[i]++;
			if(chId[i]==10 || chId[i]==32)
			    chId[i]++;
			break;
		    }
		    else
			chId[i]=1;
		}
	    }
	    else if (tag == "<tex")
	    {
		text = 1;
		infobox = 1;
		pos = begin + 26;
	    }
	}
	if (text || pos != begin)
	{
	    if (infobox && buffer == "}}")
	    {
		infobox = 0;
	    }
	    token = pos;
	    for (i = pos; i <= end; i++)
	    {
		buffer[i] = tolower(buffer[i]);
		if (!link)
		{
		    if (buffer[i] == '[' && buffer[i + 1] == '[')
		    {
			link = 1;
			if (buffer.substr(i + 2, 9) == "Category:")
			{
			    category = 1;
			    i += 10;
			}
		    }
		}
		else if (buffer[i] == ']' && buffer[i + 1] == ']')
		{
		    link = 0;
		    category = 0;
		}
		if (!isalpha(buffer[i]))
		{
		    if (ignore && buffer[i] == ' ')
		    {
			ignore = 0;
			token = i + 1;
		    }
		    else if (!ignore)
		    {
			if (buffer[i] == '|' && !link)
			    ignore = 1;
			buffer[i] = '\0';
			if (isalpha(buffer[token]))
			{
			    myword = &buffer[token];
			    if (!swCheck->in_word_set(myword.c_str(),
				    myword.size()))
			    {
				stemfile(myword);
				myword = s;
				it = myMap.find(myword);
				if (it == myMap.end())
				{
				    words.push_back(myword);
				    myMap[myword].push_back(node());
				    size = 0;
				}
				else
				{
				    size = it->second.size() - 1;
				    if (strcmp(it->second[size].docId,chId)!=0)
				    //if(it->second[size].docId!=docId)
				    {
					it->second.push_back(node());
					size++;
				    }
				}
				mynode = &myMap[myword][size];
				strcpy(mynode->docId,chId);
				//mynode->docId=docId;
				mynode->title |= title;
				mynode->category |= category;
				mynode->link |= link;
				mynode->infobox |= infobox;
				++mynode->freq;
			    }
			}
			token = i + 1;
		    }
		}
	    }
	    if (title == id)
	    {
		title = 0;
		id = 0;
	    }
	}
    }
    sort(words.begin(), words.end());
    for (it2 = words.begin(); it2 != words.end(); it2++)
    {
	buffer.clear();
	buffer += *it2 + " ";
	it = myMap.find(*it2);
	int x;
	for (int i = 0; i < it->second.size(); i++)
	{
	    buffer+=it->second[i].docId;
	    buffer+=" ";
	    sprintf(chBuffer,"%d",it->second[i].freq);
	    buffer+=chBuffer;
	    buffer+=" ";
	    if ((it->second[i].title || it->second[i].infobox
		    || it->second[i].link || it->second[i].category))
	    {
		ch=((int)it->second[i].title + 2 * (int)it->second[i].infobox + 4 * (int)it->second[i].link + 8 * (int)it->second[i].category);
		buffer+=ch;
	    }
	    buffer+=" ";
	}
	buffer+="\n";
	fpOut.write(buffer.c_str(),buffer.size());
    }
    fpIn.close();
    fpOut.close();
    return 0;
}
