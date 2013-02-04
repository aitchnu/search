#include<iostream>
#include<fstream>
#include<string>
#include<malloc.h>
#include<map>
#include<cstdlib>
#include"stemmer.h"
#include<vector>

#define SW_SIZE 670

using namespace std;

string stopwords[]={"list","gt","lt","a","able","about","above","abst","accordance","according","accordingly","across","act","actually","added","adj","affected","affecting","affects","after","afterwards","again","against","ah","all","almost","alone","along","already","also","although","always","am","among","amongst","an","and","announce","another","any","anybody","anyhow","anymore","anyone","anything","anyway","anyways","anywhere","apparently","approximately","are","aren","arent","arise","around","as","aside","ask","asking","at","auth","available","away","awfully","b","back","be","became","because","become","becomes","becoming","been","before","beforehand","begin","beginning","beginnings","begins","behind","being","believe","below","beside","besides","between","beyond","biol","both","brief","briefly","but","by","c","ca","came","can","cannot","can't","cause","causes","certain","certainly","co","com","come","comes","contain","containing","contains","could","couldnt","d","date","did","didn't","different","do","does","doesn't","doing","done","don't","down","downwards","due","during","e","each","ed","edu","effect","eg","eight","eighty","either","else","elsewhere","end","ending","enough","especially","et","et-al","etc","even","ever","every","everybody","everyone","everything","everywhere","ex","except","f","far","few","ff","fifth","first","five","fix","followed","following","follows","for","former","formerly","forth","found","four","from","further","furthermore","g","gave","get","gets","getting","give","given","gives","giving","go","goes","gone","got","gotten","h","had","happens","hardly","has","hasn't","have","haven't","having","he","hed","hence","her","here","hereafter","hereby","herein","heres","hereupon","hers","herself","hes","hi","hid","him","himself","his","hither","home","how","howbeit","however","hundred","i","id","ie","if","i'll","im","immediate","immediately","importance","important","in","inc","indeed","index","information","instead","into","invention","inward","is","isn't","it","itd","it'll","its","itself","i've","j","just","k","keep","keeps","kept","kg","km","know","known","knows","l","largely","last","lately","later","latter","latterly","least","less","lest","let","lets","like","liked","likely","line","little","'ll","look","looking","looks","ltd","m","made","mainly","make","makes","many","may","maybe","me","mean","means","meantime","meanwhile","merely","mg","might","million","miss","ml","more","moreover","most","mostly","mr","mrs","much","mug","must","my","myself","n","na","name","namely","nay","nd","near","nearly","necessarily","necessary","need","needs","neither","never","nevertheless","new","next","nine","ninety","no","nobody","non","none","nonetheless","noone","nor","normally","nos","not","noted","nothing","now","nowhere","o","obtain","obtained","obviously","of","off","often","oh","ok","okay","old","omitted","on","once","one","ones","only","onto","or","ord","other","others","otherwise","ought","our","ours","ourselves","out","outside","over","overall","owing","own","p","page","pages","part","particular","particularly","past","per","perhaps","placed","please","plus","poorly","possible","possibly","potentially","pp","predominantly","present","previously","primarily","probably","promptly","proud","provides","put","q","que","quickly","quite","qv","r","ran","rather","rd","re","readily","really","recent","recently","ref","refs","regarding","regardless","regards","related","relatively","research","respectively","resulted","resulting","results","right","run","s","said","same","saw","say","saying","says","sec","section","see","seeing","seem","seemed","seeming","seems","seen","self","selves","sent","seven","several","shall","she","shed","she'll","shes","should","shouldn't","show","showed","shown","showns","shows","significant","significantly","similar","similarly","since","six","slightly","so","some","somebody","somehow","someone","somethan","something","sometime","sometimes","somewhat","somewhere","soon","sorry","specifically","specified","specify","specifying","still","stop","strongly","sub","substantially","successfully","such","sufficiently","suggest","sup","sure","t","take","taken","taking","tell","tends","th","than","thank","thanks","thanx","that","that'll","thats","that've","the","their","theirs","them","themselves","then","thence","there","thereafter","thereby","thered","therefore","therein","there'll","thereof","therere","theres","thereto","thereupon","there've","these","they","theyd","they'll","theyre","they've","think","this","those","thou","though","thoughh","thousand","throug","through","throughout","thru","thus","til","tip","to","together","too","took","toward","towards","tried","tries","truly","try","trying","ts","twice","two","u","un","under","unfortunately","unless","unlike","unlikely","until","unto","up","upon","ups","us","use","used","useful","usefully","usefulness","uses","using","usually","v","value","various","'ve","very","via","viz","vol","vols","vs","w","want","wants","was","wasn't","way","we","wed","welcome","we'll","went","were","weren't","we've","what","whatever","what'll","whats","when","whence","whenever","where","whereafter","whereas","whereby","wherein","wheres","whereupon","wherever","whether","which","while","whim","whither","who","whod","whoever","whole","who'll","whom","whomever","whos","whose","why","widely","willing","wish","with","within","without","won't","words","world","would","wouldn't","www","x","y","yes","yet","you","youd","you'll","your","youre","yours","yourself","yourselves","you've","z","zero"};

