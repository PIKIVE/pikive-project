#include <stdio.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <set>
#include <iterator>
#include <omp.h>
#include <time.h>

using namespace std;

#define MAX 1000
#define hh printf("Hello\n");
#define NL printf("\n");

int array[99999];

struct Candidate
{
    vector< set <int> > csets;
};
struct Frequent
{
    vector< set <int> > fsets;
    bool isNull;
};

Candidate C[MAX];
Frequent L[MAX];

vector < set <int> > Database;

int min_sup;

Frequent findFrequentItemset(Candidate cd)
{
    int i,j;
    Frequent fr;
    fr.isNull = true; 
	
	#pragma omp parallel for
    for(i = 0; i< cd.csets.size(); i++)
    {
        set < int > cdItem = cd.csets[i];
        int support = 0;
		#pragma omp parallel for
        for(j = 0; j< Database.size(); j++)
        {
            set < int > dataItem = Database[j];
            vector < int > intersecSet;
            set_intersection(cdItem.begin(), cdItem.end(), dataItem.begin(), dataItem.end(), back_inserter(intersecSet));
            if(intersecSet.size() == cdItem.size())
                support+= 1;
            intersecSet.clear();
            dataItem.clear();
		}
		#pragma omp critical
        if(support >= min_sup)
        {
            fr.fsets.push_back(cdItem);
            fr.isNull = false;
            printf("{");
            set<int> :: iterator it;
            for(it = cdItem.begin(); it != cdItem.end();it++)
            {
                    printf("%d ", *it);
			}
            printf("} ->; %d\n", support);
		}
        cdItem.clear();
	}
    return fr;
}

bool checkJoinAbility( set <int> a, set <int> b)
{
    set<int>::iterator it(a.begin());
    std::advance(it,b.size()-1);
    if(std::equal(a.begin(),it,b.begin()))
        return true;
    return false;
}

Candidate apriori_gen(Frequent fr)
{
    int i,j;
    Candidate cd;
    vector < set <int> > l1 = fr.fsets;
    vector < set <int> > l2 = fr.fsets;
//	#pragma omp parallel for
    for(i = 0; i< l1.size(); i++)
    {
//		#pragma omp parallel for
        for(j = i+1; j < l2.size(); j++)
        {
            set < int > sj;
            if(l1[i].size() == 1)
            {
                set<int>::iterator it;
                it = l1[i].begin();
                sj.insert(*it);
                it = l2[j].begin();
                sj.insert(*it);
                cd.csets.push_back(sj);
			}
            else
            {
                if(checkJoinAbility(l1[i], l2[j]))
                {
                    set<int>::iterator it;
                    for(set<int> :: iterator it = l1[i].begin(); it != l1[i].end(); ++it)
                    {
                        sj.insert(*it);
					}
                    for(set<int> :: iterator it = l2[j].begin(); it != l2[j].end(); ++it)
                    {
                        sj.insert(*it);
                    }
                    cd.csets.push_back(sj);
				}
			}
		}
	}
	NL
	return cd;
}

void apriori()
{
	L[0] = findFrequentItemset(C[0]);
	int k;
	for(k = 1; L[k-1].isNull != true; k++)
	{
		C[k] = apriori_gen(L[k-1]);
		L[k] = findFrequentItemset(C[k]);
	}
}

int main(int argc, char **argv)
{
	FILE *fp;
	char buf[1000];
	clock_t t;

	if(argc < 4){
		printf("error! :: program input.dat minsup #ofthreads\n");
		return 0;
	}
//	printf("Enter minimum support count: ");
//	scanf("%d",&min_sup);
	fp = fopen(argv[1], "r");
	min_sup = atoi(argv[2]);
	omp_set_num_threads(atoi(argv[3]));

	while(fgets(buf, sizeof(buf), fp))
	{
		set < int > db;
		db.clear();
		char *ch;
		ch = strtok(buf," \n");
		while(ch != NULL)
		{
			int num = atoi(ch);
			db.insert(num);
			if(array[num] == 0)
			{
				set < int > a;
				a.clear();
				a.insert(num);
				C[0].csets.push_back(a);
				array[num] += 1;
			}
			else array[num] += 1;
			ch = strtok(NULL," \n");
		}
		sort(C[0].csets.begin(),C[0].csets.end());
		Database.push_back(db);
	}
	fclose(fp);
	t=clock();
	apriori();
	t=clock()-t;
	double time_taken = ((double)t)/CLOCKS_PER_SEC;
	printf("apriori() took %.2f seconds to execute \n",time_taken);
	return 0;
}
