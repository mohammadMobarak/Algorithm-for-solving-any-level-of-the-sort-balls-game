

#include <iostream>
#include <map>
#include <vector>
#define ll long long
using namespace std;

ll num = 10000000000;
vector<pair<int, int>>answer; // holds the tubes 
map<vector<vector<char>>, bool>mp; // to keep an eye of the collection of tubes that want to be analysed


bool stable_tube(vector<char>v)  // checks if the tube has only one color 
{
    bool ok = 1;
    char c = v[0];
    for (int i = 1; i < v.size(); i++)
    {
        if (v[i] != c)
        {
            ok = 0;
            break;
        }
    }
    return ok;
}
int better_tube(vector<vector<char>>v, char c, int should_be_avoided) //chekcs if there is a stable tube to put the ball in 
{
    int temp = -1;
    for (int i = 0; i < v.size(); i++)
    {
        if (i != should_be_avoided && v[i].size() > 0 && stable_tube(v[i]) && c == v[i][0])
        {
            temp = i;
        }
    }
    return temp;
}
bool target(vector<char>v) // checks if the tube has three balls of the same color 
{
    bool ok = 0;
    if (v.size() == 3)
    {
        char c = v[0];
        if (c == v[1] && c == v[2])
        {
            ok = 1;
        }
    }
    return ok;
}

bool done(vector<char>v) //checks if the tube is full of the same color
{
    bool ok = 0;
    if (v.size() == 4)
    {
        char c = v[0];
        if (c == v[1] && c == v[2] && c == v[3])
        {
            ok = 1;
        }
    }
    return ok;
}
void describe(vector<vector<char>>v) // shows the contents of tubes i was using this in debugging it's very useful :)
{
    for (int i = 0; i < v.size(); i++)
    {
        cout << "vector number " << i + 1 << " has : ";
        for (int j = 0; j < v[i].size(); j++)
        {
            cout << v[i][j] << " ";
        }
        cout << endl;
    }
}
bool check_perfectionism(vector<vector<char>>v) // checks if we finished sorting or not 
{
    bool ok = 1;
    for (int i = 0; i < v.size(); i++)
    {
        if (ok == 0)
        {
            break;
        }
        vector<char>z = v[i];
        if (z.size() == 0)
        {
            continue;
        }
        char c = z[0];
        for (int j = 0; j < z.size(); j++)
        {
            if (z[j] != c)
            {
                ok = 0;
                break;
            }
        }
        if (z.size() > 0 && z.size() < 4)
        {
            ok = 0;
        }
    }
    return ok;
}
int solve(vector<vector<char>>v, pair<int, int>banned_move, vector<pair<int, int>>current) //the recursive function
{
    if (mp.size() > 35000 || mp[v] == true) //terminate the process after searching for 7 seconds for the best solution and also terminates it if we analysed this current collection before 
    {
        return 0;
    }
    mp[v] = true;

    if (check_perfectionism(v)) //if we reached a better solution store the better solution
    {
        if (current.size() < num)
        {
            num = current.size();
            answer = current;
        }
    }

    int trgt = -1;
    char trgt_char;
    for (int i = 0; i < v.size(); i++) //  search if there is a tube messing one ball we store it's indes and color
    {
        if (target(v[i]))
        {
            trgt = i;
            trgt_char = v[i][0];
        }
    }
    if (trgt != -1)  //if there is a tube messing one then we search if there is a ball that could fill it 
    {
        for (int i = 0; i < v.size(); i++)
        {
            if (i != trgt)
            {
                int x = v[i].size();
                x -= 1;
                if (x > -1 && v[i][x] == trgt_char)
                {
                    vector<vector<char>>vv = v;
                    vv[i].erase(vv[i].begin() + x);
                    vv[trgt].push_back(trgt_char);
                    vector<pair<int, int>>currentnow = current;
                    currentnow.push_back({ i + 1, trgt + 1 });
                    solve(vv, { trgt, i }, currentnow);
                    return 0;
                }
            }
        }
    }
    for (int i = 0; i < v.size(); i++) //the loops of the searching
    {
        for (int j = 0; j < v.size(); j++)
        {
            if (j != i && v[j].size() > 0)
            {
                if ((j != banned_move.first || i != banned_move.second)) //cheks if the current trasion isn't the opposite of the previous one
                {
                    int syze = v[j].size(), syze2 = v[i].size();
                    char c = v[j][syze - 1];
                    if (done(v[j]) == 0 && (syze2 == 0 || (c == v[i][syze2 - 1] && syze2 < 4)))//checks if the current transion is possible
                    {
                        if (stable_tube(v[j]) && syze2 == 0)//prevents a tube with only one color from giving a ball to an empty tube
                        {
                            continue;
                        }
                        int index = -1;
                        index = better_tube(v, c, j);


                        if (index != -1) // if there is a stable tube to put the current ball in 
                        {
                            vector<vector<char>>v2 = v;
                            v2[j].erase(v2[j].begin() + (syze - 1));
                            v2[index].push_back(c);
                            vector<pair<int, int>>current2 = current;
                            current2.push_back({ j + 1, index + 1 });
                            solve(v2, { index,j }, current2);
                            return 0;
                        }

                        if (syze2 > 0 && stable_tube(v[i]) && stable_tube(v[j]) && syze > syze2)// if there is two stabel tubes wehere the bigger size one gives the ball to the smaller we swap the transiotion
                        {
                            vector<vector<char>>v2 = v;
                            v2[i].erase(v2[i].begin() + (syze2 - 1));
                            v2[j].push_back(c);
                            vector<pair<int, int>>current2 = current;
                            current2.push_back({ i + 1, j + 1 });
                            solve(v2, { j,i }, current2);
                            return 3;
                        }

                        if (stable_tube(v[j]) && stable_tube(v[i]) == 0 && v[i].size() + v[j].size() > 4)//prevent an unnessesary transition
                        {
                            continue;
                        }
                        vector<vector<char>>v2 = v;
                        v2[j].erase(v2[j].begin() + (syze - 1));
                        v2[i].push_back(c);
                        vector<pair<int, int>>current2 = current;
                        current2.push_back({ j + 1, i + 1 });
                        solve(v2, { i,j }, current2);
                    }
                }
            }
        }
    }

    return 0;
}


int main()
{

    vector<vector<char>>vec;
    cout << "enter the number of full tubes" << endl;
    int fulled;
    cin >> fulled;
    for (int i = 0; i < fulled; i++)
    {
        vector<char>v1;
        cout << "enter the contents of the tube number " << i + 1 << endl;
        string s1 = "";
        cin >> s1;
        for (int j = 0; j < s1.size(); j++)
        {
            v1.push_back(s1[j]);
        }
        vec.push_back(v1);
    }
    cout << "enter the number of empty tubes " << endl;
    int empty;
    cin >> empty;
    for (int i = 0; i < empty; i++)
    {
        vector<char>v3;
        vec.push_back(v3);
    }

    vector<pair<int, int>>current;
    solve(vec, { 0,0 }, current);
    cout << "answer has :" << answer.size() << " steps which are : " << endl;;
    for (int i = 0; i < answer.size(); i++)
    {
        cout << answer[i].first << " -> " << answer[i].second << endl;
    }
    cout << "answer has :" << answer.size() << " steps" << endl;
    cout << "the amount of calculated collections are : " << mp.size() << endl;
    return 0;
}
