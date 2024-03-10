#include<bits/stdc++.h>
using namespace std;


vector<vector<int>> candidates; 

void solve(int depth, vector<int>& path, set<int>& seen){
    if(depth == 5){
        candidates.push_back(path);
        return;
    }
    for(int i=1; i<=9; i++){
        if(seen.count(i)) continue;
        seen.insert(i);
        path.push_back(i);
        solve(depth+1, path, seen);
        path.pop_back();
        seen.erase(i);
    }
}

void prepare(){
    // initialize with likelyhood at index 0. Then append contents after.
    vector<int> candidate = {0};
    set<int> seen;
    solve(0, candidate, seen);
    // printf("Count of all possible guesses: %ld\n", candidates.size());
}

string getUserInput(){
    string res;
    cin>>res;
    return res;
}

pair<int, int> getParsedResponse(string s){
    return {s[0]-'0', s[2]-'0'};
}

void produceMasks(vector<string>& masks, int A, int B){
    string mask = "";
    for(int i=0; i<A; i++) mask+='A';
    for(int i=0; i<B; i++) mask+='B';
    for(int i=0; i<5-A-B; i++) mask+='N';
    // printf("%s\n", mask.c_str());
    do{
        masks.push_back(mask);
    }while(next_permutation(mask.begin(), mask.end()));
}

int judge(vector<int> guess, vector<int> quasi){
    // calculate NANB
    int A=0, B=0;
    for(int i=1; i<quasi.size(); i++){
        for(int j=1; j<guess.size(); j++){
            if(i==j && quasi[i]==guess[j]) A++;
            if(i!=j && quasi[i]==guess[j]) B++;
        }
    }
    return A*10+B;
}

void updateLikelyhoodOfCandidates(vector<int> guess, int A, int B){
    for(vector<int>& quasi : candidates){
        if(judge(guess, quasi)==A*10+B){
            quasi[0]++; // update likelyhood
        }else{
            // TODO: Maybe set to zero likelyhood will be another better alg?
            // quasi[0] = 0;
        }
    }
}

void resetLikelyhood(){
    // remove zero likelyhood
    vector<vector<int>> result;
    for(auto& candidate : candidates) if(candidate[0] != 0) result.push_back(candidate);

    for(auto& candidate : result) candidate[0] = 0;
    
    candidates = result;
}


int main(){
    printf("5 DIGITS NUMBER GAME.\n");

    prepare();

    printf("Start Playing with the computer to see its power!!!\n");

    bool notGuessed = true;
    int round = 1;
    while(notGuessed){

        // guess
        stable_sort(candidates.begin(), candidates.end(), [&](vector<int>a, vector<int>b){ return a.front() > b.front(); });// sort by likelyhood.
        printf("Round %4d: The computer guessed: ", round++);
        for(int i=1; i<candidates.front().size(); i++) printf("%d ", candidates[0][i]);
        printf("\n");

        // explore probability through new information.
        string response = getUserInput();// in format ?A?B
        auto [A, B] = getParsedResponse(response);

        if(A == 5) break;

        // construct possible masks...
        // vector<string> masks;
        // produceMasks(masks, A, B);

        updateLikelyhoodOfCandidates(candidates.front(), A, B);

        resetLikelyhood();
        // else, remove the incorrect guess
        // candidates.erase(candidates.begin());
    }

    printf("Looks like the computer found the answer?\n");
}