struct node
{
	int docId;
	bool title,link,category,infobox;
	int freq;
};

map<string,int> swMap;
map<string, vector< node > > myMap;

void initialize()
{
    int size=SW_SIZE;
    for(int i=0;i<size;i++)
    {
	swMap[stopwords[i]]=1;
    }
}

int main(int argc,char *argv[])
{

    s = (char *) malloc(i_max+1);
    initialize();
    char inFile[50];
    char outFile[50];
    string buffer;
    string myword;
    strcpy(inFile,argv[1]);
    strcpy(outFile,argv[2]);
    std::ifstream fpIn(inFile, std::ios::in | std::ios::binary);
    if(fpIn==NULL)
    {
	    cout<<"File not found"<<endl;
	    return 0;
    }
    std::ofstream fpOut(outFile, std::ios::app | std::ios::binary);
//    fpIn.seekg(0, std::ios::end);
//    buffer.resize(fpIn.tellg());
//    fpIn.seekg(0, std::ios::beg);
//    fpIn.read(&buffer[0],buffer.size());
    bool text = 0, title = 0, infobox = 0, link = 0, ignore = 0, id=0, category=0;
    int pos = 0;
    int begin = 0;
    int end = 0;
    int token = 0;
    int n = 0;
    int docId;
    int t_pos;
    int t_end;
    string t_buffer;
    map<string, vector<node> >::iterator it;
    while (!fpIn.eof())
    {
//	n++;
//	if (n > 100)
//	    break;
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
	    title=0;
	}
	else
	{
	    string tag = buffer.substr(begin, 4);
	    if (tag == "<tit")
	    {
		title = 1;
		pos = begin + 7;
		end = end - 8;
		t_pos=pos;
		t_end=end;
		t_buffer=buffer;
		pos=begin;
	    }
	    else if (title && tag == "<id>")
	    {
		pos = begin + 4;
		end = end - 5;
		id=1;
		docId=atoi(buffer.substr(pos,end-pos).c_str());
		pos=t_pos;
		end=t_end;
		buffer=t_buffer;
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
	    for (int i = pos; i <= end; i++)
	    {
		buffer[i]=tolower(buffer[i]);
		if (!link)
		{
		    if (buffer[i] == '[' && buffer[i + 1] == '[')
		    {
			link = 1;
			if(buffer.substr(i+2, 9)=="Category:")
			{
			    category=1;
			    i+=10;
			}
		    }
		}
		else if (buffer[i] == ']' && buffer[i + 1] == ']')
		{
		    link = 0;
		    category=0;
		}
		if (!isalpha(buffer[i]))
		{
		    if (ignore && buffer[i] == ' ')
		    {
			ignore = 0;
			token = i + 1;
		    }
		    else if(!ignore)
		    {
			if (buffer[i] == '|' && !link)
			    ignore = 1;
			buffer[i] = '\0';
			if (isalpha(buffer[token]))
			{
			    myword=&buffer[token];
			    if(swMap.find(myword)==swMap.end())
			    {
				stemfile(myword);
				myword=s;
				int size=myMap[myword].size()-1;
				//cout<<myword<<" "<<title<<infobox<<link<<category<<" "<<docId<<endl;
				if(myMap[myword].size()==0 || myMap[myword][size].docId != docId )
				{
					myMap[myword].push_back(node());
					size++;
				}
				myMap[myword][size].docId=docId;
				myMap[myword][size].title+=title;
				myMap[myword][size].category+=category;
				myMap[myword][size].link+=link;
				myMap[myword][size].infobox+=infobox;
				myMap[myword][size].freq++;
			    }
			}
			token = i + 1;
		    }
		}
	    }
	    if(title==id)
	    {
		title=0;
		id=0;
	    }
	    //	    char *temp = buffer.c_str();
	}
    }
    for (it = myMap.begin(); it != myMap.end(); it++)
    {
	//fpOut.write(it->first.c_str(),sizeof it->first);
	fpOut<<it->first<<" ";
	for (int i = 0; i < it->second.size() && i<5000 ; i++)
	{
	    fpOut << it->second[i].docId<<","<<it->second[i].freq<<",";
	    if((it->second[i].title || it->second[i].infobox || it->second[i].link || it->second[i].category))
	    {
//		fpOut.write((char *)it->second[i].title,1);
//		fpOut.write((char *)it->second[i].infobox,1);
//		fpOut.write((char *)it->second[i].link,1);
//		fpOut.write((char *)it->second[i].category,1);
		fpOut<<it->second[i].title<<it->second[i].infobox<<it->second[i].link<<it->second[i].category;
	    }
	    fpOut<<",";
	}
//	fpOut.write("\n",sizeof "\n");
	fpOut<<endl;
    }
    fpIn.close();
    return 0;
}
