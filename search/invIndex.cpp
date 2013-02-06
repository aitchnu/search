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

string docArray;
string dest;
vector<string> words;

void initialize()
{
}

void writeDocFile()
{
    dest+="docList";
    std::ofstream fp(dest, std::ios::app | std::ios::binary);
    fp.write(docArray.c_str(),docArray.size());
    fp.close();
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
    dest=argv[2];
    strcpy(inFile, argv[1]);
    if(strcmp(inFile,"invIndex")==0)
	return 0;
    strcpy(outFile, argv[2]);
    strcat(outFile,"idx_");
    strcat(outFile,inFile);
    std::ifstream fpIn(inFile, std::ios::in | std::ios::binary);
    if (fpIn == NULL)
    {
	cout <<inFile <<" : File not found" << endl;
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
	fpOut.write(buffer.c_str(),buffer.size());
	buffer.clear();
	for (int i = 0; i < it->second.size(); i++)
	{
	    buffer+=it->second[i].docId;
	    fpOut.write(buffer.c_str(),buffer.size());
	    buffer.clear();
	    fpOut.write((char *)&it->second[i].freq, sizeof(it->second[i].freq));
	    if ((it->second[i].title || it->second[i].infobox
		    || it->second[i].link || it->second[i].category))
	    {
		ch=((int)it->second[i].title + 2 * (int)it->second[i].infobox + 4 * (int)it->second[i].link + 8 * (int)it->second[i].category);
		buffer+=ch;
	    }
	    else
		buffer+='0';
	    buffer+=" ";
	}
	buffer+="\n";
	fpOut.write(buffer.c_str(),buffer.size());
    }
    writeDocFile();
    fpIn.close();
    fpOut.close();
    cout<<inFile<<" : index stored at : "<<outFile<<endl;
    return 0;
}
